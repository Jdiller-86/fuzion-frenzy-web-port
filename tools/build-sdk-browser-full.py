#!/usr/bin/env python3
"""Build the full SDK kernel/HLE and reference renderer for local-ISO browser execution.

No game input is downloaded or executed by this command. Adapted SDK sources
stay in an ignored overlay. Upstream notice: runtime/browser/sdk/SDK-NOTICE.txt.
"""
from __future__ import annotations
import argparse
from concurrent.futures import ThreadPoolExecutor, as_completed
from contextlib import ExitStack
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
from tempfile import TemporaryDirectory

ROOT = Path(__file__).resolve().parents[1]
ARTIFACTS = ROOT / '.tools/artifacts'
WORKING_OUTPUT = ARTIFACTS / 'sdk-browser-full'
COMPARISON_OUTPUT = ARTIFACTS / 'sdk-browser-candidate'
REPLAY_SCRATCH = ARTIFACTS / 'sdk-browser-replay'
THINLTO_CACHE_BYTES = 128 * 1024 * 1024
BUILD_CACHE_BYTES = 384 * 1024 * 1024


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def trim_thinlto_cache(directory: Path) -> None:
    if directory.is_symlink():
        return
    entries = []
    for path in directory.glob('llvmcache-*'):
        if path.is_file() and not path.is_symlink():
            stat = path.stat()
            entries.append((stat.st_atime_ns, path.name, stat.st_size, path))
    total = sum(entry[2] for entry in entries)
    # LLD can retain more than its policy when the current link uses those
    # entries. Once it exits, retain the most recently read files within budget.
    for _, _, size, path in sorted(entries):
        if total <= THINLTO_CACHE_BYTES:
            break
        path.unlink(missing_ok=True)
        total -= size


def finish_build_cache(cache: Path) -> None:
    if cache.resolve() != cache or cache.is_symlink():
        raise ValueError(f'Build cache must be a local directory: {cache}')
    size = sum(path.stat().st_size for path in cache.rglob('*')
               if not path.is_symlink() and path.is_file())
    if size > BUILD_CACHE_BYTES:
        shutil.rmtree(cache)
        print(f'Discarded the {size} byte build cache (384 MiB limit).', file=sys.stderr)


def isolated_environment(directory: Path) -> dict[str, str]:
    return {**os.environ, **dict.fromkeys(('TMPDIR', 'TEMP', 'TMP'), str(directory))}


def replay(command: list[str], output: Path, scratch: Path, *, candidate: bool = False) -> int:
    if scratch.resolve() != scratch:
        raise ValueError(f'Replay scratch must be a local directory: {scratch}')
    try:
        scratch.mkdir(parents=True)
    except FileExistsError:
        raise ValueError(f'Replay scratch already exists at {scratch}. '
                         f'Remove it after the previous replay has stopped.') from None
    try:
        env = isolated_environment(scratch)
        env['FUZION_BROWSER_SCRATCH'] = str(scratch)
        env['FUZION_BROWSER_BUILD'] = str(output)
        env.pop('FUZION_BROWSER_CANDIDATE', None)
        if candidate:
            env['FUZION_BROWSER_CANDIDATE'] = str(output)
        return subprocess.run(command, env=env, check=False).returncode
    finally:
        if scratch.resolve() != scratch:
            raise ValueError(f'Replay scratch became a linked directory: {scratch}')
        if scratch.exists():
            shutil.rmtree(scratch)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sdk', type=Path, default=ROOT / '.tools/XboxXBERecomp')
    parser.add_argument('--config', type=Path, default=ROOT / '.tools/fuzion-native/config/game.toml')
    parser.add_argument('--generated', type=Path, default=ROOT / 'research/verification/sdk-title-bulk-command-20261107/generated')
    parser.add_argument('--output', type=Path,
                        help='Select the working output, or the managed candidate slot with --compare')
    parser.add_argument('--compiler', default=shutil.which('em++'))
    parser.add_argument('--jobs', type=int, default=2)
    parser.add_argument('--opt', choices=['-O0', '-O1', '-O2', '-O3'], default='-O3')
    parser.add_argument('--link-opt', choices=['-O0', '-O1', '-O2', '-O3'], default=None,
                        help='Override link optimization for incremental development; objects retain --opt')
    parser.add_argument('--lto', choices=['off', 'thin', 'full'], default='off',
                        help='Enable LLVM link-time optimization for the browser module')
    parser.add_argument('--host-only', action='store_true', help='Compile runtime integration before title shards')
    parser.add_argument('--no-link', action='store_true')
    parser.add_argument('--keep-build-cache', action='store_true',
                        help='Keep an incremental cache up to 384 MiB in the selected output')
    parser.add_argument('--scene-timing', action='store_true',
                        help='Instrument the pinned scene loader and compile the opt-in texture timing probe')
    parser.add_argument('--exceptions',choices=['js','wasm'],default='wasm',help='C++ exception backend; js retains the slower development fallback')
    replay_modes = parser.add_mutually_exclusive_group()
    replay_modes.add_argument('--compare', nargs=argparse.REMAINDER, metavar='COMMAND',
                              help='Build a disposable candidate, run this command, and clean up both. Put this option last.')
    replay_modes.add_argument('--replay', nargs=argparse.REMAINDER, metavar='COMMAND',
                              help='Run this command with the existing build and one disposable scratch slot. Put this option last.')
    args = parser.parse_args()
    if args.replay is not None:
        if not args.replay or args.output or args.host_only or args.no_link or args.keep_build_cache:
            parser.error('--replay requires a command and uses the existing working output')
        if not (WORKING_OUTPUT / 'fuzion-sdk-full.wasm').is_file():
            parser.error(f'No runnable browser build at {WORKING_OUTPUT}')
        try:
            return replay(args.replay, WORKING_OUTPUT, REPLAY_SCRATCH)
        except ValueError as error:
            parser.error(str(error))
    if not args.compiler:
        parser.error('em++ missing: source .tools/emscripten-env.sh or select an Emscripten compiler')
    if args.jobs < 1:
        parser.error('--jobs must be positive')
    comparison = args.compare is not None
    output = (args.output or (COMPARISON_OUTPUT if comparison else WORKING_OUTPUT)).resolve()
    if output not in (WORKING_OUTPUT, COMPARISON_OUTPUT):
        parser.error('Select the working output or the single sdk-browser-candidate comparison slot')
    if output == COMPARISON_OUTPUT and not comparison:
        parser.error('The candidate slot requires --compare COMMAND to clean up the build and replay on exit')
    if comparison and (not args.compare or output != COMPARISON_OUTPUT or args.host_only or args.no_link or args.keep_build_cache):
        parser.error('--compare needs a command and a complete uncached build in the candidate slot')
    try:
        output.mkdir(parents=True, exist_ok=not comparison)
    except FileExistsError:
        parser.error(f'Candidate slot already exists at {output}. Remove it after its replay has stopped.')
    with ExitStack() as cleanup:
        run = output / '.build'
        try:
            run.mkdir()
        except FileExistsError:
            parser.error(f'Build workspace exists at {run}. If its build has stopped, remove it before retrying.')
        cleanup.callback(shutil.rmtree, output if comparison else run)
        cache = output / 'build-cache'
        keep_cache = args.keep_build_cache or args.host_only or args.no_link
        if cache.is_symlink() or cache.resolve() != cache:
            parser.error(f'Build cache must be a local directory: {cache}')
        if not keep_cache and cache.exists():
            shutil.rmtree(cache)
        work = cache if keep_cache else run / 'intermediates'
        work.mkdir(exist_ok=True)
        compiler_temp = run / 'compiler'
        compiler_temp.mkdir()
        compiler_env = isolated_environment(compiler_temp)
        compiler_env['EMCC_TEMP_DIR'] = str(compiler_temp)
        try:
            status = build(args, parser, output, work, run, compiler_env)
        finally:
            if keep_cache:
                finish_build_cache(cache)
        if status or not comparison:
            return status
        return replay(args.compare, output, run / 'replay', candidate=True)


def build(args: argparse.Namespace, parser: argparse.ArgumentParser,
          output: Path, work: Path, run: Path, compiler_env: dict[str, str]) -> int:
    sdk, generated = args.sdk.resolve(), args.generated.resolve()
    overlay = work / 'sdk-overlay'
    subprocess.run([sys.executable, str(ROOT / 'tools/prepare-sdk-browser-overlay.py'), '--sdk', str(sdk), '--output', str(overlay)], check=True)
    subprocess.run([sys.executable, str(ROOT / 'tools/prepare-sdk-host-header.py'), '--config', str(args.config), '--xdk', str(sdk / 'xdk'), '--output', str(work / 'image_symbols.h')], check=True)
    title_overlay = work / 'title-overlay'
    title_command = [sys.executable, str(ROOT / 'tools/prepare-sdk-browser-tailcalls.py'),
                     '--generated', str(generated), '--output', str(title_overlay)]
    if args.scene_timing:
        title_command.append('--scene-timing')
    subprocess.run(title_command, check=True)
    includes = [overlay, work, title_overlay, generated, sdk / 'harness', ROOT / 'runtime/browser/sdk']
    subprocess.run([sys.executable,str(ROOT/'tools/prepare-sdk-browser-fixed.py'),
                    '--sdk',str(sdk),'--output',str(work)],check=True)
    exception_flag='-fwasm-exceptions' if args.exceptions=='wasm' else '-fexceptions'
    common = [args.compiler, '-std=c++20', args.opt, '-DNDEBUG', '-msimd128', '-pthread', exception_flag, '-fno-strict-aliasing',
              '-DXBE_RAW_VERTICES=0', '-DXBE_RENDER_THREAD=0', '-DXBE_BROWSER=1']
    if args.lto != 'off':
        common.append(f'-flto={args.lto}')
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
    reference = work / 'reference_renderer.cpp'
    reference_text = (sdk / 'harness/reference_renderer.cpp').read_text()
    image_fields = '    std::vector<Vec> texels[6];'
    if reference_text.count(image_fields) != 1:
        raise RuntimeError('Reference image storage anchor differs from retained source')
    reference_text = reference_text.replace(
        image_fields,
        image_fields + '\n    std::shared_ptr<const uint8_t[]> gpu_pixels;\n'
        '    size_t gpu_pixel_capacity = 0;\n    uint32_t gpu_encoding = 0;',
        1)
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
    assembled_draw = '    if (!Assemble(s, prim, start, count, indices, as) || as.topology != Topology::Triangles) return;'
    if reference_text.count(assembled_draw) != 1:
        raise RuntimeError('Reference draw assembly seam differs from retained source')
    reference_text = reference_text.replace(assembled_draw, assembled_draw + '''
    uint64_t prepare_revision = 0;
    if (as.vertices.empty() && as.buffer_count == 1 && ff_webgl_enabled() && ff_webgl_gpu_vertex_enabled()) {
        prepare_revision = ff_webgl_begin_prepare_raw_draw();
        if (ff_webgl_reuse_raw_draw(as, prepare_revision)) return;
    }''', 1)
    anchor = '    // raster and output state'
    if reference_text.count(anchor) != 1:
        raise RuntimeError('Reference renderer vertex bridge anchor differs from retained source')
    bridge_hash = digest(ROOT / 'runtime/browser/sdk/webgl-draw.inc') + digest(ROOT / 'runtime/browser/sdk/webgl-images.inc')
    raw_vertex = '''    if(ff_webgl_enabled() && ff_webgl_gpu_vertex_enabled()) {
        const bool stream=as.vertices.empty();
        if(stream && as.buffer_count!=1)std::abort();
        const void* bridge_vertices=stream?as.buffers[0].src:static_cast<const void*>(as.vertices.data());
        uint32_t bridge_count=as.vertex_count,bridge_stride=stream?as.buffers[0].stride:uint32_t(regs)*16,bridge_registers=as.regs;
        uint32_t layout[16]={},bgra=0;
        if(stream)for(int r=0;r<16;++r)if(as.regs&(1u<<r)) {
            const auto& e=as.elements[r];if(e.buffer)std::abort();
            layout[r]=uint32_t(e.format)|(uint32_t(e.offset)<<8);
            if(e.format==87)bgra|=1u<<r;
        }
        const uint32_t* bridge_layout=stream?layout:nullptr;
        const char* bridge_vertex_shader=program?ff_webgl_vertex_source(s,as.regs,as.decode,bgra):ff_webgl_fixed_source(s,as.regs,as.pretransformed,as.decode,bgra);
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
        const uint32_t* bridge_layout=nullptr;
        const char* bridge_vertex_shader=nullptr;
#include "webgl-draw.inc"
    }
'''
    reference_text = '// Authored vertex bridge: ' + bridge_hash + '\n#include "full-webgl.h"\n#include "full-memory.h"\n#include <array>\n#include <cmath>\n#include <limits>\n#include <optional>\n#include <tuple>\n#include <utility>\nbool ff_webgl_equal_bytes(const void*,const void*,size_t);\nconst uint32_t* ff_webgl_palette(uint32_t, uint64_t&);\nconst float* ff_webgl_program_constants(const void*,const float*);\nvoid ff_webgl_image_owner(const void*,uint64_t,const std::shared_ptr<const void>&,uint32_t,uint32_t,bool);\nbool ff_webgl_gpu_vertex_enabled();\nconst char* ff_webgl_vertex_source(const gpu::State&,uint32_t,uint64_t,uint32_t);\nconst char* ff_webgl_fixed_source(const gpu::State&,uint32_t,bool,uint64_t,uint32_t);\nnamespace gpu { struct Assembled; struct PixelKey; }\nstd::string ff_webgl_pixel_source(const gpu::PixelKey&);\nuint64_t ff_webgl_begin_prepare_raw_draw();\nbool ff_webgl_reuse_raw_draw(const gpu::Assembled&, uint64_t);\nbool ff_webgl_draw_raw(const gpu::Assembled&,const uint32_t*,const float*,const char*,const uint32_t*,const uint32_t*,uint64_t);\nfloat* ff_webgl_fixed_constants(const gpu::State&,uint32_t,uint32_t);\n' + reference_text.replace(anchor,evaluated_vertex+anchor)
    texture_interface = '''#include <memory>
#include "gpu/texture.h"
namespace gpu {
std::shared_ptr<uint8_t[]> DecodeBrowserColor(const TextureDesc&, const uint8_t*, std::size_t, const uint32_t*);
#ifdef FF_BROWSER_TEXTURE_PROBE
int BrowserTextureProbeMode();
void BrowserTextureProbeMark(double, unsigned, unsigned, unsigned, unsigned, unsigned);
#endif
}
'''
    reference.write_text(texture_interface + reference_text)
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
    compiler_version = subprocess.check_output([args.compiler, '--version'], env=compiler_env, text=True).splitlines()[0]
    objects_dir = work / 'objects'
    objects_dir.mkdir(exist_ok=True)

    def compile_source(source: Path):
        key = hashlib.sha256(str(source).encode()).hexdigest()[:12]
        obj = objects_dir / (source.stem + '-' + key + '.o')
        source_flags = ['-msimd128', '-msse4.1'] if source.name == 'render_thread.cpp' else ['-mtail-call'] if source.parent == title_overlay else []
        if args.scene_timing and source == overlay / 'kernel/objects.cpp':
            source_flags.append('-DFF_BROWSER_DISPATCHER_PROBE')
        if args.scene_timing and source in (overlay / 'gpu/texture.cpp', reference):
            source_flags.append('-DFF_BROWSER_TEXTURE_PROBE')
        source_digest=digest(source)
        if source.name=='full-webgl.cpp':source_digest+=digest(work/'fixed-constants.inc')
        if source == overlay / 'xdk/d3d8.cpp':
            source_digest += digest(ROOT / 'runtime/browser/sdk/guest-vertex-lock.inc')
        signature = hashlib.sha256(json.dumps([common, source_flags, compiler_version, header_signature, source_digest]).encode()).hexdigest()
        stamp = obj.with_suffix('.signature')
        if obj.is_file() and stamp.is_file() and stamp.read_text() == signature:
            return obj, True, ''
        temporary = obj.with_suffix('.tmp.o')
        try:
            result = subprocess.run([*common, *source_flags, '-c', str(source), '-o', str(temporary)],
                                    env=compiler_env, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
            if result.returncode:
                return None, False, f'{source}\n{result.stdout[-16000:]}'
            temporary.replace(obj)
        finally:
            temporary.unlink(missing_ok=True)
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
    exports = ['_ff_browser_boot', '_ff_browser_set_pad', '_ff_browser_fallback_vertex_template',
               '_ff_browser_warm_guest_code', '_ff_browser_warm_guest_code_extra', '_malloc', '_free']
    thinlto_cache = work / 'thinlto-cache'
    lto_flags = []
    if args.lto == 'thin':
        lto_flags = [
            f'-Wl,--thinlto-jobs={args.jobs}',
            f'-Wl,--thinlto-cache-dir={thinlto_cache}',
            f'-Wl,--thinlto-cache-policy=cache_size_bytes={THINLTO_CACHE_BYTES}:prune_interval=0s',
        ]
    with TemporaryDirectory(prefix='link-', dir=run) as staging:
        link_output = Path(staging)
        link = [*common, args.link_opt or args.opt, '-msimd128', '-mtail-call', *lto_flags, *map(str, built), '--profiling-funcs', '--emit-symbol-map', '--no-entry', '-sMODULARIZE=1', '-sEXPORT_ES6=1',
                '-sEXPORT_NAME=createFuzionSdkRuntime', '-sENVIRONMENT=web,worker',
                '-sINITIAL_MEMORY=536870912', '-sMAXIMUM_MEMORY=2147483648', '-sALLOW_MEMORY_GROWTH=1',
                '-sSTACK_SIZE=8388608', '-sDEFAULT_PTHREAD_STACK_SIZE=2097152', '-sPTHREAD_POOL_SIZE=12',
                '-sPTHREAD_POOL_SIZE_STRICT=0', '-sEXIT_RUNTIME=0', '-sFORCE_FILESYSTEM=1',
                '-lworkerfs.js', '-lidbfs.js', '-sEXPORTED_FUNCTIONS=' + json.dumps(exports),
                '-sEXPORTED_RUNTIME_METHODS=' + json.dumps(['FS', 'WORKERFS', 'IDBFS', 'HEAPU8', 'HEAPF32', 'PThread']),
                '-o', str(link_output / 'fuzion-sdk-full.mjs')]
        try:
            subprocess.run(link, env=compiler_env, check=True)
        finally:
            if args.lto == 'thin':
                trim_thinlto_cache(thinlto_cache)
        # Check all required outputs before replacing the working module.
        for name in ('fuzion-sdk-full.wasm', 'fuzion-sdk-full.mjs'):
            if not (link_output / name).is_file():
                raise FileNotFoundError(link_output / name)
        for name in ['fuzion-sdk-full.wasm', 'fuzion-sdk-full.worker.js', 'fuzion-sdk-full.mjs']:
            if (link_output / name).is_file():
                (link_output / name).replace(output / name)
    revision = subprocess.check_output(['git', '-C', str(sdk), 'rev-parse', 'HEAD'], text=True).strip()
    (output / 'full-build-manifest.json').write_text(json.dumps({
        'sdkCommit': revision, 'compiler': compiler_version, 'optimization': args.opt, 'linkOptimization': args.link_opt or args.opt,
        'lto': args.lto,
        'sceneTiming': args.scene_timing,
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
    live_objects = {path.name for obj in built for path in (obj, obj.with_suffix('.signature'))}
    for cached in objects_dir.iterdir():
        if cached.name not in live_objects and re.fullmatch(r'.+-[0-9a-f]{12}\.(?:o|signature)', cached.name) and cached.is_file():
            cached.unlink(missing_ok=True)
    print(f'Full SDK browser module: {output / "fuzion-sdk-full.mjs"}')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
