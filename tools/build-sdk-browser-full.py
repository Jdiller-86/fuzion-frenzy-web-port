#!/usr/bin/env python3
"""Build the full SDK kernel/HLE and reference renderer for local-ISO browser execution.

No game input is downloaded or executed by this command. Adapted SDK sources
stay in an ignored overlay. Upstream notice: runtime/browser/sdk/SDK-NOTICE.txt.
"""
from __future__ import annotations
import argparse
from concurrent.futures import ThreadPoolExecutor, as_completed
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sdk', type=Path, default=ROOT / '.tools/XboxXBERecomp')
    parser.add_argument('--config', type=Path, default=ROOT / '.tools/fuzion-native/config/game.toml')
    parser.add_argument('--generated', type=Path, default=ROOT / 'research/verification/sdk-title-bulk-command-20261107/generated')
    parser.add_argument('--output', type=Path, default=ROOT / '.tools/artifacts/sdk-browser-full')
    parser.add_argument('--compiler', default=shutil.which('em++'))
    parser.add_argument('--jobs', type=int, default=2)
    parser.add_argument('--opt', choices=['-O0', '-O1', '-O2', '-O3'], default='-O3')
    parser.add_argument('--link-opt', choices=['-O0', '-O1', '-O2', '-O3'], default=None,
                        help='Override link optimization for incremental development; objects retain --opt')
    parser.add_argument('--host-only', action='store_true', help='Compile runtime integration before title shards')
    parser.add_argument('--no-link', action='store_true')
    parser.add_argument('--exceptions',choices=['js','wasm'],default='wasm',help='C++ exception backend; js retains the slower development fallback')
    args = parser.parse_args()
    if not args.compiler:
        parser.error('em++ missing: source .tools/emscripten-env.sh or select an Emscripten compiler')
    if args.jobs < 1:
        parser.error('--jobs must be positive')
    sdk, generated, output = args.sdk.resolve(), args.generated.resolve(), args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    overlay = output / 'sdk-overlay'
    subprocess.run([sys.executable, str(ROOT / 'tools/prepare-sdk-browser-overlay.py'), '--sdk', str(sdk), '--output', str(overlay)], check=True)
    subprocess.run([sys.executable, str(ROOT / 'tools/prepare-sdk-host-header.py'), '--config', str(args.config), '--xdk', str(sdk / 'xdk'), '--output', str(output / 'image_symbols.h')], check=True)
    title_overlay = output / 'title-overlay'
    subprocess.run([sys.executable, str(ROOT / 'tools/prepare-sdk-browser-tailcalls.py'),
                    '--generated', str(generated), '--output', str(title_overlay)], check=True)
    includes = [overlay, output, title_overlay, generated, sdk / 'harness', ROOT / 'runtime/browser/sdk']
    subprocess.run([sys.executable,str(ROOT/'tools/prepare-sdk-browser-fixed.py'),
                    '--sdk',str(sdk),'--output',str(output)],check=True)
    exception_flag='-fwasm-exceptions' if args.exceptions=='wasm' else '-fexceptions'
    common = [args.compiler, '-std=c++20', args.opt, '-DNDEBUG', '-msimd128', '-pthread', exception_flag, '-fno-strict-aliasing',
              '-DXBE_RAW_VERTICES=0', '-DXBE_RENDER_THREAD=0', '-DXBE_BROWSER=1']
    for directory in includes:
        common += ['-I', str(directory)]
    runtime = []
    for group in ['kernel', 'cpu', 'xdk']:
        runtime += sorted((overlay / group).glob('*.cpp'))
    runtime = [path for path in runtime if path.name != 'memory.cpp']
    runtime += [overlay / 'core/log.cpp']
    runtime += [overlay / ('gpu/' + name) for name in ['texture.cpp', 'vertex.cpp', 'shaders.cpp', 'render_thread.cpp']]
    runtime += [ROOT / ('runtime/browser/sdk/' + name) for name in ['full-memory.cpp', 'full-platform.cpp', 'full-entry.cpp', 'full-webgl.cpp']]
    # The reference renderer uses kernel::HostPtr, already translated by the overlay.
    reference = output / 'reference_renderer.cpp'
    reference_text = (sdk / 'harness/reference_renderer.cpp').read_text()
    # P8 palettes are real resource inputs, including font transparency. The
    # upstream CPU harness omitted them; preserve the title's stage palette.
    reference_text = reference_text.replace('FindImage(uint32_t header)', 'FindImage(uint32_t header, uint32_t palette_header = 0)')
    reference_text = reference_text.replace('    if (auto it = g_images.find(key);', '    uint64_t palette_hash=0;\n    const uint32_t* palette=d.format==0x0B?ff_webgl_palette(palette_header,palette_hash):nullptr;\n    key ^= palette_hash;\n    key ^= DirtyEpoch(d.data);\n    if (auto it = g_images.find(key);')
    reference_text = reference_text.replace('DecodeLevel(d, 0, f, guest, nullptr, raw, pitch)', 'DecodeLevel(d, 0, f, guest, palette, raw, pitch)')
    reference_text = reference_text.replace('FindImage(s.textures[size_t(i)])', 'BrowserImage(s.textures[size_t(i)], s.palettes[size_t(i)],d)')
    image_anchor='void Written(uint32_t lo, uint32_t hi) {'
    if reference_text.count(image_anchor)!=1:
        raise RuntimeError('Reference image-cache seam differs')
    reference_text=reference_text.replace(image_anchor,'#include "webgl-images.inc"\n'+image_anchor+'\n    BrowserImagesWritten(lo,hi);')
    reference_text=reference_text.replace('    Assembled as;', '    static thread_local Assembled as;')
    anchor = '    // raster and output state'
    if reference_text.count(anchor) != 1:
        raise RuntimeError('Reference renderer vertex bridge anchor differs from retained source')
    bridge_hash = digest(ROOT / 'runtime/browser/sdk/webgl-draw.inc') + digest(ROOT / 'runtime/browser/sdk/webgl-images.inc')
    raw_vertex = '''    if(ff_webgl_enabled() && ff_webgl_gpu_vertex_enabled() && !as.vertices.empty()) {
        const void* bridge_vertices=as.vertices.data();
        uint32_t bridge_count=as.vertex_count,bridge_stride=uint32_t(regs)*16,bridge_registers=as.regs;
        const char* bridge_vertex_shader=program?ff_webgl_vertex_source(s,as.regs):ff_webgl_fixed_source(s,as.regs,as.pretransformed);
#include "webgl-draw.inc"
    }
'''
    vertex_anchor='    std::vector<Shaded> verts(as.vertex_count);'
    if reference_text.count(vertex_anchor)!=1:
        raise RuntimeError('Reference programmable vertex seam differs')
    reference_text=reference_text.replace(vertex_anchor,raw_vertex+vertex_anchor)
    evaluated_vertex='''    {
        const void* bridge_vertices=verts.data();
        uint32_t bridge_count=uint32_t(verts.size()),bridge_stride=sizeof(Shaded),bridge_registers=0;
        const char* bridge_vertex_shader=nullptr;
#include "webgl-draw.inc"
    }
'''
    reference_text = '// Authored vertex bridge: ' + bridge_hash + '\n#include "full-webgl.h"\n#include "full-memory.h"\n#include <array>\n#include <limits>\n#include <tuple>\nconst uint32_t* ff_webgl_palette(uint32_t, uint64_t&);\nvoid ff_webgl_image_owner(const void*,uint64_t,const std::shared_ptr<const void>&);\nbool ff_webgl_gpu_vertex_enabled();\nconst char* ff_webgl_vertex_source(const gpu::State&,uint32_t);\nconst char* ff_webgl_fixed_source(const gpu::State&,uint32_t,bool);\nvoid ff_webgl_fixed_constants(const gpu::State&,uint32_t,uint32_t,float*);\n' + reference_text.replace(anchor,evaluated_vertex+anchor)
    reference.write_text(reference_text)
    runtime += [reference]
    original_title = sorted(generated.glob('x86_recomp.[0-9]*.cpp')) + [generated / 'x86_func_mapping.cpp']
    title = [title_overlay / path.name for path in original_title]
    if len(title) != 65:
        parser.error('Expected retained 64 title shards and function mapping')
    sources = runtime if args.host_only else runtime + title
    header_state = hashlib.sha256()
    for directory in includes:
        for path in sorted(directory.rglob('*.h')):
            header_state.update(str(path).encode()); header_state.update(path.read_bytes())
    header_signature = header_state.hexdigest()
    compiler_version = subprocess.check_output([args.compiler, '--version'], text=True).splitlines()[0]
    objects_dir = output / 'objects'
    objects_dir.mkdir(exist_ok=True)

    def compile_source(source: Path):
        key = hashlib.sha256(str(source).encode()).hexdigest()[:12]
        obj = objects_dir / (source.stem + '-' + key + '.o')
        source_flags = ['-msimd128', '-msse4.1'] if source.name == 'render_thread.cpp' else ['-mtail-call'] if source.parent == title_overlay else []
        source_digest=digest(source)
        if source.name=='full-webgl.cpp':source_digest+=digest(output/'fixed-constants.inc')
        signature = hashlib.sha256(json.dumps([common, source_flags, compiler_version, header_signature, source_digest]).encode()).hexdigest()
        stamp = obj.with_suffix('.signature')
        if obj.is_file() and stamp.is_file() and stamp.read_text() == signature:
            return obj, True, ''
        temporary = obj.with_suffix('.tmp.o')
        result = subprocess.run([*common, *source_flags, '-c', str(source), '-o', str(temporary)], text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        if result.returncode:
            return None, False, f'{source}\n{result.stdout[-16000:]}'
        temporary.replace(obj)
        stamp.write_text(signature)
        return obj, False, ''

    built = []
    with ThreadPoolExecutor(max_workers=args.jobs) as pool:
        tasks = {pool.submit(compile_source, source): source for source in sources}
        for future in as_completed(tasks):
            obj, reused, error = future.result()
            if error:
                for task in tasks: task.cancel()
                print(error, file=sys.stderr)
                return 1
            built.append(obj)
            print(f'{"reuse" if reused else "compile"} {tasks[future].name}', flush=True)
    if args.host_only or args.no_link:
        print(f'Compiled {len(built)} real SDK integration objects; no title execution claimed')
        return 0
    exports = ['_ff_browser_boot', '_ff_browser_set_pad', '_malloc', '_free']
    link_output = output / 'link-stage'
    link_output.mkdir(exist_ok=True)
    link = [*common, args.link_opt or args.opt, '-msimd128', '-mtail-call', *map(str, built), '--profiling-funcs', '--emit-symbol-map', '--no-entry', '-sMODULARIZE=1', '-sEXPORT_ES6=1',
            '-sEXPORT_NAME=createFuzionSdkRuntime', '-sENVIRONMENT=web,worker',
            '-sINITIAL_MEMORY=536870912', '-sMAXIMUM_MEMORY=2147483648', '-sALLOW_MEMORY_GROWTH=1',
            '-sSTACK_SIZE=8388608', '-sDEFAULT_PTHREAD_STACK_SIZE=2097152', '-sPTHREAD_POOL_SIZE=12',
            '-sPTHREAD_POOL_SIZE_STRICT=0', '-sEXIT_RUNTIME=0', '-sFORCE_FILESYSTEM=1',
            '-lworkerfs.js', '-lidbfs.js', '-sEXPORTED_FUNCTIONS=' + json.dumps(exports),
            '-sEXPORTED_RUNTIME_METHODS=' + json.dumps(['FS', 'WORKERFS', 'IDBFS', 'HEAPU8', 'HEAPF32', 'PThread']),
            '-o', str(link_output / 'fuzion-sdk-full.mjs')]
    subprocess.run(link, check=True)
    # Never serve a partially written wasm paired with the previous JS glue.
    for name in ['fuzion-sdk-full.wasm', 'fuzion-sdk-full.worker.js', 'fuzion-sdk-full.mjs']:
        if (link_output / name).is_file():
            (link_output / name).replace(output / name)
    revision = subprocess.check_output(['git', '-C', str(sdk), 'rev-parse', 'HEAD'], text=True).strip()
    (output / 'full-build-manifest.json').write_text(json.dumps({
        'sdkCommit': revision, 'compiler': compiler_version, 'optimization': args.opt, 'linkOptimization': args.link_opt or args.opt,
        'exceptions':args.exceptions,
        'sourceCount': len(sources), 'moduleBytes': (output / 'fuzion-sdk-full.wasm').stat().st_size,
        'runtimePatchManifestSha256': digest(ROOT / 'desktop/windows/patches/manifest.json'),
        'rendererBackends': ['webgl2-triangles', 'software-reference'],
        'textureCascadeFix': 'Published shader-key fix consumed by both browser backends',
        'descriptorRolloverFix': 'Preserved D3D12-only fix; browser uses WebGL resources',
        'tailTransfers': json.loads((title_overlay / 'tailcall-manifest.json').read_text()),
        'evidence': 'full SDK and title compilation/link only; original execution requires player-local image',
        'generatedSources': {path.name: digest(path) for path in original_title},
    }, indent=2) + '\n')
    print(f'Full SDK browser module: {output / "fuzion-sdk-full.mjs"}')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
