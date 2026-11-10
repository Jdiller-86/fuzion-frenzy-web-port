const runtime = (name) => `runtime/core/${name}`;
const optimized = ['-std=c11', '-O2', '-Wall', '-Wextra', '-Werror', '-I', 'runtime/core'];
const sanitizer = ['-std=c11', '-O1', '-g', '-Wall', '-Wextra', '-Werror', '-I', 'runtime/core',
  '-fsanitize=address,undefined', '-fno-sanitize-recover=all'];

/* Every link set is explicit. CI-only historical recipes remain named variants
 * where their sources or flags differ from the corresponding native fixture. */
const sources = {
  sha256: ['tests/sha256_test.c', runtime('sha256.c')],
  kernel_capture: ['tests/kernel_capture_test.c', runtime('kernel_capture.c'), runtime('sha256.c')],
  trace_capture: ['tests/trace_capture_test.c', runtime('trace_capture.c'), runtime('sha256.c')],
  performance_clock: ['tests/performance_clock_test.c', runtime('performance_clock.c')],
  unnamed_mutants: ['tests/unnamed_mutants_test.c', runtime('unnamed_mutants.c'), runtime('virtual_memory.c')],
  thread_bootstrap: ['tests/thread_bootstrap_test.c', runtime('thread_bootstrap.c')],
  irql_pair: ['tests/irql_pair_test.c', runtime('irql_pair.c'), runtime('virtual_memory.c')],
  memory_statistics: ['tests/memory_statistics_test.c', runtime('memory_statistics.c'), runtime('virtual_memory.c')],
  named_events: ['tests/named_events_test.c', runtime('named_events.c'), runtime('virtual_memory.c')],
  local_mutants: ['tests/local_mutants_test.c', runtime('local_mutants.c'), runtime('named_events.c'), runtime('virtual_memory.c')],
  local_mutant_acquire: ['tests/local_mutant_acquire_test.c', runtime('local_mutants.c'), runtime('named_events.c'), runtime('virtual_memory.c')],
  local_mutant_release: ['tests/local_mutant_release_test.c', runtime('local_mutants.c'), runtime('named_events.c'), runtime('virtual_memory.c')],
  device_registers: ['tests/device_registers_test.c', runtime('device_registers.c'), runtime('thread_bootstrap.c')],
  pcrtc_state: ['tests/pcrtc_state_test.c', runtime('pcrtc_state.c')],
  kernel_timers: ['tests/kernel_timers_test.c', runtime('kernel_timers.c'), runtime('kernel_objects.c')],
  port_io: ['tests/port_io_test.c', runtime('port_io.c')],
  controller_device: ['tests/controller_device_test.c', runtime('controller_device.c'), runtime('controller_port.c'), runtime('controller_packet.c')],
  controller_packet: ['tests/controller_packet_test.c', runtime('controller_packet.c')],
  controller_port: ['tests/controller_port_test.c', runtime('controller_port.c'), runtime('controller_packet.c')],
  pfifo_status: ['tests/pfifo_status_test.c', runtime('pfifo_status.c')],
  guest_mirror: ['tests/guest_mirror_test.c', runtime('guest_mirror.c')],
  guest_strings: ['tests/guest_strings_test.c', runtime('guest_strings.c'), runtime('virtual_memory.c')],
  debug_service: ['tests/debug_service_test.c', runtime('debug_service.c'), runtime('guest_strings.c'), runtime('virtual_memory.c')],
  guest_write_map: ['tests/guest_write_map_test.c', runtime('guest_write_map.c')],
  pfifo_channel: ['tests/pfifo_channel_test.c', runtime('pfifo_channel.c')],
  pfifo_dma: ['tests/pfifo_dma_test.c', runtime('pfifo_dma.c')],
  pfifo_pusher: ['tests/pfifo_pusher_test.c', runtime('pfifo_pusher.c'), runtime('pfifo_status.c')],
  vblank_wait: ['tests/vblank_wait_test.c', runtime('vblank_wait.c')],
  display_mode: ['tests/display_mode_test.c', runtime('display_mode.c')],
  av_unknown_option: ['tests/av_unknown_option_test.c', runtime('av_unknown_option.c')],
  av_saved_data: ['tests/av_saved_data_test.c', runtime('av_saved_data.c')],
  av_null_result: ['tests/av_null_result_test.c', runtime('av_null_result.c')],
  mm_free_contiguous: ['tests/mm_free_contiguous_test.c', runtime('mm_free_contiguous.c')],
  tls_scope: ['tests/tls_scope_test.c', runtime('tls_scope.c')],
  interrupt_vector: ['tests/interrupt_vector_test.c', runtime('interrupt_vector.c'), runtime('virtual_memory.c')],
  reg_save_order: ['tests/reg_save_order_test.c', runtime('reg_save_order.c')],
  reg_save_compiled_probe: ['tests/reg_save_compiled_probe.c'],
  av_query_fixture: ['tests/av_query_fixture_test.c', runtime('av_query.c'), runtime('virtual_memory.c'), runtime('thread_bootstrap.c')],
  av_query_adapter: ['tests/av_query_adapter_test.c', runtime('av_query_adapter.c'), runtime('av_query.c'), runtime('virtual_memory.c'), runtime('thread_bootstrap.c')],
  fatx_volume: ['tests/fatx_volume_test.c', runtime('fatx_volume.c'), runtime('local_disk.c'), runtime('file_namespace.c'), runtime('local_volume.c'), runtime('local_directories.c'), runtime('object_namespace.c'), runtime('virtual_memory.c'), runtime('thread_bootstrap.c')],
  local_disc: ['tools/fixtures/local_disc_check.c', runtime('local_disc.c'), runtime('object_namespace.c'), runtime('virtual_memory.c'), runtime('thread_bootstrap.c')],
  recomp_memory_access_xmm: ['tests/recomp_memory_access_xmm_test.c'],
  recomp_memory_access_vm: ['tests/recomp_memory_access_vm_test.c', runtime('virtual_memory.c'), runtime('thread_bootstrap.c')],
};

const fixtures = Object.entries(sources).map(([name, sourceFiles]) => ({
  name, sources: sourceFiles, modes: [
    { name: '', flags: optimized },
    { name: ' (asan/ubsan)', suffix: '-san', flags: sanitizer },
  ],
}));

const ci = (name, sourceFiles, flags = optimized, mode = { name: '', flags }) =>
  fixtures.push({ name, sources: sourceFiles, modes: [mode] });
const geom = ['-std=c11', '-O2', '-Wall', '-Wextra', '-Werror', '-ffp-contract=off'];
ci('nv2a_triangle', ['tests/nv2a_triangle_test.c', 'renderer/nv2a_triangle.c', '-lm'], geom);
ci('nv2a_line', ['tests/nv2a_line_test.c', 'renderer/nv2a_line.c', '-lm'], geom);
ci('nv2a_quad', ['tests/nv2a_quad_test.c', 'renderer/nv2a_quad.c', '-lm'], geom);
ci('kernel_objects', ['tests/kernel_objects_test.c', runtime('kernel_objects.c')]);
ci('local_settings', ['tests/local_settings_test.c', runtime('local_settings.c')]);
ci('dpc_context', ['tests/dpc_context_test.c', runtime('dpc_context.c'), runtime('kernel_timers.c'), runtime('kernel_objects.c')]);
ci('virtual_memory', ['tests/virtual_memory_test.c', runtime('virtual_memory.c')]);
ci('memory_access', ['tests/memory_access_test.c', runtime('virtual_memory.c'), runtime('device_registers.c')],
  [...optimized, '-I', '{scratch}']);
ci('local_volume', ['tests/local_volume_test.c', runtime('local_volume.c'), runtime('virtual_memory.c')]);
ci('local_directories', ['tests/local_directories_test.c', runtime('local_directories.c'), runtime('local_volume.c'), runtime('guest_strings.c'), runtime('virtual_memory.c')]);
ci('object_namespace', ['tests/object_namespace_test.c', runtime('object_namespace.c'), runtime('virtual_memory.c')]);
ci('file_namespace', ['tests/file_namespace_test.c', runtime('file_namespace.c'), runtime('object_namespace.c'), runtime('local_directories.c'), runtime('local_volume.c'), runtime('virtual_memory.c')]);
ci('local_files', ['tests/local_files_test.c', runtime('local_files.c'), runtime('local_directories.c'), runtime('local_volume.c'), runtime('virtual_memory.c')]);
ci('image_sections', ['tests/image_sections_test.c', runtime('image_sections.c'), runtime('virtual_memory.c')]);
ci('console_settings', ['tests/console_settings_test.c', runtime('console_settings.c'), runtime('virtual_memory.c')]);
ci('local_disk', ['tests/local_disk_test.c', runtime('local_disk.c'), runtime('virtual_memory.c')]);
ci('fatx_root', ['tests/fatx_volume_test.c', runtime('fatx_volume.c'), runtime('local_disk.c'), runtime('object_namespace.c'), runtime('file_namespace.c'), runtime('local_volume.c'), runtime('local_directories.c'), runtime('virtual_memory.c')]);
ci('av_query_ci', ['tests/av_query_fixture_test.c', runtime('av_query.c'), runtime('virtual_memory.c')]);
fixtures.at(-1).modes.push({ name: ' (asan/ubsan)', suffix: '-san', flags: ['-std=c11', '-O1', '-g', '-fsanitize=address,undefined', '-fno-omit-frame-pointer', '-Wall', '-Wextra', '-Werror'] });
ci('device_registers_ci', ['tests/device_registers_test.c', runtime('device_registers.c')]);
ci('gpu_instance', ['tests/gpu_instance_test.c', runtime('gpu_instance.c'), runtime('virtual_memory.c')]);
ci('time_fields', ['tests/time_fields_test.c', runtime('time_fields.c')]);

// CI's owned-memory case depends on this generated include, emitted in scratch.
fixtures.find((fixture) => fixture.name === 'memory_access').prepare = {
  command: 'python', args: ['translator/recomp_memory_access.py', '{scratch}/owned-memory.inc'],
};

export const nativeFixtures = fixtures;
