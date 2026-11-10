#include "shared-abi.h"
#include "local_disc.h"
#include "fatx_directory_cursor.h"
#include "controller_device.h"
#include "irql_pair.h"
static ff_irql_pair irql_pair;
static uint32_t irql_imports,irql_raw[2],irql_words[8];
static uint32_t sdk_irql_profile(uint32_t);
static uint32_t sdk_irql_validate(void);
static void sdk_irql_kernel(uint32_t,ff_sdk_scalar *);
static ff_controller_device host_controllers;
static uint32_t host_words[16];
#include "kernel_timers.h"
static uint32_t sdk_enabled, sdk_active, sdk_current_target, sdk_words[20];


static uint32_t scope41_words[48];
static ff_sdk_scalar scope41_child_entry;
enum { S41_ACTIVE=0,S41_STAGE,S41_PARENT,S41_CALLSITE,S41_CALLEE,S41_RETURN,
       S41_ENTRY_ESP,S41_ARG0,S41_EAX,S41_ECX,S41_EDX,S41_EBX,S41_ESP,
       S41_EBP,S41_ESI,S41_EDI,S41_FS,S41_DF,S41_PRE_ESP,S41_PRE_RA,
       S41_POST_EAX,S41_POST_ECX,S41_POST_EDX,S41_POST_EBX,S41_POST_ESP,
       S41_POST_EBP,S41_POST_ESI,S41_POST_EDI,S41_POST_FS,S41_POST_DF,
       S41_CHILD_STAGE,S41_CHILD_ESP,S41_CHILD_RA,S41_CHILD_ARG,S41_CHILD_PRE_ESP,
       S41_CHILD_EAX,S41_CHILD_ECX,S41_CHILD_EDX,S41_CHILD_EBX,S41_CHILD_POST_ESP,
       S41_CHILD_EBP,S41_CHILD_ESI,S41_CHILD_EDI,S41_CHILD_FS,S41_CHILD_DF,
       S41_REJECTED,S41_CHILD_ENTRY_ESI,S41_CHILD_ENTRY_EDI };

static ff_local_disc title_disc;
static uint32_t disc_words[32];
static uint32_t mapped_words[128];
static uint32_t tls_getter_words[64];
static uint8_t tls_getter_tls_before[20];
static uint32_t dosmap_words[192];
static uint8_t dosmap_ptd_before[132],dosmap_ptd_after[132];
static uint8_t mapped_tls_before[20],mapped_tls_kernel[20],mapped_tls_after[20];
static uint32_t sdk_denied_map(const uint32_t *,uint32_t *);
static void sdk_denied_return(const uint32_t *);
static void sdk_denied_trace(uint32_t,uint32_t);
static void sdk_denied_esp(const char *,const char *);
void ff_probe_sdk_tls_getter_continuation(void);

static uint32_t sdk_disc_provider(const uint32_t *,uint32_t *);
static void sdk_disc_return(const uint32_t *);
static void sdk_disc_bind_reached(void);

static uint32_t stdio_active,stdio_words[640];
static uint8_t stdio_inputs[19],stdio_object_before[32],stdio_object_after[32],stdio_locks_before[56];
static uint32_t sdk_stdio_enter(ff_boot_runtime *,uint32_t);
static uint32_t sdk_stdio_leave(ff_boot_runtime *,uint32_t);
static uint32_t stos_attempts,stos_completed,stos_eax,stos_ecx,stos_edi;
static uint32_t device_stos[5];
static uint32_t log_bridge_words[26];
static uint32_t second_log_bridge_words[26], second_query_words[8], title_return_words[18];
static uint32_t third_log_bridge_words[29], parent_setup_words[34], parent_fill_words[20];
static uint32_t third_log_memory_stage, third_log_memory_reads;
/* Two fixed diagnostic banks share the same original code and authoritative state.
 * MODE<=21 always uses bank0. MODE22 admits exactly the reached tail as bank1. */
typedef struct {
    uint32_t prefix[24],hash[24],call[32],wait[3][24],extra[40],kernel_regs[6];
    uint32_t node[48],release[64],continuation[48];
} ff_sdk_invocation_evidence;
static ff_sdk_invocation_evidence invocation_evidence[2];
static uint32_t sdk_invocation,tail_words[16];
/* MODE23 root/callee entry plus two checked original fills; no synthetic return. */
static uint32_t startup_words[64];
/* MODE24 directory candidate entry, existing289return and unserved202frame. */
static uint32_t directory_words[64];
/* MODE26 first directory query: no cursor or later call admission. */
static uint32_t query_words[64];
/* MODE27 helper plus the actual two caller returns; fixed observations only. */
static uint32_t copy_words[128];
static uint8_t copy_source[76],copy_before[305],copy_expected[305];
/* MODE28: one new caller of the unchanged retained C logger. */
static uint32_t record_log_words[64];
static uint32_t logger45[1024];
static void s45_trace(uint32_t,uint32_t);
static void s45_ledger_esp(const char *,const char *);
/* MODE29: exact first 4E960 invocation and its real return, when taken. */
static uint32_t gate_helper_words[64];
/* MODE30: one original comparison invocation; dependent locale helper guarded. */
typedef struct {uint32_t words[64];uint8_t name[13],literal[16];} ff_compare_evidence;
static ff_compare_evidence compare_evidence[2];
static uint32_t compare_index,compare_started,compare_returned;
/* MODE31: original next-directory entry and exact unserved kernel frame. */
static uint32_t next_directory_words[64];
/* MODE33 checked error service, original TLS store and actual enclosing returns. */
static uint32_t error_words[64],tls_words[96],error_outer_words[32];
/* MODE34 reuses the existing C close body; no original-body or SDK-map addition. */
static uint32_t close_words[128],close_parent_words[64];
static uint8_t error_tls_before[20],error_tls_after[20];
static ff_fatx_single_cursor directory_cursor;
static uint32_t next_query_words[64];
static uint8_t next_query_before[328],next_query_after[328],next_iosb_before[8],next_iosb_after[8];
#define compare_words (compare_evidence[compare_index].words)
#define compare_name (compare_evidence[compare_index].name)
#define compare_literal (compare_evidence[compare_index].literal)
#define prefix_words (invocation_evidence[sdk_invocation].prefix)
#define hash_words (invocation_evidence[sdk_invocation].hash)
#define call_words (invocation_evidence[sdk_invocation].call)
#define wait_frames (invocation_evidence[sdk_invocation].wait)
#define wait_extra (invocation_evidence[sdk_invocation].extra)
#define wait_kernel_regs (invocation_evidence[sdk_invocation].kernel_regs)
#define node_words (invocation_evidence[sdk_invocation].node)
#define release_words (invocation_evidence[sdk_invocation].release)
#define continuation_words (invocation_evidence[sdk_invocation].continuation)

typedef struct {
 uint32_t words[96]; ff_sdk_scalar entry,post;
 uint8_t old_nodes[2][0x50],sentinel_before[0x50],fresh_before[0x50],input[15],table[1024];
} ff_scope42_allocator;
static ff_scope42_allocator scope42_allocator;
static uint32_t scope42_root_seh;
enum { A42_ACTIVE=0,A42_STAGE,A42_ROOT,A42_CALLER,A42_SITE,A42_CALLEE,A42_RA,
 A42_ESP,A42_ARG0,A42_ARG1,A42_ARG2,A42_ARG3,A42_ARG4,A42_ARG5,
 A42_HASH_RETURN,A42_HASH_ESP,A42_ALLOC_ENTER,A42_ALLOC_PRE,A42_ALLOC_POST,
 A42_ALLOC_EAX,A42_POST_TRACE,A42_CHILD_30943,A42_CHILD_3082D,
 A42_CHILD_311F9,A42_KERNEL234,A42_NODE,A42_RELEASE,A42_CONT,A42_REJECTED,A42_PARENT_POST,
 A42_WAIT_ATTEMPTS,A42_WAIT_COMPLETED,A42_RELEASE_ATTEMPTS,A42_RELEASE_COMPLETED,
 A42_POINTER_READY,A42_WRITE_INDEX,A42_OLD_HASH0,A42_OLD_HASH1,A42_SENTINEL_HASH,
 A42_FRESH_HASH,A42_FINAL_LIST,A42_INPUT_HASH,A42_TABLE_HASH };
#define a42 (scope42_allocator.words)
static ff_sdk_invocation_evidence allocator42_evidence;
static const uint32_t *scope41_read(uint32_t,uint32_t,ff_sdk_scalar *);
static int scope41_same_scalar(const ff_sdk_scalar *,const ff_sdk_scalar *);
static int scope41_allocated_node(uint32_t,uint32_t,ff_sdk_scalar *);
static void scope41_validate_list(ff_sdk_scalar *);
uint32_t ff_probe_sdk_scope41_active(void);
static uint32_t sdk_wait_object(uint32_t,uint32_t);
static void sdk_scope42_capture_old(ff_sdk_scalar *);
static void sdk_scope42_pointer_proof(ff_sdk_scalar *);
static void sdk_scope42_prewrite(ff_sdk_scalar *);
static void sdk_scope42_write_check(uint32_t,uint32_t,uint32_t,ff_sdk_scalar *);
static void sdk_scope42_final_list(ff_sdk_scalar *);

static int sdk_scope42(void);
static ff_sdk_invocation_evidence *sdk_evidence(void);
static uint32_t sdk_scope42_word(uint32_t);
uint32_t ff_probe_sdk_scope42_begin_allocator(ff_sdk_scalar *);
void ff_probe_sdk_scope42_allocator_post(ff_sdk_scalar *);
void ff_probe_sdk_scope42_parent_post(ff_sdk_scalar *);
#undef prefix_words
#undef hash_words
#undef call_words
#undef wait_frames
#undef wait_extra
#undef wait_kernel_regs
#undef node_words
#undef release_words
#undef continuation_words
#define prefix_words (sdk_evidence()->prefix)
#define hash_words (sdk_evidence()->hash)
#define call_words (sdk_evidence()->call)
#define wait_frames (sdk_evidence()->wait)
#define wait_extra (sdk_evidence()->extra)
#define wait_kernel_regs (sdk_evidence()->kernel_regs)
#define node_words (sdk_evidence()->node)
#define release_words (sdk_evidence()->release)
#define continuation_words (sdk_evidence()->continuation)

/* Coordinator-authored, caller-bounded proof state; never writes guest RAM. */
enum { S43_ROOT=1,S43_ALLOC=2,S43_WRITES=3,S43_DONE=4,
       S43_META=64,S43_SCALARS=80,S43_EVIDENCE=358,
       S43_RECORD_WORDS=S43_META+S43_SCALARS+S43_EVIDENCE,
       S43_WORDS=8+4*S43_RECORD_WORDS };
typedef struct {
 uint32_t kind,q,header,extent,bytes,owner;
 uint8_t expected[0x420];
} ff_scope43_block;
typedef struct {
 uint32_t active,kind,stage,ret,esp,seh,args[3],input_span;
 uint32_t entered,hash_entered,hash_exited,hash_done,hash_eax,hash_esp;
 uint32_t alloc_seen,selected,selected_h,selected_size,total,header,extent,q;
 uint32_t split,pointer_ready,release_ra,wait_a,wait_c,release_a,release_c;
 uint32_t matched,old_tail,old_count,write_index,continuation,pre_seen,post_seen;
 uint32_t pre_site,result,post_esp,new_block,complete;
 ff_sdk_scalar root_entry,root_pre,root_post,allocator_entry,allocator_post;
 ff_sdk_scalar hash_entry,hash_pre,hash_post;
 uint8_t input[16];
 ff_sdk_invocation_evidence evidence;
} ff_scope43_call;
static ff_scope43_call scope43_calls[4];
static ff_scope43_block scope43_blocks[6];
static uint32_t scope43_count,scope43_current,scope43_block_count,scope43_ready,scope43_guard;
static uint8_t scope43_sentinel[0x50],scope43_table[1024];
/* Caller-scoped C-wrapper / existing SDK-allocator bridge. No guest mutation. */
static ff_scope43_call allocator44;
static uint32_t bridge44[64];
static uint8_t payload44_before[0x40000];
enum { S44_WORDS=64+64+80+358 };
static int sdk_scope44(void);
static int sdk_allocator_scope(void);
static uint32_t sdk_scope44_word(uint32_t);
static void s44_pointer_post(ff_sdk_scalar*);
static void s44_trace(uint32_t,uint32_t);
static void s44_esp(const char*,const char*);
static void sdk_shared_remaining_budget(uint32_t,uint32_t);
extern void ff_sdk_allocator_c44(void);
void ff_title_0004BF10(void);
void ff_sdk_shared_c_4c480(ff_sdk_scalar*);
uint32_t ff_probe_sdk_scope44_active(void);
uint32_t ff_probe_sdk_scope44_allocator_begin(ff_sdk_scalar*);
void ff_probe_sdk_scope44_allocator_post(ff_sdk_scalar*);
void ff_probe_sdk_scope44_cpp_complete(void);


static int sdk_scope43(void);
static uint32_t sdk_scope43_word(uint32_t);
static ff_scope43_call *s43_call(void);
static uint32_t s43_request(void);
static uint32_t s43_total(void);
static uint32_t s43_baseline(uint32_t);
static void s43_memory(uint32_t,uint32_t,uint32_t,ff_sdk_scalar*);
static void s43_enter(uint32_t,ff_sdk_scalar*);
static void s43_pre_ret(uint32_t,uint32_t,ff_sdk_scalar*);
static void s43_call_enter(ff_sdk_scalar*);
static void s43_call_exit(ff_sdk_scalar*);
static void s43_release_enter(ff_sdk_scalar*);
static void s43_pointer_post(ff_sdk_scalar*);
static void s43_continuation(ff_sdk_scalar*);
static void sdk_release_post(ff_sdk_scalar*);
uint32_t ff_probe_sdk_scope43_active(void);
uint32_t ff_probe_sdk_scope43_begin(uint32_t,ff_sdk_scalar*);
uint32_t ff_probe_sdk_scope43_allocator_begin(ff_sdk_scalar*);
void ff_probe_sdk_scope43_allocator_post(ff_sdk_scalar*);
void ff_probe_sdk_scope43_return_post(uint32_t,ff_sdk_scalar*);
void ff_probe_sdk_scope43_raw_pre_ret(uint32_t,ff_sdk_scalar*);
void ff_probe_sdk_scope43_hash_post(ff_sdk_scalar*);


enum { W_STAGE=0,W_OUT,W_OUT0,W_OUT1,W_AFTER0,W_AFTER1,W_KERNEL_CALLS,
       W_KERNEL_SP,W_KERNEL_RA,W_HANDLE,W_MODE,W_ALERT,W_TIMEOUT,W_OBJECT,
       W_OWNER_BEFORE,W_OWNER_AFTER,W_EAX_BEFORE,W_EAX_AFTER,W_KERNEL_SP_AFTER,
       W_GLOBAL,W_ATTEMPTS_BEFORE,W_COMPLETED_BEFORE,W_ATTEMPTS_AFTER,W_COMPLETED_AFTER,
       W_POST_COUNT,W_POST_SP,W_POST_TAG,W_POST_EAX,W_POST_EBX,W_POST_ESI,W_POST_EDI,
       W_POST_EBP,W_POST_DF,W_POST_SEH,W_HANDLE_COUNT,W_OBJECT_COUNT,W_FS };
/* MODE14 0x1E893C / ordinal-156 telemetry, words 276..314. */
static uint32_t tick_words[39], tick_extra_words[15], tick_enabled, tick_raw_slot, tick_initial_cell;
static uint32_t tick_stage, tick_slot_reads, tick_cell_reads, tick_refreshing, tick_prewrite_reads, tick_entry_recheck_reads, tick_entry_frame_reads, tick_exit_frame_reads;
static uint32_t tick_entry_esp, tick_entry_eflags, tick_entry_seh;
#define SDK_TICK_SLOT 0x002534DCu
#define SDK_TICK_CELL 0x03F11000u
#define SDK_TICK_RAW 0x8000009Cu
#define SDK_TICK_OWNER_START 0x03C00000u
#define SDK_TICK_OWNER_END 0x04000000u
enum {
    TICK_ATTEMPTS=0,TICK_PRE_RET_VALIDATED,TICK_ENTRY_RET,TICK_ENTRY_ESP,TICK_ENTRY_EAX,
    TICK_ENTRY_EBX,TICK_ENTRY_ESI,TICK_ENTRY_EDI,TICK_ENTRY_EBP,TICK_ENTRY_DF,
    TICK_ENTRY_EFLAGS,TICK_ENTRY_SEH,TICK_SLOT_READS,TICK_POINTER,TICK_CELL_READS,
    TICK_REFRESHES,TICK_CLOCK_LO,TICK_CLOCK_HI,TICK_MILLISECONDS,TICK_CELL_INITIAL,TICK_CELL_UPDATED,
    TICK_CELL_PERMISSIONS,TICK_OWNER_START,TICK_OWNER_END,TICK_DISK_VALUE,
    TICK_LAUNCH_SLOT_VALUE,TICK_LAUNCH_CELL_VALUE,TICK_EXIT_EAX,TICK_EXIT_ESP,
    TICK_EXIT_RET,TICK_EXIT_EBX,TICK_EXIT_ESI,TICK_EXIT_EDI,TICK_EXIT_EBP,TICK_EXIT_DF,
    TICK_EXIT_EFLAGS,TICK_EXIT_SEH,TICK_PRE_RET_ATTEMPTS,TICK_IMPORT_RAW
};
enum { TICK_ENTRY_SLOT_VALUE=0,TICK_ENTRY_CELL_VALUE,TICK_ENTRY_RECHECKS,
       TICK_PREWRITE_SLOT_VALUE,TICK_PREWRITE_CELL_VALUE,TICK_PREWRITE_RECHECKS,
       TICK_OWNER_ID,TICK_PAGE_COMMITTED,TICK_PAGE_PERMISSION,TICK_REGION_KIND,
       TICK_REGION_BASE,TICK_REGION_BYTES,TICK_REGION_CELL_END,TICK_PREWRITE_ACTIVE,
       TICK_FINAL_REFRESH_ACTIVE };
static uint32_t getter_completed, getter_active, getter_active_id;
extern void ff_title_0004B230(void);
static uint32_t setup_attempts,setup_unexpected_returns;
static uint32_t leaf_attempts,leaf_completed,init_attempts,init_completed;
static uint32_t launch_data_enabled,launch_data_original,launch_data_disk;
#define SDK_LAUNCH_DATA_SLOT 0x00253368u
#define SDK_LAUNCH_DATA_CELL 0x03f10004u
static uint32_t sdk_launch_data_profile(uint32_t enable);
static uint32_t sdk_tick_validate(void);
static uint32_t sdk_tick_cell_mapping(uint32_t);
static uint32_t sdk_tick_profile(uint32_t enable);
static uint32_t sdk_peek(const ff_timer_queue *, uint32_t, ff_dpc_call *, uint32_t *);
static void sdk_handoff_30943(void);
#define ff_dpc_peek sdk_peek
#define ff_probe_reset ff_probe_legacy_reset

extern void ff_sdk_tls_getter_c(void);
extern void ff_sdk_dosmaperr_c(void);
uint32_t ff_probe_sdk_dosmap_precheck(void);
uint32_t ff_probe_sdk_dosmap_begin(void);
uint32_t ff_probe_sdk_dosmap_child(uint32_t,const ff_sdk_scalar *);
void ff_probe_sdk_dosmap_child_return(const ff_sdk_scalar *);
void ff_probe_sdk_dosmap_complete(const ff_sdk_scalar *);
void ff_probe_sdk_dosmap_continuation(uint32_t);
#define FF_PROBE_STATUS_ERROR_PROVIDER(r,m) sdk_denied_map((r),(m))
#define FF_PROBE_STATUS_ERROR_RETURN(r) sdk_denied_return((r))
#define FF_PROBE_C_TRACE_EVENT(k,a) sdk_denied_trace((k),(a))
#define FF_PROBE_C_ESP_EVENT(n,t) sdk_denied_esp((n),(t))
#define FF_PROBE_CREATE_FILE_PROVIDER(a,s) sdk_disc_provider((a),(s))
#define FF_PROBE_CREATE_FILE_RETURN(row) sdk_disc_return((row))
#define ff_boot_enter_critical sdk_stdio_enter
#define ff_boot_leave_critical sdk_stdio_leave
#include "/mnt/c/Codex/Projects/Fuzion Frenzy Web Port/tools/fixtures/thread_startup_probe.c"
#undef FF_PROBE_STATUS_ERROR_PROVIDER
#undef FF_PROBE_STATUS_ERROR_RETURN
#undef FF_PROBE_C_TRACE_EVENT
#undef FF_PROBE_C_ESP_EVENT
#undef FF_PROBE_CREATE_FILE_PROVIDER
#undef FF_PROBE_CREATE_FILE_RETURN
#undef ff_boot_enter_critical
#undef ff_boot_leave_critical
#undef ff_probe_reset
#undef ff_dpc_peek
#undef eax
#undef ecx
#undef edx
#undef ebx
#undef esp
#undef esi
#undef edi

static uint32_t sdk_peek(const ff_timer_queue *t, uint32_t irql, ff_dpc_call *call, uint32_t *present)
{
    ff_dpc_call candidate;
    uint32_t available = 0;
    uint32_t status = ff_dpc_peek(t, irql, &candidate, &available);
    if (status) return status;
    if (sdk_active && available) return FF_BOOT_UNSUPPORTED;
    if (available) *call = candidate;
    *present = available;
    return status;
}
static ff_boot_thread *sdk_thread(void)
{
    if (!sdk_active || !sdk_current_target || !vm_ready || virtual_memory.ram != guest_ram || boot.ram != guest_ram ||
        boot.count > FF_BOOT_MAX_THREADS || boot.current >= boot.count || boot.current >= FF_BOOT_MAX_THREADS)
        stop(9, 0, 234);
    ff_boot_thread *t = &boot.threads[boot.current];
    if (t->state != FF_BOOT_RUNNING || t->id != boot.current + 1u) stop(9, t->id, 234);
    return t;
}
static int sdk_scope42(void) { return (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u))) && a42[A42_ACTIVE]==1u; }
static ff_sdk_invocation_evidence *sdk_evidence(void) {
 if(sdk_scope44())return &allocator44.evidence;
 return sdk_scope43()?&s43_call()->evidence:(sdk_scope42()?&allocator42_evidence:&invocation_evidence[sdk_invocation]);
}
static uint32_t sdk_scope42_word(uint32_t index) {
 if(index<96u)return a42[index];
 if(index>=96u && index<106u) {
  const uint32_t v[10]={scope42_allocator.entry.eax,scope42_allocator.entry.ecx,
   scope42_allocator.entry.edx,scope42_allocator.entry.ebx,scope42_allocator.entry.esp,
   scope42_allocator.entry.ebp,scope42_allocator.entry.esi,scope42_allocator.entry.edi,
   scope42_allocator.entry.fs_base,scope42_allocator.entry.df};return v[index-96u];
 }
 if(index>=106u && index<116u) {
  const uint32_t v[10]={scope42_allocator.post.eax,scope42_allocator.post.ecx,
   scope42_allocator.post.edx,scope42_allocator.post.ebx,scope42_allocator.post.esp,
   scope42_allocator.post.ebp,scope42_allocator.post.esi,scope42_allocator.post.edi,
   scope42_allocator.post.fs_base,scope42_allocator.post.df};return v[index-106u];
 }
 if(index>=116u) {
  const ff_sdk_invocation_evidence *v=&allocator42_evidence;uint32_t j=index-116u;
  if(j<24u)return v->prefix[j];j-=24u;
  if(j<24u)return v->hash[j];j-=24u;
  if(j<32u)return v->call[j];j-=32u;
  if(j<72u)return v->wait[j/24u][j%24u];j-=72u;
  if(j<40u)return v->extra[j];j-=40u;
  if(j<6u)return v->kernel_regs[j];j-=6u;
  if(j<48u)return v->node[j];j-=48u;
  if(j<64u)return v->release[j];j-=64u;
  if(j<48u)return v->continuation[j];
 }
 return 0u;
}
static uint32_t sdk_scope42_begin_allocator(ff_sdk_scalar *s) {
 if(!s || (sdk_enabled!=42u && (sdk_enabled!=43u && (sdk_enabled!=44u && sdk_enabled!=45u))) || sdk_current_target!=0x4f5e0u ||
    !ff_probe_sdk_scope41_active() || a42[A42_ACTIVE] || a42[A42_STAGE]) return 0u;
 if(scope41_words[S41_STAGE]!=4u || scope41_words[S41_CHILD_STAGE]!=2u ||
    s->esp!=0x03c10e38u || (s->esp&3u) || g_seh_ebp!=scope42_root_seh) return 0u;
 const uint32_t *f=scope41_read(s->esp,28u,s);
 if(f[0]!=0x4d539u || f[1]!=0x50u || f[2]!=0x25bfa8u || f[3]!=0x5bu ||
    f[4]!=0x2001u || f[5]!=0x25bfccu || f[6]!=0u) return 0u;
 if(!local_mutant_ready || !local_mutant_wait_enabled || !local_mutant_release_enabled ||
    sdk_wait_object(0x8000cu,0u)!=3u)stop(9,s->esp,0x4bf10u);
 const uint32_t wa=local_mutant_wait_attempts,wc=local_mutant_wait_completed;
 const uint32_t ra=local_mutant_release_attempts,rc=local_mutant_release_completed;
 memset(&allocator42_evidence,0,sizeof allocator42_evidence);
 memset(&scope42_allocator,0,sizeof scope42_allocator);
 a42[A42_ACTIVE]=1u; a42[A42_STAGE]=1u; a42[A42_ROOT]=0x4f5e0u;
 a42[A42_CALLER]=0x4d4f0u; a42[A42_SITE]=0x4d534u; a42[A42_CALLEE]=0x4bf10u;
 a42[A42_RA]=f[0]; a42[A42_ESP]=s->esp;
 a42[A42_WAIT_ATTEMPTS]=wa;a42[A42_WAIT_COMPLETED]=wc;
 a42[A42_RELEASE_ATTEMPTS]=ra;a42[A42_RELEASE_COMPLETED]=rc;
 for(uint32_t i=0;i<6u;i++)a42[A42_ARG0+i]=f[i+1u];
 scope42_allocator.entry=*s;
 /* Bind old checked-provider records to this separate invocation bank. */
 prefix_words[0]=1u;prefix_words[2]=scope41_words[S41_ENTRY_ESP];
 prefix_words[3]=scope41_words[S41_RETURN];prefix_words[4]=scope41_words[S41_ARG0];
 prefix_words[7]=scope41_words[S41_EBX];prefix_words[8]=scope41_words[S41_ESI];
 prefix_words[9]=scope41_words[S41_EDI];prefix_words[10]=scope41_words[S41_EBP];
 prefix_words[12]=scope42_root_seh;
 hash_words[1]=1u;hash_words[15]=scope41_words[S41_CHILD_EAX];
 a42[A42_STAGE]=2u;sdk_scope42_capture_old(s);return 1u;
}
uint32_t ff_probe_sdk_scope42_begin_allocator(ff_sdk_scalar *s)
{ return sdk_scope42_begin_allocator(s); }
static void sdk_scope42_allocator_post(ff_sdk_scalar *s) {
 if(!s || !sdk_scope42() || a42[A42_STAGE]!=2u || s->esp!=a42[A42_ESP]+4u ||
    s->df!=scope42_allocator.entry.df || s->fs_base!=scope42_allocator.entry.fs_base)
   stop(9,s?s->esp:0,0x4bf10u);
 if(s->ebx!=call_words[19] || s->esi!=call_words[20] || s->edi!=call_words[21] ||
    s->ebp!=call_words[22] || s->eax!=call_words[18] || s->fs_base!=scope42_allocator.entry.fs_base)
   stop(9,s->esp,0x4bf10u);
 a42[A42_ALLOC_POST]=1u; a42[A42_ALLOC_EAX]=s->eax;
 const ff_sdk_scalar actual=*s;scope42_allocator.post=actual;
 ff_sdk_shared_write(s);recomp_trace_exit("sdk-shared-post-ret",0x4bf10u);
 ff_sdk_scalar observed={0};ff_sdk_shared_read(&observed);
 if(!scope41_same_scalar(&actual,&observed))stop(9,s->esp,0x4bf10u);
 a42[A42_POST_TRACE]=1u;a42[A42_STAGE]=3u;
}
void ff_probe_sdk_scope42_allocator_post(ff_sdk_scalar *s)
{ sdk_scope42_allocator_post(s); }
void ff_probe_sdk_scope42_parent_post(ff_sdk_scalar *s) {
 if(!sdk_scope42())return;
 if(!s || a42[A42_STAGE]!=3u || !continuation_words[0] ||
    s->esp!=scope41_words[S41_ENTRY_ESP]+4u || s->eax!=1u ||
    s->ebx!=scope41_words[S41_EBX] || s->esi!=scope41_words[S41_ESI] ||
    s->edi!=scope41_words[S41_EDI] || s->ebp!=scope41_words[S41_EBP] ||
    s->df!=scope41_words[S41_DF] || s->fs_base!=scope41_words[S41_FS] || g_seh_ebp!=scope42_root_seh)
   stop(9,s?s->esp:0,0x4d4f0u);
 sdk_scope42_final_list(s);
 if(!a42[A42_FINAL_LIST])stop(9,s->esp,0x4d4f0u);
 a42[A42_PARENT_POST]=1u;a42[A42_STAGE]=4u;a42[A42_ACTIVE]=0u;
}
static uint32_t sdk_scope42_wait_baseline(uint32_t slot) {
 if(slot==0u)return a42[A42_WAIT_ATTEMPTS];
 if(slot==1u)return a42[A42_WAIT_COMPLETED];
 if(slot==2u)return a42[A42_RELEASE_ATTEMPTS];
 if(slot==3u)return a42[A42_RELEASE_COMPLETED];
 return UINT32_MAX;
}
static uint32_t scope42_fnv(const uint8_t *p,uint32_t n) {
 uint32_t h=2166136261u;for(uint32_t i=0;i<n;++i)h=(h^p[i])*16777619u;return h;
}
static void scope42_put32(uint8_t *p,uint32_t off,uint32_t v) {
 p[off]=(uint8_t)v;p[off+1u]=(uint8_t)(v>>8);p[off+2u]=(uint8_t)(v>>16);p[off+3u]=(uint8_t)(v>>24);
}
static void sdk_scope42_capture_old(ff_sdk_scalar *s) {
 const uint32_t sentinel=0x2d02d8u,old0=0x6f4030u,old1=0x6f40b0u;
 scope41_validate_list(s);
 if(*(const uint32_t *)scope41_read(0x2d02fcu,4u,s)!=old0 ||
    *(const uint32_t *)scope41_read(0x2d0300u,4u,s)!=old1)stop(9,0x2d02fcu,0x4bf10u);
 const uint8_t *n0=(const uint8_t *)scope41_read(old0,0x50u,s);
 const uint8_t *n1=(const uint8_t *)scope41_read(old1,0x50u,s);
 const uint8_t *sent=(const uint8_t *)scope41_read(sentinel,0x50u,s);
 memcpy(scope42_allocator.old_nodes[0],n0,0x50u);memcpy(scope42_allocator.old_nodes[1],n1,0x50u);
 memcpy(scope42_allocator.sentinel_before,sent,0x50u);
 const uint8_t *input=(const uint8_t *)scope41_read(scope41_words[S41_ARG0],15u,s);
 const uint8_t *table=(const uint8_t *)scope41_read(0x2d0330u,1024u,s);
 for(uint32_t i=0;i<14u;++i)if(input[i]==0u)stop(9,scope41_words[S41_ARG0],i);
 if(input[14]!=0u)stop(9,scope41_words[S41_ARG0],15u);
 memcpy(scope42_allocator.input,input,15u);
 memcpy(scope42_allocator.table,table,1024u);
 a42[A42_OLD_HASH0]=scope42_fnv(n0,0x50u);a42[A42_OLD_HASH1]=scope42_fnv(n1,0x50u);
 a42[A42_SENTINEL_HASH]=scope42_fnv(sent,0x50u);
 a42[A42_INPUT_HASH]=scope42_fnv(input,15u);a42[A42_TABLE_HASH]=scope42_fnv(table,1024u);
}
static void sdk_scope42_pointer_proof(ff_sdk_scalar *s) {
 const uint32_t header=node_words[8],q=call_words[18];
 if(!s || !sdk_scope42() || a42[A42_STAGE]!=3u || release_words[63]!=4u || call_words[9]!=0u ||
    virtual_memory.bytes<0x80u || !header || header>virtual_memory.bytes-0x80u || (header&3u) ||
    node_words[9]!=0x80u || q!=header+0x30u || q<header ||
    ff_vm_check(&virtual_memory,header,0x80u,FF_VM_READ|FF_VM_WRITE) ||
    ff_vm_check(&virtual_memory,q,0x50u,FF_VM_READ|FF_VM_WRITE) ||
    node_words[23]==header || node_words[23]==q)stop(9,q,0x4d539u);
 const ff_boot_thread *t=sdk_thread();
 if((header<0x6f4080u && 0x6f4000u<header+0x80u) ||
    (header<0x6f4100u && 0x6f4080u<header+0x80u) ||
    (header<0x2d0328u && 0x2d02d8u<header+0x80u) ||
    (header<0x2d0730u && 0x2d0330u<header+0x80u) ||
    (header<scope41_words[S41_ARG0]+15u && scope41_words[S41_ARG0]<header+0x80u) ||
    (header<t->stack_top && t->stack_low<header+0x80u))stop(9,header,0x80u);
 const uint32_t *h=(const uint32_t *)scope41_read(header,0x30u,s);
 if(h[1]!=0x80u || *(const uint16_t *)scope41_read(header+8u,2u,s)!=1u || h[11]!=0xdeadbeefu)
   stop(9,header,h[1]);
 const uint8_t *payload=(const uint8_t *)scope41_read(q,0x50u,s);
 if(memcmp(scope42_allocator.old_nodes[0],scope41_read(0x6f4030u,0x50u,s),0x50u) ||
    memcmp(scope42_allocator.old_nodes[1],scope41_read(0x6f40b0u,0x50u,s),0x50u) ||
    memcmp(scope42_allocator.sentinel_before,scope41_read(0x2d02d8u,0x50u,s),0x50u) ||
    memcmp(scope42_allocator.table,scope41_read(0x2d0330u,1024u,s),1024u) ||
    memcmp(scope42_allocator.input,scope41_read(scope41_words[S41_ARG0],15u,s),15u) ||
    *(const uint32_t *)scope41_read(0x2d02fcu,4u,s)!=0x6f4030u ||
    *(const uint32_t *)scope41_read(0x2d0300u,4u,s)!=0x6f40b0u ||
    !scope41_allocated_node(0x6f4030u,0x582664edu,s) ||
    !scope41_allocated_node(0x6f40b0u,0x1eea8d9du,s))stop(9,q,0x4d539u);
 memcpy(scope42_allocator.fresh_before,payload,0x50u);
 a42[A42_FRESH_HASH]=scope42_fnv(payload,0x50u);a42[A42_POINTER_READY]=1u;
}
static void sdk_scope42_prewrite(ff_sdk_scalar *s) {
 const uint32_t q=call_words[18],tail=0x6f40b0u,sentinel=0x2d02d8u;
 if(!s || a42[A42_STAGE]!=3u || !a42[A42_POINTER_READY] || a42[A42_WRITE_INDEX]!=2u ||
    s->esp!=scope41_words[S41_ENTRY_ESP]-8u || s->eax!=q || s->ecx!=tail || s->edx!=tail ||
    s->ebx!=scope41_words[S41_CHILD_EAX] || s->esi!=scope41_words[S41_ESI] ||
    s->edi!=scope41_words[S41_ARG0] || s->ebp!=scope41_words[S41_EBP] ||
    s->df!=scope41_words[S41_DF] || s->fs_base!=scope41_words[S41_FS] || g_seh_ebp!=scope42_root_seh)
   stop(9,s?s->esp:0,0x4d555u);
 if(memcmp(scope42_allocator.old_nodes[0],scope41_read(0x6f4030u,0x50u,s),0x50u) ||
    memcmp(scope42_allocator.old_nodes[1],scope41_read(tail,0x50u,s),0x50u) ||
    memcmp(scope42_allocator.sentinel_before,scope41_read(sentinel,0x50u,s),0x50u) ||
    memcmp(scope42_allocator.table,scope41_read(0x2d0330u,1024u,s),1024u) ||
    memcmp(scope42_allocator.input,scope41_read(scope41_words[S41_ARG0],15u,s),15u))stop(9,q,0x4d555u);
 if(scope42_fnv(scope42_allocator.input,15u)!=a42[A42_INPUT_HASH] ||
    scope42_fnv(scope42_allocator.table,1024u)!=a42[A42_TABLE_HASH] ||
    scope42_allocator.input[14]!=0u)stop(9,q,0x4d555u);
 if(!scope41_allocated_node(0x6f4030u,0x582664edu,s) ||
    !scope41_allocated_node(tail,0x1eea8d9du,s))stop(9,q,0x4d555u);
 const uint32_t *oldtail=(const uint32_t *)scope41_read(tail,0x50u,s);
 const uint32_t *sent=(const uint32_t *)scope41_read(sentinel,0x50u,s);
 const uint32_t *fresh=(const uint32_t *)scope41_read(q,0x50u,s);
 if(oldtail[9]!=sentinel || sent[9]!=0x6f4030u || sent[10]!=tail || fresh[9]!=sentinel || fresh[10]!=tail ||
    ff_vm_check(&virtual_memory,tail+0x24u,4u,FF_VM_WRITE) ||
    ff_vm_check(&virtual_memory,sentinel+0x28u,4u,FF_VM_WRITE))stop(9,q,0x4d555u);
 uint8_t expected[0x50];memcpy(expected,scope42_allocator.fresh_before,sizeof expected);
 scope42_put32(expected,0x24u,sentinel);scope42_put32(expected,0x28u,tail);
 if(memcmp(expected,fresh,sizeof expected))stop(9,q,0x4d555u);
 a42[A42_CONT]=1u;
}
static void sdk_scope42_write_check(uint32_t address,uint32_t bytes,uint32_t access,ff_sdk_scalar *s) {
 const uint32_t q=call_words[18],tail=0x6f40b0u,sentinel=0x2d02d8u;
 const uint32_t i=a42[A42_WRITE_INDEX];
 if(!sdk_scope42() || !s || !(access&FF_VM_WRITE) || !a42[A42_POINTER_READY])return;
 if(i==0u){if(address!=q+0x28u || bytes!=4u || s->eax!=q || s->ecx!=tail)stop(9,address,i);}
 else if(i==1u){if(address!=q+0x24u || bytes!=4u || s->eax!=q)stop(9,address,i);}
 else if(i==2u){if(!a42[A42_CONT] || address!=tail+0x24u || bytes!=4u || s->eax!=q || s->edx!=tail)stop(9,address,i);}
 else if(i==3u){if(!a42[A42_CONT] || address!=scope41_words[S41_ENTRY_ESP]-12u || s->esp!=address || bytes!=4u || s->esi!=scope41_words[S41_ESI] || ff_vm_check(&virtual_memory,address,4u,FF_VM_WRITE))stop(9,address,i);}
 else if(i==4u){if(!a42[A42_CONT] || address!=sentinel+0x28u || bytes!=4u || s->eax!=q || s->ecx!=sentinel)stop(9,address,i);}
 else if(i>=5u && i<=19u){const uint32_t k=i-5u;const uint8_t *live=(const uint8_t *)scope41_read(scope41_words[S41_ARG0]+k,1u,s);if(address!=q+k || bytes!=1u || (uint8_t)s->edx!=*live || *live!=scope42_allocator.input[k])stop(9,address,i);}
 else if(i==20u){if(address!=q+0x30u || bytes!=4u || s->ecx!=q+0x2cu)stop(9,address,i);}
 else if(i==21u){if(address!=q+0x2cu || bytes!=4u || s->ecx!=q+0x2cu)stop(9,address,i);}
 else if(i==22u){if(address!=q+0x20u || bytes!=4u || s->ebx!=scope41_words[S41_CHILD_EAX])stop(9,address,i);}
 else if(i==23u){if(address!=q+0x4cu || bytes!=4u)stop(9,address,i);}
 else stop(9,address,i);
 ++a42[A42_WRITE_INDEX];
}
static void sdk_scope42_final_list(ff_sdk_scalar *s) {
 const uint32_t q=call_words[18],old0=0x6f4030u,old1=0x6f40b0u,sentinel=0x2d02d8u;
 if(a42[A42_WRITE_INDEX]!=24u || !a42[A42_CONT])stop(9,q,a42[A42_WRITE_INDEX]);
 uint8_t expected0[0x50],expected1[0x50],expectedSent[0x50],expectedQ[0x50];
 memcpy(expected0,scope42_allocator.old_nodes[0],0x50u);
 memcpy(expected1,scope42_allocator.old_nodes[1],0x50u);
 memcpy(expectedSent,scope42_allocator.sentinel_before,0x50u);
 memcpy(expectedQ,scope42_allocator.fresh_before,0x50u);
 scope42_put32(expected1,0x24u,q);scope42_put32(expectedSent,0x28u,q);
 memcpy(expectedQ,scope42_allocator.input,15u);
 scope42_put32(expectedQ,0x20u,scope41_words[S41_CHILD_EAX]);
 scope42_put32(expectedQ,0x24u,sentinel);scope42_put32(expectedQ,0x28u,old1);
 scope42_put32(expectedQ,0x2cu,q+0x2cu);scope42_put32(expectedQ,0x30u,q+0x2cu);
 scope42_put32(expectedQ,0x4cu,0u);
 if(memcmp(expected0,scope41_read(old0,0x50u,s),0x50u) ||
    memcmp(expected1,scope41_read(old1,0x50u,s),0x50u) ||
    memcmp(expectedSent,scope41_read(sentinel,0x50u,s),0x50u) ||
    memcmp(expectedQ,scope41_read(q,0x50u,s),0x50u))stop(9,q,0x4f639u);
 if(*(const uint32_t *)scope41_read(0x2d02fcu,4u,s)!=old0 ||
    *(const uint32_t *)scope41_read(0x2d0300u,4u,s)!=q)stop(9,q,0x4f639u);
 const uint32_t *n0=(const uint32_t *)scope41_read(old0,0x50u,s);
 const uint32_t *n1=(const uint32_t *)scope41_read(old1,0x50u,s);
 const uint32_t *nq=(const uint32_t *)scope41_read(q,0x50u,s);
 if(n0[9]!=old1 || n0[10]!=sentinel || n1[9]!=q || n1[10]!=old0 ||
    nq[9]!=sentinel || nq[10]!=old1 || nq[8]!=scope41_words[S41_CHILD_EAX] ||
    !scope41_allocated_node(old0,n0[8],s) || !scope41_allocated_node(old1,n1[8],s))stop(9,q,0x4f639u);
 a42[A42_FINAL_LIST]=1u;
}

static int sdk_scope43(void) {
 return (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u)) && scope43_count && scope43_current<scope43_count &&
        scope43_current<4u && scope43_calls[scope43_current].active;
}
static ff_scope43_call *s43_call(void) {
 if(sdk_scope44())return &allocator44;
 if(!sdk_scope43())stop(9,scope43_current,43u);
 return &scope43_calls[scope43_current];
}
uint32_t ff_probe_sdk_scope43_active(void) { return sdk_scope43()?1u:0u; }
static uint32_t s43_request(void) {if(sdk_scope44())return 0x40000u;return s43_call()->kind==0x4d620u?0x420u:0x50u;}
static uint32_t s43_total(void) {return s43_request()+0x30u;}
static uint32_t s43_u32(const uint8_t *p) {
 return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);
}
static const uint8_t *s43_read(uint32_t a,uint32_t n,ff_sdk_scalar *s) {
 return (const uint8_t*)scope41_read(a,n,s);
}
static const uint32_t *s43_frame(ff_sdk_scalar *s,uint32_t n,uint32_t tag) {
 if(!s)stop(9,0u,tag);
 const ff_boot_thread *t=sdk_thread();
 if((s->esp&3u)||s->esp<t->stack_low||s->esp>t->stack_top||n>t->stack_top-s->esp)
  stop(9,s->esp,tag);
 return (const uint32_t*)s43_read(s->esp,n,s);
}
static uint32_t s43_baseline(uint32_t n) {
 ff_scope43_call *c=s43_call();
 const uint32_t v[4]={c->wait_a,c->wait_c,c->release_a,c->release_c};
 if(n>=4u)stop(9,n,43u);return v[n];
}
static int s43_nonvolatile(const ff_sdk_scalar *a,const ff_sdk_scalar *b) {
 return a->ebx==b->ebx&&a->esi==b->esi&&a->edi==b->edi&&a->ebp==b->ebp&&
        a->fs_base==b->fs_base&&a->df==b->df;
}
static ff_scope43_block *s43_block(uint32_t q,uint32_t kind) {
 for(uint32_t i=0;i<scope43_block_count;i++)
  if(scope43_blocks[i].q==q&&scope43_blocks[i].kind==kind)return &scope43_blocks[i];
 return NULL;
}
static void s43_header(uint32_t h,uint32_t extent,ff_sdk_scalar *s) {
 if((h&3u)||!h||extent<0x30u||extent>virtual_memory.bytes||h>virtual_memory.bytes-extent||
    ff_vm_check(&virtual_memory,h,extent,FF_VM_READ|FF_VM_WRITE))stop(9,h,extent);
 const uint8_t *p=s43_read(h,0x30u,s);
 if(s43_u32(p+4u)!=extent||p[8]!=1u||p[9]!=0u||s43_u32(p+0x2cu)!=0xdeadbeefu)
  stop(9,h,extent);
}
static ff_scope43_block *s43_add(uint32_t kind,uint32_t q,uint32_t h,uint32_t extent,
                                uint32_t bytes,uint32_t owner,ff_sdk_scalar *s) {
 if(scope43_block_count>=6u||q!=h+0x30u||bytes>0x420u||bytes>extent-0x30u)
  stop(9,q,bytes);
 s43_header(h,extent,s);
 for(uint32_t i=0;i<scope43_block_count;i++) {
  const ff_scope43_block *b=&scope43_blocks[i];
  if(h<b->header+b->extent&&b->header<h+extent)stop(9,h,b->header);
 }
 ff_scope43_block *b=&scope43_blocks[scope43_block_count++];memset(b,0,sizeof *b);
 b->kind=kind;b->q=q;b->header=h;b->extent=extent;b->bytes=bytes;b->owner=owner;
 memcpy(b->expected,s43_read(q,bytes,s),bytes);return b;
}
static void s43_images(ff_sdk_scalar *s) {
 if(memcmp(scope43_sentinel,s43_read(0x2d02d8u,0x50u,s),0x50u)||
    memcmp(scope43_table,s43_read(0x2d0330u,1024u,s),1024u))stop(9,0x2d02d8u,43u);
 for(uint32_t i=0;i<scope43_block_count;i++) {
  const ff_scope43_block *b=&scope43_blocks[i];
  s43_header(b->header,b->extent,s);
  if(memcmp(b->expected,s43_read(b->q,b->bytes,s),b->bytes))stop(9,b->q,43u);
 }
}
static void s43_cache_links(ff_sdk_scalar *s) {
 uint32_t cur=s43_u32(scope43_sentinel+0x24u),prev=0x2d02d8u,seen[4]={0},n=0;
 while(cur!=0x2d02d8u) {
  if(n>=4u)stop(9,cur,43u);
  for(uint32_t i=0;i<n;i++)if(seen[i]==cur)stop(9,cur,43u);
  ff_scope43_block *b=s43_block(cur,1u);
  if(!b||s43_u32(b->expected+0x28u)!=prev)stop(9,cur,43u);
  seen[n++]=cur;prev=cur;cur=s43_u32(b->expected+0x24u);
 }
 uint32_t wanted=0;for(uint32_t i=0;i<scope43_block_count;i++)if(scope43_blocks[i].kind==1u)++wanted;
 if(n!=wanted||prev!=s43_u32(scope43_sentinel+0x28u))stop(9,n,wanted);
 s43_images(s);
}
static void s43_object_links(uint32_t node,ff_sdk_scalar *s) {
 ff_scope43_block *cache=s43_block(node,1u);if(!cache)stop(9,node,43u);
 const uint32_t l=node+0x2cu;
 uint32_t cur=s43_u32(cache->expected+0x2cu),prev=l,seen[2]={0},n=0;
 while(cur!=l) {
  if(n>=2u)stop(9,cur,43u);
  for(uint32_t i=0;i<n;i++)if(seen[i]==cur)stop(9,cur,43u);
  ff_scope43_block *b=s43_block(cur,2u);
  if(!b||b->owner!=node||s43_u32(b->expected+4u)!=prev||s43_u32(b->expected+0xcu)!=node)
   stop(9,cur,node);
  seen[n++]=cur;prev=cur;cur=s43_u32(b->expected);
 }
 if(prev!=s43_u32(cache->expected+0x30u)||n!=s43_u32(cache->expected+0x4cu))stop(9,node,n);
 s43_images(s);
}
static void s43_init(ff_sdk_scalar *s) {
 if(scope43_ready)return;
 if(a42[A42_ACTIVE]||a42[A42_STAGE]!=4u||!a42[A42_FINAL_LIST]||!a42[A42_PARENT_POST]||
    scope41_words[S41_ACTIVE]||scope43_block_count)stop(9,scope43_block_count,43u);
 for(uint32_t i=0;i<2u;i++) {
  const ff_sdk_invocation_evidence *e=&invocation_evidence[i];const uint32_t q=e->call[18];
  if(!scope41_allocated_node(q,e->call[10],s)||q<0x30u)stop(9,q,i);
  (void)s43_add(1u,q,q-0x30u,0x80u,0x50u,0u,s);
 }
 const uint32_t q=allocator42_evidence.call[18],h=allocator42_evidence.node[8];
 if(!q||q!=h+0x30u||allocator42_evidence.node[9]!=0x80u||
    allocator42_evidence.release[47]!=1u||allocator42_evidence.release[50]!=q)stop(9,q,42u);
 (void)s43_add(1u,q,h,0x80u,0x50u,0u,s);
 memcpy(scope43_sentinel,s43_read(0x2d02d8u,0x50u,s),0x50u);
 memcpy(scope43_table,s43_read(0x2d0330u,1024u,s),1024u);
 scope43_ready=1u;s43_cache_links(s);
}
static uint32_t s43_find_hash(uint32_t hash) {
 uint32_t cur=s43_u32(scope43_sentinel+0x24u),n=0;
 while(cur!=0x2d02d8u) {
  ff_scope43_block *b=s43_block(cur,1u);if(!b||n++>=4u)stop(9,cur,hash);
  if(s43_u32(b->expected+0x20u)==hash)return cur;
  cur=s43_u32(b->expected+0x24u);
 }
 return 0u;
}
static void s43_patch(uint32_t a,uint32_t bytes,uint32_t value) {
 uint8_t *p=NULL;
 if(a>=0x2d02d8u&&a<=0x2d0328u&&bytes<=0x2d0328u-a)p=scope43_sentinel+(a-0x2d02d8u);
 for(uint32_t i=0;i<scope43_block_count;i++) {
  ff_scope43_block *b=&scope43_blocks[i];
  if(a>=b->q&&a<=b->q+b->bytes&&bytes<=b->q+b->bytes-a) {
   if(p)stop(9,a,bytes);p=b->expected+(a-b->q);
  }
 }
 if(!p||(bytes!=1u&&bytes!=4u))stop(9,a,bytes);
 if(bytes==1u)*p=(uint8_t)value;else scope42_put32(p,0u,value);
}
uint32_t ff_probe_sdk_scope43_begin(uint32_t target,ff_sdk_scalar *s) {
 if(!s||(sdk_enabled!=43u && (sdk_enabled!=44u && sdk_enabled!=45u))||sdk_current_target!=0x4f5e0u||sdk_scope43())return 0u;
 const uint32_t kinds[3]={0x4d620u,0x4d4f0u,0x4d620u};
 const uint32_t esps[3]={0x03c10e50u,0x03c10e4cu,0x03c10e40u};
 const uint32_t ras[3]={0x4f647u,0x4f656u,0x4f664u};
 const uint32_t inputs[3]={0x25c4d0u,0x25c4c0u,0x25c4c0u};
 if(scope43_count>=3u||target!=kinds[scope43_count]||s->esp!=esps[scope43_count])return 0u;
 const uint32_t n=scope43_count;const uint32_t *f=s43_frame(s,target==0x4d620u?16u:8u,target);
 if(f[0]!=ras[n]||f[1]!=inputs[n]||(target==0x4d620u&&(f[2]!=4u||f[3])))return 0u;
 s43_init(s);s43_cache_links(s);
 ff_scope43_call *c=&scope43_calls[n];memset(c,0,sizeof *c);
 c->kind=target;c->ret=f[0];c->esp=s->esp;c->seh=g_seh_ebp;c->root_entry=*s;
 c->args[0]=f[1];if(target==0x4d620u){c->args[1]=f[2];c->args[2]=f[3];}
 c->input_span=n?16u:15u;memcpy(c->input,s43_read(c->args[0],c->input_span,s),c->input_span);
 for(uint32_t i=0;i+1u<c->input_span;i++)if(!c->input[i])stop(9,c->args[0],i);
 if(c->input[c->input_span-1u])stop(9,c->args[0],c->input_span);
 c->active=1u;c->stage=S43_ROOT;scope43_current=n;++scope43_count;
 prefix_words[0]=1u;prefix_words[2]=c->esp;prefix_words[3]=c->ret;prefix_words[4]=c->args[0];
 prefix_words[7]=s->ebx;prefix_words[8]=s->esi;prefix_words[9]=s->edi;prefix_words[10]=s->ebp;prefix_words[12]=c->seh;
 return 1u;
}
static void s43_enter(uint32_t guest,ff_sdk_scalar *s) {
 ff_scope43_call *c=s43_call();
 if(guest==c->kind) {
  if(c->entered||c->stage!=S43_ROOT||!scope41_same_scalar(&c->root_entry,s))stop(9,s->esp,guest);
  c->entered=1u;return;
 }
 if(guest==0x4dc30u) {
  const uint32_t *f=s43_frame(s,8u,guest);
  const uint32_t delta=c->kind==0x4d620u?12u:16u,ra=c->kind==0x4d620u?0x4d62bu:0x4d4fcu;
  if(!c->entered||c->hash_entered||c->stage!=S43_ROOT||s->esp!=c->esp-delta||f[0]!=ra||f[1]!=c->args[0])
   stop(9,s->esp,guest);
  c->hash_entered=1u;c->hash_entry=*s;c->hash_esp=s->esp;
  hash_words[0]=1u;hash_words[2]=s->esp;hash_words[3]=f[0];hash_words[4]=f[1];
  return;
 }
 stop(9,guest,43u);
}
void ff_probe_sdk_scope43_hash_post(ff_sdk_scalar *s) {
 ff_scope43_call *c=s43_call();
 if(!c->hash_exited||c->hash_done||s->esp!=c->hash_esp+4u||s->eax!=c->hash_pre.eax||
    !s43_nonvolatile(&c->hash_entry,s))stop(9,s->esp,0x4dc30u);
 c->hash_post=*s;c->hash_eax=s->eax;c->hash_done=1u;
 hash_words[1]=1u;hash_words[15]=s->eax;
 s43_images(s);c->matched=s43_find_hash(c->hash_eax);
 if(c->kind==0x4d620u&&c->matched)s43_object_links(c->matched,s);
 recomp_trace_exit("sdk-m43-post-ret",0x4dc30u);
}
uint32_t ff_probe_sdk_scope43_allocator_begin(ff_sdk_scalar *s) {
 if(!sdk_scope43()||!s)return 0u;
 ff_scope43_call *c=s43_call();const uint32_t r=s43_request();
 const uint32_t *f=s43_frame(s,28u,0x4bf10u);
 const uint32_t ra=c->kind==0x4d620u?0x4d67du:0x4d539u;
 if(c->stage!=S43_ROOT||!c->hash_done||c->alloc_seen||s->esp!=c->esp-36u||f[0]!=ra||
    f[1]!=r||f[2]!=0x25bfa8u||f[3]!=(c->kind==0x4d620u?0x9du:0x5bu)||
    f[4]!=0x2001u||f[5]!=(c->kind==0x4d620u?0x25bfecu:0x25bfccu)||f[6])return 0u;
 if((c->kind==0x4d620u&&(!c->matched||s->esi!=c->matched||s->edi!=c->args[1]||s->ebx!=c->root_entry.ebx))||
    (c->kind==0x4d4f0u&&(c->matched||s->edi!=c->args[0]||s->ebx!=c->hash_eax||s->esi!=c->root_entry.esi))||
    s->ebp!=c->root_entry.ebp||s->fs_base!=c->root_entry.fs_base||s->df!=c->root_entry.df||
    g_seh_ebp!=c->seh||!local_mutant_ready||!local_mutant_wait_enabled||!local_mutant_release_enabled||
    sdk_wait_object(0x8000cu,0u)!=3u)stop(9,s->esp,0x4bf10u);
 c->allocator_entry=*s;c->alloc_seen=1u;c->stage=S43_ALLOC;c->total=r+0x30u;
 c->wait_a=local_mutant_wait_attempts;c->wait_c=local_mutant_wait_completed;
 c->release_a=local_mutant_release_attempts;c->release_c=local_mutant_release_completed;
 s43_images(s);return 1u;
}
static void s43_call_enter(ff_sdk_scalar *s) {
 ff_scope43_call *c=s43_call();const uint32_t *f=s43_frame(s,28u,0x4bf10u);
 if(c->stage!=S43_ALLOC||call_words[0]||!scope41_same_scalar(s,&c->allocator_entry))stop(9,s->esp,0x4bf10u);
 call_words[0]=1u;call_words[2]=s->esp;call_words[3]=f[0];
 for(uint32_t i=0;i<6u;i++)call_words[4u+i]=f[1u+i];
 call_words[10]=s->ebx;call_words[11]=s->esi;call_words[12]=s->edi;call_words[13]=s->ebp;
 call_words[14]=s->df;call_words[15]=c->seh;call_words[25]=28u;
}
static void s43_call_exit(ff_sdk_scalar *s) {
 ff_scope43_call *c=s43_call();const uint32_t *f=s43_frame(s,28u,0x4bf10u);
 if(!call_words[0]||call_words[1]||c->stage!=S43_ALLOC||s->esp!=c->allocator_entry.esp||
    f[0]!=call_words[3]||!s43_nonvolatile(&c->allocator_entry,s)||g_seh_ebp!=c->seh)
  stop(9,s->esp,0x4bf10u);
 for(uint32_t i=0;i<6u;i++)if(f[1u+i]!=call_words[4u+i])stop(9,s->esp,i);
 call_words[1]=1u;call_words[16]=s->esp;call_words[17]=f[0];call_words[18]=s->eax;
 call_words[19]=s->ebx;call_words[20]=s->esi;call_words[21]=s->edi;call_words[22]=s->ebp;
 call_words[23]=s->df;call_words[24]=g_seh_ebp;
}
static void s43_selection(ff_sdk_scalar *s) {
 ff_scope43_call *c=s43_call();const uint32_t h=s->esi,t=s43_total();
 if(c->selected||wait_extra[W_STAGE]!=8u||wait_extra[W_POST_COUNT]!=1u||
    node_words[0]||release_words[0]||s->edi!=t||s->ebx!=s43_request()||
    !h||(h&3u)||h>virtual_memory.bytes-0x30u||
    sdk_wait_object(wait_frames[0][4],sdk_thread()->id)!=wait_extra[W_OBJECT])stop(9,h,0x4bfe5u);
 const uint8_t *head=s43_read(h,0x30u,s);const uint32_t size=s43_u32(head+4u);
 if(head[8]||head[9]||size<t||size>virtual_memory.bytes||h>virtual_memory.bytes-size||
    ff_vm_check(&virtual_memory,h,size,FF_VM_READ|FF_VM_WRITE))stop(9,h,size);
 for(uint32_t i=0;i<scope43_block_count;i++) {
  const ff_scope43_block *b=&scope43_blocks[i];
  if(h<b->header+b->extent&&b->header<h+size)stop(9,h,b->header);
 }
 c->selected=1u;c->selected_h=h;c->selected_size=size;c->split=size>t+0x30u;
}
static void s43_release_enter(ff_sdk_scalar *s) {
 ff_scope43_call *c=s43_call();const uint32_t *f=s43_frame(s,8u,0x3080du);
 if(c->stage!=S43_ALLOC||release_words[0]||release_words[63]||wait_extra[W_STAGE]!=8u||
    s->esp!=c->allocator_entry.esp-20u||s->ebp!=c->allocator_entry.ebp||s->fs_base!=c->allocator_entry.fs_base||
    s->df||g_seh_ebp!=c->seh||local_mutant_release_attempts!=c->release_a||local_mutant_release_completed!=c->release_c||
    (f[0]!=0x4c0dau&&f[0]!=0x4bfb8u))stop(9,s->esp,0x3080du);
 if(f[0]==0x4c0dau) {
  const uint32_t expected=c->split?c->total:c->selected_size;
  if(!c->selected||s->esi!=c->selected_h||s->edi!=c->total||
     (c->split?(!node_words[0]||!node_words[1]||node_words[24]!=1u||node_words[8]!=c->selected_h||node_words[9]!=c->total):
                (node_words[0]||node_words[1]||node_words[24])))stop(9,s->esi,0x4c0dau);
  s43_header(c->selected_h,expected,s);c->header=c->selected_h;c->extent=expected;
 } else if(c->selected||node_words[0]||s->esi!=0x2cce98u)stop(9,s->esi,0x4bfb8u);
 const uint32_t handle=s43_u32(s43_read(0x2cd794u,4u,s));
 if(f[1]!=handle||sdk_wait_object(handle,sdk_thread()->id)!=wait_extra[W_OBJECT])stop(9,handle,221u);
 c->release_ra=f[0];release_words[0]=1u;release_words[2]=s->esp;release_words[3]=f[0];release_words[4]=f[1];
 release_words[5]=s->eax;release_words[6]=s->ebx;release_words[7]=s->esi;release_words[8]=s->edi;
 release_words[9]=s->ebp;release_words[10]=s->df;release_words[11]=c->seh;release_words[17]=wait_extra[W_OBJECT];
 release_words[23]=c->release_a;release_words[24]=c->release_c;release_words[27]=handle;
 release_words[62]=s->fs_base;release_words[63]=1u;
}
static void s43_pointer_post(ff_sdk_scalar *s) {
 ff_scope43_call *c=s43_call();const uint32_t *args=s43_frame(s,24u,call_words[3]);
 if(c->stage!=S43_ALLOC||!call_words[1]||release_words[63]!=4u||release_words[47]||
    s->esp!=c->allocator_entry.esp+4u||s->eax!=call_words[18]||
    !s43_nonvolatile(&c->allocator_entry,s)||g_seh_ebp!=c->seh||
    local_mutant_wait_attempts!=c->wait_a+1u||local_mutant_wait_completed!=c->wait_c+1u||
    local_mutant_release_attempts!=c->release_a+1u||local_mutant_release_completed!=c->release_c+1u||
    sdk_wait_object(release_words[4],0u)!=release_words[17])stop(9,s->esp,call_words[3]);
 for(uint32_t i=0;i<6u;i++){if(args[i]!=call_words[4u+i])stop(9,s->esp,i);release_words[52u+i]=args[i];}
 c->allocator_post=*s;c->q=s->eax;
 if(c->q) {
  if(!c->selected||!c->header||c->q!=c->header+0x30u)stop(9,c->q,43u);
  const ff_boot_thread *thread=sdk_thread();
  if((c->header<thread->stack_top&&thread->stack_low<c->header+c->extent)||
     (c->header<0x2d0730u&&0x2d02d8u<c->header+c->extent)||
     (c->header<c->args[0]+c->input_span&&c->args[0]<c->header+c->extent)||
     (c->header<0x2cd800u&&0x2cce80u<c->header+c->extent))stop(9,c->header,c->extent);
  s43_images(s);
  c->new_block=scope43_block_count;
  (void)s43_add(c->kind==0x4d620u?2u:1u,c->q,c->header,c->extent,s43_request(),c->matched,s);
  c->pointer_ready=1u;
  c->old_tail=c->kind==0x4d620u?s43_u32(s43_block(c->matched,1u)->expected+0x30u):s43_u32(scope43_sentinel+0x28u);
  c->old_count=c->kind==0x4d620u?s43_u32(s43_block(c->matched,1u)->expected+0x4cu):0u;
 } else if(c->selected||c->release_ra!=0x4bfb8u)stop(9,c->q,43u);
 release_words[47]=1u;release_words[48]=s->esp;release_words[49]=call_words[3];release_words[50]=s->eax;
 release_words[51]=call_words[1];release_words[58]=s43_request();release_words[59]=6u;
 release_words[60]=0u;release_words[61]=24u;release_words[63]=5u;c->stage=S43_WRITES;
}
void ff_probe_sdk_scope43_allocator_post(ff_sdk_scalar *s) {
 ff_scope43_call *c=s43_call();if(c->pointer_ready||release_words[47])stop(9,s->esp,0x4bf10u);
 s43_pointer_post(s);ff_sdk_shared_write(s);recomp_trace_exit("sdk-shared-post-ret",0x4bf10u);
}
static void s43_continuation(ff_sdk_scalar *s) {
 ff_scope43_call *c=s43_call();const uint32_t *f=s43_frame(s,16u,0x4d555u);
 if(c->kind!=0x4d4f0u||c->stage!=S43_WRITES||!c->pointer_ready||c->write_index!=2u||c->continuation||
    s->esp!=c->esp-8u||s->eax!=c->q||s->ebx!=c->hash_eax||s->esi!=c->root_entry.esi||
    s->edi!=c->args[0]||s->ebp!=c->root_entry.ebp||s->fs_base!=c->root_entry.fs_base||
    s->df||g_seh_ebp!=c->seh||s->edx!=c->old_tail||s->ecx!=c->old_tail||
    f[0]!=c->root_entry.edi||f[1]!=c->root_entry.ebx||f[2]!=c->ret||f[3]!=c->args[0])
  stop(9,s->esp,0x4d555u);
 s43_images(s);c->continuation=1u;
 continuation_words[0]=1u;continuation_words[3]=1u;continuation_words[4]=s->esp;
 for(uint32_t i=0;i<4u;i++)continuation_words[5u+i]=f[i];
}
static void s43_store(uint32_t a,uint32_t bytes,ff_sdk_scalar *s) {
 ff_scope43_call *c=s43_call();const uint32_t i=c->write_index,q=c->q;
 uint32_t wanted=0,value=0,size=4u,stack=0u;
 if(c->stage!=S43_WRITES||!c->pointer_ready)stop(9,a,i);
 s43_images(s);
 if(memcmp(c->input,s43_read(c->args[0],c->input_span,s),c->input_span))stop(9,c->args[0],i);
 if(c->kind==0x4d620u) {
  const uint32_t n=c->matched,l=n+0x2cu;
  if(s->eax!=q)stop(9,a,s->eax);
  if(i==0u) {
   if(s->esp!=c->esp-8u||s->esi!=n||s->edi!=c->args[1]||s->ebx!=c->root_entry.ebx||
      s->edx!=c->old_tail)stop(9,s->esp,0x4d68au);
   wanted=q+4u;value=c->old_tail;
  } else if(i==1u){wanted=q;value=l;if(s->ecx!=l)stop(9,a,i);}
  else if(i==2u){wanted=c->old_tail;value=q;if(s->edx!=c->old_tail)stop(9,a,i);}
  else if(i==3u){wanted=l+4u;value=q;if(s->ecx!=l)stop(9,a,i);}
  else if(i==4u){wanted=q+0x18u;value=c->args[1];if(s->edi!=value)stop(9,a,i);}
  else if(i==5u){wanted=q+0x1cu;value=c->args[2];if(s->ecx!=value)stop(9,a,i);}
  else if(i==6u){wanted=q+0xcu;value=n;if(s->esi!=n)stop(9,a,i);}
  else if(i==7u){wanted=q+8u;value=1u;}
  else if(i==8u){wanted=q+0x10u;value=q+0x20u;if(s->edx!=value)stop(9,a,i);}
  else if(i==9u){wanted=q+0x14u;value=0u;}
  else if(i==10u){wanted=n+0x4cu;value=c->old_count+1u;if(s->ecx!=value)stop(9,a,i);}
  else stop(9,a,i);
 } else {
  const uint32_t last=5u+c->input_span;
  if(i==0u) {
   const uint32_t *f=s43_frame(s,16u,0x4d549u);
   if(s->esp!=c->esp-8u||s->eax!=q||s->ecx!=c->old_tail||s->ebx!=c->hash_eax||
      s->esi!=c->root_entry.esi||s->edi!=c->args[0]||f[0]!=c->root_entry.edi||
      f[1]!=c->root_entry.ebx||f[2]!=c->ret||f[3]!=c->args[0])stop(9,s->esp,0x4d549u);
   wanted=q+0x28u;value=c->old_tail;
  } else if(i==1u){wanted=q+0x24u;value=0x2d02d8u;if(s->eax!=q)stop(9,a,i);}
  else if(i==2u){wanted=c->old_tail+0x24u;value=q;if(!c->continuation||s->eax!=q||s->edx!=c->old_tail)stop(9,a,i);}
  else if(i==3u){wanted=c->esp-12u;value=c->root_entry.esi;stack=1u;if(!c->continuation||s->esp!=wanted||s->esi!=value)stop(9,a,i);}
  else if(i==4u){wanted=0x2d0300u;value=q;if(s->eax!=q||s->ecx!=0x2d02d8u)stop(9,a,i);}
  else if(i>=5u&&i<last){uint32_t k=i-5u;wanted=q+k;size=1u;value=c->input[k];if((uint8_t)s->edx!=(uint8_t)value)stop(9,a,i);}
  else if(i==last){wanted=q+0x30u;value=q+0x2cu;if(s->ecx!=value)stop(9,a,i);}
  else if(i==last+1u){wanted=q+0x2cu;value=q+0x2cu;if(s->ecx!=value)stop(9,a,i);}
  else if(i==last+2u){wanted=q+0x20u;value=c->hash_eax;if(s->ebx!=value)stop(9,a,i);}
  else if(i==last+3u){wanted=q+0x4cu;value=0u;}
  else stop(9,a,i);
 }
 if(a!=wanted||bytes!=size||s->ebp!=c->root_entry.ebp||s->fs_base!=c->root_entry.fs_base||
    s->df||g_seh_ebp!=c->seh)stop(9,a,i);
 if(!stack)s43_patch(a,bytes,value);
 ++c->write_index;
}
static void s43_memory(uint32_t a,uint32_t n,uint32_t access,ff_sdk_scalar *s) {
 if(!sdk_allocator_scope()||scope43_guard)return;
 scope43_guard=1u;ff_scope43_call *c=s43_call();
 /* The only admitted write to this usage counter in this allocator phase is
    original4BFE5, after its free-list selection and before header splitting. */
 if(c->stage==S43_ALLOC&&(access&FF_VM_WRITE)&&a==0x2ccec8u) {
  if(n!=4u)stop(9,a,n);s43_selection(s);
 }
 /* The no-fit caller has no authored4C0DA observer. Its first actual post-RET
    memory access provides the same checked callee-cleanup witness instead. */
 if(c->stage==S43_ALLOC&&release_words[63]==3u&&c->release_ra==0x4bfb8u&&
    s->esp==release_words[2]+8u) {
  ff_sdk_shared_write(s);sdk_release_post(s);
 }
 if(c->stage==S43_WRITES&&(access&FF_VM_WRITE))s43_store(a,n,s);
 scope43_guard=0u;
}
static void s43_pre_ret(uint32_t guest,uint32_t site,ff_sdk_scalar *s) {
 ff_scope43_call *c=s43_call();
 if(guest==0x4dc30u) {
  const uint32_t *f=s43_frame(s,8u,guest);
  if(!c->hash_entered||c->hash_exited||s->esp!=c->hash_entry.esp||
     f[0]!=(c->kind==0x4d620u?0x4d62bu:0x4d4fcu)||f[1]!=c->args[0]||
     !s43_nonvolatile(&c->hash_entry,s)||g_seh_ebp!=c->seh)stop(9,s->esp,guest);
  c->hash_pre=*s;c->hash_exited=1u;recomp_trace_exit("sdk-shared",guest);return;
 }
 const uint32_t *f=s43_frame(s,c->kind==0x4d620u?16u:8u,guest);
 if(guest!=c->kind||c->pre_seen||s->esp!=c->esp||f[0]!=c->ret||f[1]!=c->args[0]||
    (c->kind==0x4d620u&&(f[2]!=c->args[1]||f[3]!=c->args[2]))||
    !s43_nonvolatile(&c->root_entry,s)||g_seh_ebp!=c->seh||!c->hash_done)
  stop(9,s->esp,guest);
 if(c->kind==0x4d620u&&(site!=0x4d653u&&site!=0x4d686u&&site!=0x4d6c3u))stop(9,site,guest);
 if(c->kind==0x4d620u) {
  if(c->q) {if(s->eax!=c->q||!c->pointer_ready||c->write_index!=11u)stop(9,s->eax,guest);}
  else if(s->eax||c->write_index||c->pointer_ready||(!c->alloc_seen&&c->matched))stop(9,s->eax,guest);
 } else {
  const uint32_t wanted=c->alloc_seen&&!c->q?0u:1u;
  if(s->eax!=wanted||(!c->alloc_seen&&!c->matched)||
     (c->q&&(!c->pointer_ready||!c->continuation||c->write_index!=9u+c->input_span)))stop(9,s->eax,guest);
 }
 s43_cache_links(s);
 if(c->kind==0x4d620u&&c->q)s43_object_links(c->matched,s);
 c->root_pre=*s;c->pre_seen=1u;c->pre_site=site;recomp_trace_exit("sdk-shared",guest);
}
void ff_probe_sdk_scope43_raw_pre_ret(uint32_t site,ff_sdk_scalar *s) {
 if(!sdk_scope43()||s43_call()->kind!=0x4d620u)stop(9,site,0x4d620u);
 ff_sdk_shared_write(s);s43_pre_ret(0x4d620u,site,s);
}
void ff_probe_sdk_scope43_return_post(uint32_t target,ff_sdk_scalar *s) {
 ff_scope43_call *c=s43_call();
 if(target!=c->kind||!c->pre_seen||c->post_seen||s->esp!=c->esp+4u||s->eax!=c->root_pre.eax||
    !s43_nonvolatile(&c->root_entry,s)||g_seh_ebp!=c->seh)stop(9,s->esp,target);
 const uint32_t *f=s43_frame(s,c->kind==0x4d620u?12u:4u,target);
 if(f[0]!=c->args[0]||(c->kind==0x4d620u&&(f[1]!=c->args[1]||f[2]!=c->args[2])))stop(9,s->esp,target);
 s43_cache_links(s);c->root_post=*s;c->post_seen=1u;c->result=s->eax;c->post_esp=s->esp;
 c->complete=1u;c->stage=S43_DONE;prefix_words[1]=1u;prefix_words[15]=s->eax;
 ff_sdk_shared_write(s);recomp_trace_exit("sdk-m43-post-ret",target);c->active=0u;
}
static uint32_t s43_scalar_word(const ff_sdk_scalar *s,uint32_t i) {
 const uint32_t v[10]={s->eax,s->ecx,s->edx,s->ebx,s->esp,s->ebp,s->esi,s->edi,s->fs_base,s->df};return v[i];
}
static uint32_t s43_evidence_word(const ff_sdk_invocation_evidence *e,uint32_t i) {
 if(i<24u)return e->prefix[i];i-=24u;if(i<24u)return e->hash[i];i-=24u;
 if(i<32u)return e->call[i];i-=32u;if(i<72u)return e->wait[i/24u][i%24u];i-=72u;
 if(i<40u)return e->extra[i];i-=40u;if(i<6u)return e->kernel_regs[i];i-=6u;
 if(i<48u)return e->node[i];i-=48u;if(i<64u)return e->release[i];i-=64u;
 return i<48u?e->continuation[i]:0u;
}
static uint32_t sdk_scope43_word(uint32_t i) {
 if(i<8u){const uint32_t v[8]={scope43_count,scope43_current,scope43_block_count,scope43_ready,4u,S43_META,S43_SCALARS,S43_EVIDENCE};return v[i];}
 i-=8u;const uint32_t bank=i/S43_RECORD_WORDS,w=i%S43_RECORD_WORDS;if(bank>=4u)return 0u;
 const ff_scope43_call *c=&scope43_calls[bank];
 if(w<S43_META) {
  const uint32_t v[64]={c->active,c->kind,c->stage,c->ret,c->esp,c->seh,c->args[0],c->args[1],c->args[2],c->input_span,
   c->entered,c->hash_entered,c->hash_exited,c->hash_done,c->hash_eax,c->hash_esp,c->alloc_seen,c->selected,c->selected_h,
   c->selected_size,c->total,c->header,c->extent,c->q,c->split,c->pointer_ready,c->release_ra,c->wait_a,c->wait_c,
   c->release_a,c->release_c,c->matched,c->old_tail,c->old_count,c->write_index,c->continuation,c->pre_seen,c->post_seen,
   c->pre_site,c->result,c->post_esp,c->new_block,c->complete};return v[w];
 }
 if(w<S43_META+S43_SCALARS) {
  const uint32_t n=(w-S43_META)/10u,k=(w-S43_META)%10u;
  const ff_sdk_scalar *s=n==0u?&c->root_entry:n==1u?&c->root_pre:n==2u?&c->root_post:
    n==3u?&c->allocator_entry:n==4u?&c->allocator_post:n==5u?&c->hash_entry:n==6u?&c->hash_pre:&c->hash_post;
  return s43_scalar_word(s,k);
 }
 return s43_evidence_word(&c->evidence,w-S43_META-S43_SCALARS);
}


static void s45_post(ff_sdk_scalar*);
static int sdk_scope44(void) {return (sdk_enabled==44u || sdk_enabled==45u)&&bridge44[0]==1u;}
static int sdk_allocator_scope(void) {return sdk_scope43()||sdk_scope44();}
uint32_t ff_probe_sdk_scope44_active(void) {return sdk_scope44()?1u:0u;}
static void s44_root_frame(ff_sdk_scalar *s,uint32_t bytes) {
 const uint32_t *f=s43_frame(s,bytes,0x4c480u);
 if(f[0]!=0x4f68eu||memcmp(f+1,bridge44+10u,16u))stop(9,s->esp,0x4c480u);
}
static uint32_t s44_hash_payload(const uint8_t *p) {
 uint32_t v=2166136261u;for(uint32_t i=0;i<0x40000u;i++)v=(v^p[i])*16777619u;return v;
}
static void s44_images(ff_sdk_scalar *s) {
 s43_images(s);
 if(allocator44.q) {
  s43_header(allocator44.header,allocator44.extent,s);
  const uint8_t *p=s43_read(allocator44.q,0x40000u,s);
  if(memcmp(p,payload44_before,0x40000u))stop(9,allocator44.q,0x40000u);
  bridge44[28]=s44_hash_payload(p);bridge44[29]=1u;
 }
}
static void s44_fp_preserved(const ff_guest_cpu *a,const ff_guest_cpu *b) {
 if(a->r_mxcsr!=b->r_mxcsr||a->r_fp_top!=b->r_fp_top||a->r_fp_cmp!=b->r_fp_cmp||
    a->r_fp_control_word!=b->r_fp_control_word||a->r_fp_cc!=b->r_fp_cc||a->r_force_return!=b->r_force_return||
    memcmp(a->fp_values,b->fp_values,sizeof a->fp_values)||memcmp(a->xmm_bytes,b->xmm_bytes,sizeof a->xmm_bytes)||
    memcmp(a->mm_bytes,b->mm_bytes,sizeof a->mm_bytes))stop(9,g_esp,0x4c480u);
}
void ff_sdk_shared_c_4c480(ff_sdk_scalar *s) {
 if(!s||(sdk_enabled!=44u && sdk_enabled!=45u)||sdk_current_target!=0x4f5e0u||bridge44[0]||bridge44[5]||
    sdk_scope43()||scope43_count!=3u||scope43_block_count!=6u||!scope43_ready||
    !scope43_calls[0].complete||!scope43_calls[1].complete||!scope43_calls[2].complete||
    scope43_calls[3].active||scope43_calls[3].complete||s->esp!=0x03c10e50u||s->df)
  stop(9,s?s->esp:0u,0x4c480u);
 ff_sdk_shared_write(s);ff_boot_thread *t=sdk_thread();
 if(g_seh_ebp!=scope43_calls[2].seh||s->ebp!=scope43_calls[2].root_entry.ebp||
    s->fs_base!=t->fs||g_fs_base!=t->fs||recomp_lookup(0x4c480u)!=ff_title_0004C480||
    ff_vm_check(&virtual_memory,0x4c480u,44u,FF_VM_EXECUTE))stop(9,s->esp,0x4c480u);
 const uint32_t *f=s43_frame(s,20u,0x4c480u);
 const uint32_t expected[5]={0x4f68eu,0x40000u,0x25c558u,0x162u,0x2006u};
 if(memcmp(f,expected,sizeof expected))stop(9,s->esp,0x4c480u);
 if(s->esp<t->stack_low+0x1000u||s->esp>t->stack_top-20u||
    ff_vm_check(&virtual_memory,t->stack_low,s->esp+20u-t->stack_low,FF_VM_READ|FF_VM_WRITE))
  stop(9,s->esp,0x4c480u);
 memset(&allocator44,0,sizeof allocator44);allocator44.kind=0x4c480u;
 allocator44.esp=s->esp;allocator44.ret=f[0];allocator44.seh=g_seh_ebp;allocator44.root_entry=*s;
 allocator44.active=1u;allocator44.stage=S43_ROOT;
 memcpy(bridge44+10u,f+1,16u);bridge44[9]=g_seh_ebp;bridge44[20]=20u;bridge44[25]=g_eflags;
 s43_images(s);bridge44[30]=1u;
 bridge44[6]=s43_u32(s43_read(0x2cd788u,4u,s));
 /* This profile admits a bounded selector prefix, not a universal table size. */
 if(bridge44[6]>=32u)stop(9,bridge44[6],0x2cd708u);
 bridge44[7]=s43_u32(s43_read(0x2cd708u+4u*bridge44[6],4u,s));
 bridge44[8]=s43_u32(s43_read(0x2cd78cu,4u,s));
 bridge44[32]=44u;
 for(uint32_t i=0;i<6u;i++)bridge44[40u+i]=ff_probe_abi_violation_word(i);
 ff_guest_cpu before,after;ff_capture_cpu(&before);
 sdk_shared_remaining_budget(1024u,0x4c480u);
 bridge44[0]=1u;
 ff_title_0004C480(); /* Retained original C body; all guest stores stay original. */
 ff_capture_cpu(&after);ff_sdk_shared_read(s);(void)sdk_thread();
 if(!bridge44[1]||!bridge44[3]||!bridge44[4]||bridge44[52]!=1u||bridge44[5]||
    s->esp!=allocator44.esp+4u||s->eax!=allocator44.q||
    !s43_nonvolatile(&allocator44.root_entry,s)||g_seh_ebp!=allocator44.seh)
  stop(9,s->esp,0x4c480u);
 const uint32_t *retained=(const uint32_t*)s43_read(allocator44.esp,20u,s);
 if(memcmp(retained,expected,sizeof expected))stop(9,allocator44.esp,0x4c480u);
 s44_fp_preserved(&before,&after);bridge44[38]=1u;
 for(uint32_t i=0;i<6u;i++){
  bridge44[46u+i]=ff_probe_abi_violation_word(i);
  if(bridge44[46u+i]!=bridge44[40u+i])stop(9,i,0x4c480u);
 }
 s44_images(s);bridge44[31]=1u;
 allocator44.root_post=*s;allocator44.post_seen=1u;allocator44.post_esp=s->esp;
 allocator44.result=s->eax;allocator44.complete=1u;allocator44.stage=S43_DONE;allocator44.active=0u;
 /* Publish completion into the NEW bank only after the real C return and all
    restoration/image guards, while the scoped accessor is still active. */
 allocator44.evidence.prefix[1]=1u;allocator44.evidence.prefix[15]=s->eax;
 bridge44[24]=s->esp;bridge44[26]=g_eflags;bridge44[5]=1u;bridge44[0]=0u;
 ff_sdk_shared_write(s);
}
void ff_title_0004BF10(void) {
 if(!sdk_scope44()){ff_title_0004BF10_c_fallback44();return;}
 ff_sdk_allocator_c44();
}
uint32_t ff_probe_sdk_scope44_allocator_begin(ff_sdk_scalar *s) {
 if(!s||!sdk_scope44()||!bridge44[1]||bridge44[2]||allocator44.stage!=S43_ROOT)return 0u;
 ff_sdk_shared_write(s);(void)sdk_thread();const uint32_t *f=s43_frame(s,28u,0x4bf10u);
 const uint32_t expected[7]={0x4c4a8u,bridge44[10],bridge44[11],bridge44[12],bridge44[13],bridge44[7],0u};
 if(s->esp!=allocator44.esp-28u||memcmp(f,expected,sizeof expected)||
    !s43_nonvolatile(&allocator44.root_entry,s)||g_seh_ebp!=allocator44.seh||
    s43_u32(s43_read(0x2cd788u,4u,s))!=bridge44[6]||
    s43_u32(s43_read(0x2cd708u+4u*bridge44[6],4u,s))!=bridge44[7]||
    s43_u32(s43_read(0x2cd78cu,4u,s))!=bridge44[8]||
    !local_mutant_ready||!local_mutant_wait_enabled||!local_mutant_release_enabled||
    sdk_wait_object(0x8000cu,0u)!=3u)stop(9,s->esp,0x4bf10u);
 memcpy(bridge44+14u,f+1,24u);bridge44[21]=s->esp;bridge44[2]=1u;
 allocator44.allocator_entry=*s;allocator44.alloc_seen=1u;allocator44.stage=S43_ALLOC;
 allocator44.total=0x40030u;
 allocator44.wait_a=local_mutant_wait_attempts;allocator44.wait_c=local_mutant_wait_completed;
 allocator44.release_a=local_mutant_release_attempts;allocator44.release_c=local_mutant_release_completed;
 s43_images(s);return 1u;
}
static void s44_pointer_post(ff_sdk_scalar *s) {
 ff_scope43_call *c=&allocator44;const uint32_t *args=s43_frame(s,24u,0x4c4a8u);
 if(!sdk_scope44()||bridge44[3]||c->stage!=S43_ALLOC||!call_words[1]||release_words[63]!=4u||release_words[47]||
    s->esp!=c->allocator_entry.esp+4u||s->eax!=call_words[18]||
    !s43_nonvolatile(&c->allocator_entry,s)||g_seh_ebp!=c->seh||
    local_mutant_wait_attempts!=c->wait_a+1u||local_mutant_wait_completed!=c->wait_c+1u||
    local_mutant_release_attempts!=c->release_a+1u||local_mutant_release_completed!=c->release_c+1u||
    sdk_wait_object(release_words[4],0u)!=release_words[17])stop(9,s->esp,0x4bf10u);
 for(uint32_t i=0;i<6u;i++){if(args[i]!=call_words[4u+i]||args[i]!=bridge44[14u+i])stop(9,s->esp,i);release_words[52u+i]=args[i];}
 c->allocator_post=*s;c->q=s->eax;
 if(c->q) {
  if(!c->selected||!c->header||c->q!=c->header+0x30u||c->extent<0x40030u)stop(9,c->q,44u);
  const ff_boot_thread *t=sdk_thread();
  if((c->header<t->stack_top&&t->stack_low<c->header+c->extent)||
     (c->header<0x2d0730u&&0x2d02d8u<c->header+c->extent)||
     (c->header<0x2cd800u&&0x2cce80u<c->header+c->extent))stop(9,c->header,c->extent);
  s43_header(c->header,c->extent,s);s43_images(s);
  for(uint32_t i=0;i<scope43_block_count;i++){
   const ff_scope43_block *b=&scope43_blocks[i];
   if(c->header<b->header+b->extent&&b->header<c->header+c->extent)stop(9,c->header,b->header);
  }
  if(ff_vm_check(&virtual_memory,c->q,0x40000u,FF_VM_READ|FF_VM_WRITE))stop(9,c->q,0x40000u);
  memcpy(payload44_before,s43_read(c->q,0x40000u,s),0x40000u);
  bridge44[27]=s44_hash_payload(payload44_before);bridge44[33]=c->header;bridge44[34]=c->extent;
  bridge44[35]=c->q;bridge44[36]=c->total;bridge44[37]=0x40000u;c->pointer_ready=1u;
 }else if(c->selected||c->release_ra!=0x4bfb8u)stop(9,c->q,44u);
 release_words[47]=1u;release_words[48]=s->esp;release_words[49]=call_words[3];release_words[50]=s->eax;
 release_words[51]=call_words[1];release_words[58]=0x40000u;release_words[59]=6u;
 release_words[60]=0u;release_words[61]=24u;release_words[63]=5u;c->stage=S43_WRITES;
 bridge44[22]=s->esp;bridge44[3]=1u;
}
void ff_probe_sdk_scope44_allocator_post(ff_sdk_scalar *s) {
 s44_pointer_post(s);ff_sdk_shared_write(s);recomp_trace_exit("sdk-m44-post-ret",0x4bf10u);
}
static void s44_trace(uint32_t kind,uint32_t guest) {
 if(!sdk_scope44()||guest!=0x4c480u)return;
 ff_sdk_scalar s;ff_sdk_shared_read(&s);s44_root_frame(&s,20u);
 if(kind==1u) {
  if(bridge44[1]||allocator44.stage!=S43_ROOT||!scope41_same_scalar(&allocator44.root_entry,&s))stop(9,s.esp,guest);
  uint32_t *p=allocator44.evidence.prefix;
  if(p[0]||p[1])stop(9,s.esp,guest);
  p[0]=1u;p[2]=s.esp;p[3]=allocator44.ret;
  p[4]=bridge44[10];p[5]=bridge44[11];p[6]=bridge44[12];
  p[7]=s.ebx;p[8]=s.esi;p[9]=s.edi;p[10]=s.ebp;p[12]=allocator44.seh;
  bridge44[1]=1u;allocator44.entered=1u;
 }else if(kind==2u) {
  if(!bridge44[3]||bridge44[52]!=1u||bridge44[4]||allocator44.stage!=S43_WRITES||s.esp!=allocator44.esp||
     s.eax!=allocator44.q||!s43_nonvolatile(&allocator44.root_entry,&s)||g_seh_ebp!=allocator44.seh)
   stop(9,s.esp,guest);
  s44_images(&s);allocator44.root_pre=s;allocator44.pre_seen=1u;bridge44[23]=s.esp;bridge44[4]=1u;
 }else stop(9,kind,guest);
}
static void s44_esp(const char *name,const char *tag) {
 if(!sdk_scope44()||strcmp(name,"ff_title_0004C480")||strcmp(tag,"after call 0x0004BF10"))return;
 if(!bridge44[3]||bridge44[52]||g_esp!=bridge44[22]||g_esp!=allocator44.esp-24u)stop(9,g_esp,0x4c4a8u);
 bridge44[52]=1u;bridge44[53]=g_esp;
}
void ff_probe_sdk_scope44_cpp_complete(void) {
 if((sdk_enabled!=44u && sdk_enabled!=45u)||bridge44[0]||bridge44[5]!=1u||bridge44[39])stop(9,g_esp,0x4c480u);
 bridge44[39]=1u;
}
static uint32_t sdk_scope44_word(uint32_t i) {
 if(i<64u)return bridge44[i];i-=64u;
 const ff_scope43_call *c=&allocator44;
 if(i<64u){const uint32_t v[64]={c->active,c->kind,c->stage,c->ret,c->esp,c->seh,c->args[0],c->args[1],c->args[2],c->input_span,
  c->entered,c->hash_entered,c->hash_exited,c->hash_done,c->hash_eax,c->hash_esp,c->alloc_seen,c->selected,c->selected_h,
  c->selected_size,c->total,c->header,c->extent,c->q,c->split,c->pointer_ready,c->release_ra,c->wait_a,c->wait_c,
  c->release_a,c->release_c,c->matched,c->old_tail,c->old_count,c->write_index,c->continuation,c->pre_seen,c->post_seen,
  c->pre_site,c->result,c->post_esp,c->new_block,c->complete};return v[i];}i-=64u;
 if(i<80u){const uint32_t n=i/10u,k=i%10u;const ff_sdk_scalar *s=n==0u?&c->root_entry:n==1u?&c->root_pre:n==2u?&c->root_post:
  n==3u?&c->allocator_entry:n==4u?&c->allocator_post:n==5u?&c->hash_entry:n==6u?&c->hash_pre:&c->hash_post;return s43_scalar_word(s,k);}i-=80u;
 return i<358u?s43_evidence_word(&c->evidence,i):0u;
}

static const uint32_t *scope41_read(uint32_t va,uint32_t bytes,ff_sdk_scalar *s)
{
    ff_boot_thread *t=sdk_thread();
    if(!bytes || va>virtual_memory.bytes || bytes>virtual_memory.bytes-va ||
       ff_vm_check(&virtual_memory,va,bytes,FF_VM_READ))stop(9,va,bytes);
    if(bytes==4u && ((va&3u)!=0u))stop(9,va,bytes);
    if(va< t->stack_low || va>t->stack_top || bytes>t->stack_top-va) {
        /* Non-stack data is still checked against the authoritative VM map. */
        if(ff_vm_check(&virtual_memory,va,bytes,FF_VM_READ))stop(9,va,bytes);
    }
    return (const uint32_t *)ff_sdk_shared_memory(va,bytes,FF_VM_READ,s);
}
uint32_t ff_probe_sdk_scope41_active(void)
{ return (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u)))) && sdk_current_target==0x4f5e0u && scope41_words[S41_ACTIVE]==1u; }

uint32_t ff_probe_sdk_scope41_begin(uint32_t guest,ff_sdk_scalar *s)
{
    if((sdk_enabled!=41u && (sdk_enabled!=42u && (sdk_enabled!=43u && (sdk_enabled!=44u && sdk_enabled!=45u)))) || sdk_current_target!=0x4f5e0u || !s)return 0u;
    ff_boot_thread *t=sdk_thread();
    if(guest==0x4d4f0u) {
        if(scope41_words[S41_ACTIVE] || scope41_words[S41_STAGE]!=0u || s->esp!=0x03c10e5cu || (s->esp&3u) ||
           s->esp<t->stack_low || s->esp>t->stack_top || 8u>t->stack_top-s->esp)
            return 0u;
        const uint32_t *frame=scope41_read(s->esp,8u,s);
        if(frame[0]!=0x4f639u || frame[1]!=0x0025c4d0u)return 0u;
        memset(scope41_words,0,sizeof scope41_words);
        scope41_words[S41_ACTIVE]=1u;scope41_words[S41_STAGE]=1u;
        scope41_words[S41_PARENT]=0x4f5e0u;scope41_words[S41_CALLSITE]=0x4f634u;
        scope41_words[S41_CALLEE]=guest;scope41_words[S41_RETURN]=frame[0];
        scope41_words[S41_ENTRY_ESP]=s->esp;scope41_words[S41_ARG0]=frame[1];if((sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u))))scope42_root_seh=g_seh_ebp;
        scope41_words[S41_EAX]=s->eax;scope41_words[S41_ECX]=s->ecx;
        scope41_words[S41_EDX]=s->edx;scope41_words[S41_EBX]=s->ebx;
        scope41_words[S41_ESP]=s->esp;scope41_words[S41_EBP]=s->ebp;
        scope41_words[S41_ESI]=s->esi;scope41_words[S41_EDI]=s->edi;
        scope41_words[S41_FS]=s->fs_base;scope41_words[S41_DF]=s->df;
        return 1u;
    }
    if(guest==0x4dc30u && scope41_words[S41_ACTIVE]==1u &&
       scope41_words[S41_STAGE]==2u) {
        if(s->esp!=0x03c10e4cu || (s->esp&3u) || s->esp<t->stack_low ||
           s->esp>t->stack_top || 8u>t->stack_top-s->esp)return 0u;
        const uint32_t *frame=scope41_read(s->esp,8u,s);
        if(frame[0]!=0x4d4fcu || frame[1]!=scope41_words[S41_ARG0])return 0u;
        /* Hash table is 256 checked entries; the source body indexes a byte. */
        (void)scope41_read(0x2d0330u,1024u,s);
        /* Validate the actual NUL-terminated byte span without inventing a
           title-specific length. Existing checked memory/work limits bound it. */
        const uint32_t start=frame[1];
        if(start>=virtual_memory.bytes)stop(9,start,0x4dc30u);
        uint32_t terminated=0u;
        for(uint32_t off=0u;off<virtual_memory.bytes-start;++off) {
            const uint8_t *b=(const uint8_t *)scope41_read(start+off,1u,s);
            if(*b==0u){terminated=1u;break;}
        }
        if(!terminated)stop(9,start,0x4dc30u);
        memset(&scope41_words[S41_CHILD_STAGE],0,15u*sizeof(uint32_t));
        scope41_words[S41_CHILD_STAGE]=1u;scope41_words[S41_CHILD_ESP]=s->esp;
        scope41_words[S41_CHILD_RA]=frame[0];scope41_words[S41_CHILD_ARG]=frame[1];
        scope41_words[S41_CHILD_ENTRY_ESI]=s->esi;scope41_words[S41_CHILD_ENTRY_EDI]=s->edi;scope41_child_entry=*s;
        scope41_words[S41_STAGE]=3u;
        return 1u;
    }
    return 0u;
}
void ff_probe_sdk_scope41_enter(uint32_t guest,ff_sdk_scalar *s)
{
    if(!s || !ff_probe_sdk_scope41_active())stop(9,guest,0);
    if(guest==0x4d4f0u && scope41_words[S41_STAGE]==1u &&
       s->esp==scope41_words[S41_ENTRY_ESP]) { scope41_words[S41_STAGE]=2u;return; }
    if(guest==0x4dc30u && scope41_words[S41_STAGE]==3u &&
       s->esp==scope41_words[S41_CHILD_ESP])return;
    stop(9,s->esp,guest);
}
void ff_probe_sdk_scope41_pre_ret(uint32_t guest,ff_sdk_scalar *s)
{
    if(!s || !ff_probe_sdk_scope41_active())stop(9,guest,0);
    if(guest==0x4dc30u && scope41_words[S41_CHILD_STAGE]==1u &&
       s->esp==scope41_words[S41_CHILD_ESP] &&
       s->esi==scope41_child_entry.esi && s->edi==scope41_child_entry.edi &&
       s->ebx==scope41_child_entry.ebx && s->ebp==scope41_child_entry.ebp &&
       s->fs_base==scope41_child_entry.fs_base && s->df==scope41_child_entry.df &&
       *(const uint32_t *)scope41_read(s->esp,4u,s)==scope41_words[S41_CHILD_RA]) {
        scope41_words[S41_CHILD_PRE_ESP]=s->esp;return;
    }
    if(guest==0x4d4f0u && scope41_words[S41_STAGE]>=2u &&
       s->esp==scope41_words[S41_ENTRY_ESP] && s->ebx==scope41_words[S41_EBX] &&
       s->esi==scope41_words[S41_ESI] && s->ebp==scope41_words[S41_EBP] &&
       s->edi==scope41_words[S41_EDI] && s->fs_base==scope41_words[S41_FS] &&
       s->df==scope41_words[S41_DF] &&
       *(const uint32_t *)scope41_read(s->esp,4u,s)==scope41_words[S41_RETURN]) {
        scope41_words[S41_PRE_ESP]=s->esp;scope41_words[S41_PRE_RA]=scope41_words[S41_RETURN];return;
    }
    stop(9,s->esp,guest);
}
static uint32_t sdk_wait_object(uint32_t handle,uint32_t owner);
static int scope41_allocated_node(uint32_t address,uint32_t node_hash,ff_sdk_scalar *s)
{
    (void)s;
    static const uint32_t args[6]={0x50u,0x25bfa8u,0x5bu,0x2001u,0x25bfccu,0u};
    for(uint32_t bank=0;bank<2u;++bank) {
        const uint32_t *call=invocation_evidence[bank].call;
        const uint32_t *release=invocation_evidence[bank].release;
        if(call[0]!=1u || call[1]!=1u || call[18]!=address || call[10]!=node_hash ||
           release[47]!=1u || release[50]!=address || release[58]!=0x50u ||
           release[59]!=6u || release[63]!=5u || !release[4] || !release[17] ||
           sdk_wait_object(release[4],0u)!=release[17] || address<0x30u)continue;
        uint32_t matches=1u;
        for(uint32_t i=0;i<6u;++i)if(call[4u+i]!=args[i] || release[52u+i]!=args[i])matches=0u;
        if(!matches)continue;
        /* These retained call/return and sdk_pointer_post records prove this exact
           allocator payload and extent. release[60] is synchronization ownership,
           not heap ownership, so it is not used as the node-owner claim. */
        if(!ff_vm_check(&virtual_memory,address,0x50u,FF_VM_READ|FF_VM_WRITE))return 1;
    }
    return 0;
}
static void scope41_validate_list(ff_sdk_scalar *s)
{
    const uint32_t sentinel=0x2d02d8u;
    const uint32_t head=*(const uint32_t *)scope41_read(0x2d02fcu,4u,s);
    const uint32_t tail=*(const uint32_t *)scope41_read(0x2d0300u,4u,s);
    if(head==sentinel) { if(tail!=sentinel)stop(9,tail,sentinel); return; }
    if(tail==sentinel)stop(9,head,tail);
    uint32_t seen[16]={0},count=0,previous=sentinel,current=head,last=sentinel;
    while(current!=sentinel) {
        if(count>=16u || (current&3u))
            stop(9,current,0x4d4f0u);
        for(uint32_t i=0;i<count;++i)if(seen[i]==current)stop(9,current,0x4d4f0u);
        if(ff_vm_check(&virtual_memory,current,0x50u,FF_VM_READ|FF_VM_WRITE))stop(9,current,0x50u);
        const uint32_t *node=(const uint32_t *)scope41_read(current,0x50u,s);
        const uint32_t node_hash=node[0x20u/4u];
        const uint32_t next=node[0x24u/4u],prev=node[0x28u/4u];
        if(!scope41_allocated_node(current,node_hash,s))stop(9,current,node_hash);
        if(prev!=previous || !next || next==current)stop(9,current,0x4d4f0u);
        seen[count++]=current;previous=current;last=current;current=next;
    }
    if(last!=tail)stop(9,last,tail);
}
static int scope41_same_scalar(const ff_sdk_scalar *a,const ff_sdk_scalar *b)
{
    return a->eax==b->eax && a->ecx==b->ecx && a->edx==b->edx && a->ebx==b->ebx &&
           a->esp==b->esp && a->ebp==b->ebp && a->esi==b->esi && a->edi==b->edi &&
           a->fs_base==b->fs_base && a->df==b->df;
}
void ff_probe_sdk_scope41_post_ret(uint32_t guest,ff_sdk_scalar *s)
{
    if(!s || !ff_probe_sdk_scope41_active())stop(9,guest,0);
    if(guest==0x4dc30u && scope41_words[S41_CHILD_STAGE]==1u &&
       scope41_words[S41_CHILD_PRE_ESP]==scope41_words[S41_CHILD_ESP] &&
       s->esp==scope41_words[S41_CHILD_ESP]+4u) {
        scope41_words[S41_CHILD_EAX]=s->eax;scope41_words[S41_CHILD_ECX]=s->ecx;
        scope41_words[S41_CHILD_EDX]=s->edx;scope41_words[S41_CHILD_EBX]=s->ebx;
        scope41_words[S41_CHILD_POST_ESP]=s->esp;scope41_words[S41_CHILD_EBP]=s->ebp;
        scope41_words[S41_CHILD_ESI]=s->esi;scope41_words[S41_CHILD_EDI]=s->edi;
        scope41_words[S41_CHILD_FS]=s->fs_base;scope41_words[S41_CHILD_DF]=s->df;
        scope41_words[S41_CHILD_STAGE]=2u;scope41_words[S41_STAGE]=4u;
        scope41_validate_list(s);
        const ff_sdk_scalar actual=*s;ff_sdk_shared_write(s);
        recomp_trace_exit("sdk-shared-post-ret",guest);
        ff_sdk_scalar observed={0};ff_sdk_shared_read(&observed);
        if(!scope41_same_scalar(&actual,&observed))stop(9,s->esp,guest);
        return;
    }
    if(guest==0x4d4f0u && scope41_words[S41_STAGE]==4u && scope41_words[S41_CHILD_STAGE]==2u &&
       scope41_words[S41_PRE_ESP]==scope41_words[S41_ENTRY_ESP] &&
       s->esp==scope41_words[S41_ENTRY_ESP]+4u) {
        scope41_words[S41_POST_EAX]=s->eax;scope41_words[S41_POST_ECX]=s->ecx;
        scope41_words[S41_POST_EDX]=s->edx;scope41_words[S41_POST_EBX]=s->ebx;
        scope41_words[S41_POST_ESP]=s->esp;scope41_words[S41_POST_EBP]=s->ebp;
        scope41_words[S41_POST_ESI]=s->esi;scope41_words[S41_POST_EDI]=s->edi;
        scope41_words[S41_POST_FS]=s->fs_base;scope41_words[S41_POST_DF]=s->df;
        scope41_words[S41_STAGE]=5u;scope41_words[S41_ACTIVE]=0u;
        const ff_sdk_scalar actual=*s;ff_sdk_shared_write(s);
        recomp_trace_exit("sdk-shared-post-ret",guest);
        ff_sdk_scalar observed={0};ff_sdk_shared_read(&observed);
        if(!scope41_same_scalar(&actual,&observed))stop(9,s->esp,guest);
        return;
    }
    stop(9,s->esp,guest);
}
void ff_probe_sdk_scope41_allocator_attempt(ff_sdk_scalar *s)
{
    if(!s || !ff_probe_sdk_scope41_active() || scope41_words[S41_STAGE]!=4u ||
       s->esp!=0x03c10e38u || (s->esp&3u))stop(9,s?s->esp:0u,0x4bf10u);
    ff_boot_thread *t=sdk_thread();
    if(s->esp<t->stack_low || s->esp>t->stack_top || 28u>t->stack_top-s->esp)
        stop(9,s->esp,0x4bf10u);
    const uint32_t *frame=scope41_read(s->esp,28u,s);
    if(frame[0]!=0x4d539u || frame[1]!=0x50u || frame[2]!=0x25bfa8u ||
       frame[3]!=0x5bu || frame[4]!=0x2001u || frame[5]!=0x25bfccu || frame[6]!=0u)
        stop(9,s->esp,0x4bf10u);
    /* Preserve the completed hash-child actual-return record when the optional
       allocator path is reached and deliberately denied by shared_enter. */
    scope41_words[S41_STAGE]=6u;
    ++scope41_words[S41_REJECTED];
}

static void sdk_require_zero_xpp(uint32_t address,ff_sdk_scalar *scalar)
{
    if (ff_vm_check(&virtual_memory,address,12u,FF_VM_READ|FF_VM_WRITE))
        stop(9,address,FF_VM_READ|FF_VM_WRITE);
    for (uint32_t i=0;i<3u;++i) {
        uint32_t value=*(uint32_t *)ff_sdk_shared_memory(address+4u*i,4u,FF_VM_READ,scalar);
        if (value) stop(9,address+4u*i,0);
    }
}
void ff_probe_reset(void)
{
    memset(scope41_words,0,sizeof scope41_words);
    ff_disc_reset(&title_disc);memset(disc_words,0,sizeof disc_words);
    memset(mapped_words,0,sizeof mapped_words);memset(mapped_tls_before,0,20u);
    memset(tls_getter_words,0,sizeof tls_getter_words);memset(tls_getter_tls_before,0,sizeof tls_getter_tls_before);
    memset(dosmap_words,0,sizeof dosmap_words);memset(dosmap_ptd_before,0,sizeof dosmap_ptd_before);memset(dosmap_ptd_after,0,sizeof dosmap_ptd_after);
    memset(mapped_tls_kernel,0,20u);memset(mapped_tls_after,0,20u);
    sdk_enabled = sdk_active = sdk_current_target = 0;
    stdio_active=0u;memset(stdio_words,0,sizeof stdio_words);memset(stdio_inputs,0,sizeof stdio_inputs);
    memset(stdio_object_before,0,sizeof stdio_object_before);memset(stdio_object_after,0,sizeof stdio_object_after);
    memset(stdio_locks_before,0,sizeof stdio_locks_before);
    memset(close_words,0,sizeof close_words);memset(close_parent_words,0,sizeof close_parent_words);
    memset(error_words,0,sizeof error_words);memset(tls_words,0,sizeof tls_words);memset(error_outer_words,0,sizeof error_outer_words);
    memset(error_tls_before,0,sizeof error_tls_before);memset(error_tls_after,0,sizeof error_tls_after);
    memset(query_words,0,sizeof query_words);
    memset(copy_words,0,sizeof copy_words);
    memset(copy_source,0,sizeof copy_source);memset(copy_before,0,sizeof copy_before);
    memset(copy_expected,0,sizeof copy_expected);
    memset(record_log_words,0,sizeof record_log_words);
    memset(logger45,0,sizeof logger45);
    memset(gate_helper_words,0,sizeof gate_helper_words);
    memset(directory_cursor.name,0,sizeof directory_cursor.name);memset(&directory_cursor,0,sizeof directory_cursor);
    memset(next_query_words,0,sizeof next_query_words);
    memset(next_query_before,0,sizeof next_query_before);memset(next_query_after,0,sizeof next_query_after);
    memset(next_iosb_before,0,sizeof next_iosb_before);memset(next_iosb_after,0,sizeof next_iosb_after);
    memset(next_directory_words,0,sizeof next_directory_words);
    memset(compare_evidence,0,sizeof compare_evidence);compare_index=compare_started=compare_returned=0u;
    memset(directory_words,0,sizeof directory_words);
    memset(startup_words,0,sizeof startup_words);
    scope42_root_seh=0;memset(&scope42_allocator,0,sizeof scope42_allocator);memset(&allocator42_evidence,0,sizeof allocator42_evidence);
    memset(&allocator44,0,sizeof allocator44);memset(bridge44,0,sizeof bridge44);
    memset(scope43_calls,0,sizeof scope43_calls);memset(scope43_blocks,0,sizeof scope43_blocks);scope43_count=scope43_current=scope43_block_count=scope43_ready=scope43_guard=0u;
    sdk_invocation=0;memset(invocation_evidence,0,sizeof invocation_evidence);memset(tail_words,0,sizeof tail_words);
    memset(continuation_words,0,sizeof continuation_words);
    memset(release_words,0,sizeof release_words);
    memset(node_words,0,sizeof node_words);
    memset(wait_frames,0,sizeof wait_frames);memset(wait_extra,0,sizeof wait_extra);memset(wait_kernel_regs,0,sizeof wait_kernel_regs);
    memset(call_words,0,sizeof call_words);
    memset(prefix_words,0,sizeof prefix_words);memset(hash_words,0,sizeof hash_words);
    ff_irql_pair_init(&irql_pair);irql_imports=0;memset(irql_raw,0,sizeof irql_raw);memset(irql_words,0,sizeof irql_words);
    memset(host_words,0,sizeof host_words);ff_controller_device_init(&host_controllers);
    setup_attempts=setup_unexpected_returns=0;
    stos_attempts=stos_completed=stos_eax=stos_ecx=stos_edi=0;
    leaf_attempts=leaf_completed=init_attempts=init_completed=0;
    launch_data_enabled=launch_data_original=launch_data_disk=0;
    memset(sdk_words, 0, sizeof sdk_words);
    memset(device_stos,0,sizeof device_stos);
    memset(log_bridge_words,0,sizeof log_bridge_words);
    memset(second_log_bridge_words,0,sizeof second_log_bridge_words);
    memset(third_log_bridge_words,0,sizeof third_log_bridge_words);
    memset(parent_setup_words,0,sizeof parent_setup_words);
    memset(parent_fill_words,0,sizeof parent_fill_words);
    third_log_memory_stage=third_log_memory_reads=0;
    memset(second_query_words,0,sizeof second_query_words);
    memset(title_return_words,0,sizeof title_return_words);
    memset(tick_words,0,sizeof tick_words);memset(tick_extra_words,0,sizeof tick_extra_words);
    tick_enabled=tick_raw_slot=tick_initial_cell=tick_stage=tick_slot_reads=tick_cell_reads=tick_refreshing=tick_prewrite_reads=tick_entry_recheck_reads=tick_entry_frame_reads=tick_exit_frame_reads=0;
    tick_entry_esp=tick_entry_eflags=tick_entry_seh=0;
    getter_completed=getter_active=getter_active_id=0;
    ff_probe_legacy_reset();
}
uint32_t ff_probe_sdk_chain_profile(uint32_t enable)
{
    if (enable>45u || sdk_active || local_mutant_wait_sealed || probe_trace_count || kernel_call_count || parent_returned)
        return FF_BOOT_INVALID;
    if (enable>=9u && !host_words[0]) return FF_BOOT_INVALID;
    uint32_t import_status=sdk_irql_validate();
    if (import_status) return import_status;
    uint32_t tick_status=FF_BOOT_OK;
    if (enable>=14u || tick_enabled) { tick_status=sdk_tick_validate(); if (tick_status) return tick_status; }

    uint32_t data_status=sdk_launch_data_profile(enable>=3u);
    if (data_status) return data_status;
    uint32_t irql_status=sdk_irql_profile(enable>=11u);
    if (irql_status) return irql_status;
    tick_status=sdk_tick_profile(enable>=14u);
    if (tick_status) return tick_status;
    sdk_enabled = enable; sdk_words[0] = enable;
    return 0;
}
static uint32_t sdk_invocation_word(uint32_t bank,uint32_t index)
{
    if(bank>=2u)return 0u;
    const ff_sdk_invocation_evidence *v=&invocation_evidence[bank];
    if(index>=413u && index<437u)return v->prefix[index-413u];
    if(index>=437u && index<461u)return v->hash[index-437u];
    if(index>=461u && index<493u)return v->call[index-461u];
    if(index>=493u && index<565u)return v->wait[(index-493u)/24u][(index-493u)%24u];
    if(index>=565u && index<605u)return v->extra[index-565u];
    if(index>=605u && index<653u)return v->node[index-605u];
    if(index>=653u && index<717u)return v->release[index-653u];
    if(index>=717u && index<765u)return v->continuation[index-717u];
    return 0u;
}
uint32_t ff_probe_sdk_chain_word(uint32_t index) {
    if(sdk_enabled==45u && index>=7943u && index<8967u)return logger45[index-7943u];
    if((sdk_enabled==44u || sdk_enabled==45u) && index>=7377u && index<7943u)return sdk_scope44_word(index-7377u);
    if((sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u)) && index>=5361u && index<7377u)return sdk_scope43_word(index-5361u);
    if((sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u))) && index>=4887u && index<5361u)return sdk_scope42_word(index-4887u);
    if(index>=4839u && index<4887u)return scope41_words[index-4839u];
    if((sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u))))))) && index>=4299u && index<4363u)return tls_getter_words[index-4299u];
    if((sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u))))))) && index>=4363u && index<4383u)return tls_getter_tls_before[index-4363u];
    if((sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u)))))) && index>=4383u && index<4575u)return dosmap_words[index-4383u];
    if((sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u)))))) && index>=4575u && index<4707u)return dosmap_ptd_before[index-4575u];
    if((sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u)))))) && index>=4707u && index<4839u)return dosmap_ptd_after[index-4707u];
    if((sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u)))))))) && index>=4111u && index<4239u)return mapped_words[index-4111u];
    if((sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u)))))))) && index>=4239u && index<4259u)return mapped_tls_before[index-4239u];
    if((sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u)))))))) && index>=4259u && index<4279u)return mapped_tls_kernel[index-4259u];
    if((sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u)))))))) && index>=4279u && index<4299u)return mapped_tls_after[index-4279u];
    if((sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u)))))))))) && index>=3316u && index<3956u)return stdio_words[index-3316u];
    if((sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u)))))))))) && index>=3956u && index<3975u)return stdio_inputs[index-3956u];
    if((sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u)))))))))) && index>=3975u && index<4007u)return stdio_object_before[index-3975u];
    if((sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u)))))))))) && index>=4007u && index<4039u)return stdio_object_after[index-4007u];
    if((sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u)))))))))) && index>=4039u && index<4095u)return stdio_locks_before[index-4039u];
    if((sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u)))))))))) && index>=4095u && index<4111u){
        const uint32_t live[16]={g_ebp,g_seh_ebp,g_esp,g_fs_base,(uint32_t)g_df,g_eax,g_ecx,g_edx,g_ebx,g_esi,g_edi,stdio_active,kernel_call_count,abi_violation_count,abi_esp_count,g_eflags};
        return live[index-4095u];
    }
    if((sdk_enabled==34u || (sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u))))))))))) && index>=3124u && index<3252u)return close_words[index-3124u];
    if((sdk_enabled==34u || (sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u))))))))))) && index>=3252u && index<3316u)return close_parent_words[index-3252u];
    if((sdk_enabled==33u || (sdk_enabled==34u || (sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u)))))))))))) && index>=2892u && index<2956u)return error_words[index-2892u];
    if((sdk_enabled==33u || (sdk_enabled==34u || (sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u)))))))))))) && index>=2956u && index<3052u)return tls_words[index-2956u];
    if((sdk_enabled==33u || (sdk_enabled==34u || (sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u)))))))))))) && index>=3052u && index<3084u)return error_outer_words[index-3052u];
    if((sdk_enabled==33u || (sdk_enabled==34u || (sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u)))))))))))) && index>=3084u && index<3104u)return error_tls_before[index-3084u];
    if((sdk_enabled==33u || (sdk_enabled==34u || (sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u)))))))))))) && index>=3104u && index<3124u)return error_tls_after[index-3104u];
    if((sdk_enabled==32u || (sdk_enabled==33u || (sdk_enabled==34u || (sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u))))))))))))) && index>=2156u && index<2220u)return next_query_words[index-2156u];
    if((sdk_enabled==32u || (sdk_enabled==33u || (sdk_enabled==34u || (sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u))))))))))))) && index>=2220u && index<2548u)return next_query_before[index-2220u];
    if((sdk_enabled==32u || (sdk_enabled==33u || (sdk_enabled==34u || (sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u))))))))))))) && index>=2548u && index<2876u)return next_query_after[index-2548u];
    if((sdk_enabled==32u || (sdk_enabled==33u || (sdk_enabled==34u || (sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u))))))))))))) && index>=2876u && index<2884u)return next_iosb_before[index-2876u];
    if((sdk_enabled==32u || (sdk_enabled==33u || (sdk_enabled==34u || (sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u))))))))))))) && index>=2884u && index<2892u)return next_iosb_after[index-2884u];
    if((sdk_enabled==31u || (sdk_enabled==32u || (sdk_enabled==33u || (sdk_enabled==34u || (sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u)))))))))))))) && index>=2092u && index<2156u)return next_directory_words[index-2092u];
    if((sdk_enabled==30u || (sdk_enabled==31u || (sdk_enabled==32u || (sdk_enabled==33u || (sdk_enabled==34u || (sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u))))))))))))))) && index>=1962u && index<2026u)return compare_evidence[0].words[index-1962u];
    if((sdk_enabled==30u || (sdk_enabled==31u || (sdk_enabled==32u || (sdk_enabled==33u || (sdk_enabled==34u || (sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u))))))))))))))) && index>=2026u && index<2090u)return compare_evidence[1].words[index-2026u];
    if((sdk_enabled==30u || (sdk_enabled==31u || (sdk_enabled==32u || (sdk_enabled==33u || (sdk_enabled==34u || (sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u))))))))))))))) && index==2090u)return compare_started;
    if((sdk_enabled==30u || (sdk_enabled==31u || (sdk_enabled==32u || (sdk_enabled==33u || (sdk_enabled==34u || (sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u))))))))))))))) && index==2091u)return compare_returned;
    if((sdk_enabled==29u || (sdk_enabled==30u || (sdk_enabled==31u || (sdk_enabled==32u || (sdk_enabled==33u || (sdk_enabled==34u || (sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u)))))))))))))))) && index>=1898u && index<1962u)return gate_helper_words[index-1898u];
    if((sdk_enabled==28u || (sdk_enabled==29u || (sdk_enabled==30u || (sdk_enabled==31u || (sdk_enabled==32u || (sdk_enabled==33u || (sdk_enabled==34u || (sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u))))))))))))))))) && index>=1758u && index<1822u)return record_log_words[index-1758u];
    if((sdk_enabled==28u || (sdk_enabled==29u || (sdk_enabled==30u || (sdk_enabled==31u || (sdk_enabled==32u || (sdk_enabled==33u || (sdk_enabled==34u || (sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u))))))))))))))))) && index>=1822u && index<1898u)return copy_source[index-1822u];
    if((sdk_enabled==27u || (sdk_enabled==28u || (sdk_enabled==29u || (sdk_enabled==30u || (sdk_enabled==31u || (sdk_enabled==32u || (sdk_enabled==33u || (sdk_enabled==34u || (sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u)))))))))))))))))) && index>=1453u && index<1758u)return copy_before[index-1453u];
    if(index>=1325u && index<1453u)return copy_words[index-1325u];
    if(index>=1261u && index<1325u)return query_words[index-1261u];
    if(index>=1197u && index<1261u)return directory_words[index-1197u];
    if(index>=1133u && index<1197u)return startup_words[index-1133u];
    if(index>=765u && index<781u)return tail_words[index-765u];
    if(index>=781u && index<1133u)return sdk_invocation_word(1u,index-368u);
    if(index>=413u && index<765u)return sdk_invocation_word(0u,index);

    if(index>=717u && index<765u)return continuation_words[index-717u];
    if(index>=653u && index<717u)return release_words[index-653u];
    if(index>=605u && index<653u)return node_words[index-605u];
    if(index>=493u && index<565u)return wait_frames[(index-493u)/24u][(index-493u)%24u];
    if(index>=565u && index<605u)return wait_extra[index-565u];
    if(index>=461u && index<493u)return call_words[index-461u];
    if(index>=413u && index<437u)return prefix_words[index-413u];
    if(index>=437u && index<461u)return hash_words[index-437u];
    if(index>=198u && index<224u) return log_bridge_words[index-198u];
    if(index>=224u && index<250u) return second_log_bridge_words[index-224u];
    if(index>=250u && index<258u) return second_query_words[index-250u];
    if(index>=258u && index<276u) return title_return_words[index-258u];
    if(index>=276u && index<315u) return tick_words[index-276u];
    if(index>=315u && index<330u) return tick_extra_words[index-315u];
    if(index>=330u && index<359u) return third_log_bridge_words[index-330u];
    if(index>=359u && index<393u) return parent_setup_words[index-359u];
    if(index>=393u && index<413u) return parent_fill_words[index-393u];
    if (index>=185u && index<193u) return irql_words[index-185u];
    if (index==193u) return irql_imports;
    if (index==194u) return irql_pair.raise_count;
    if (index==195u) return irql_pair.lower_count;
    if (index==196u) return irql_pair.active;
    if (index==197u) return boot.count && boot.current<boot.count && boot.current<FF_BOOT_MAX_THREADS &&
        boot.threads[boot.current].fs<FF_GUEST_BYTES-0x24u ? guest_ram[boot.threads[boot.current].fs+0x24u]:UINT32_MAX;
    if (index<20u) return sdk_words[index];
    if (index>=180u && index<185u) return device_stos[index-180u];
    if (index>=164u && index<180u) return host_words[index-164u];
    if (index>=49u && index<164u) return MEM32(0x2cc1f0u+4u*(index-49u));
    if (index>=33u && index<41u) return MEM32(0x2bd620u+4u*(index-33u));
    switch(index) {
    case 44:return stos_attempts;
    case 45:return stos_completed;
    case 46:return stos_eax;
    case 47:return stos_ecx;
    case 48:return stos_edi;
    case 41:return setup_attempts;
    case 42:return setup_unexpected_returns;
    case 43:return MEM32(0x2cc1b0u);
    case 31:return init_attempts;
    case 32:return init_completed;
    case 29:return leaf_attempts;
    case 30:return leaf_completed;
    case 20:return launch_data_enabled;
    case 21:return 164u;
    case 22:return SDK_LAUNCH_DATA_SLOT;
    case 23:return SDK_LAUNCH_DATA_CELL;
    case 24:return launch_data_enabled?MEM32(SDK_LAUNCH_DATA_CELL):0u;
    case 25:return launch_data_original;
    case 26:return launch_data_enabled?MEM32(SDK_LAUNCH_DATA_SLOT):0u;
    case 27:return launch_data_disk;
    case 28:return launch_data_enabled?MEM32(FF_DISK_DATA_ADDRESS):0u;
    default:return 0;
    }
}
uint8_t *ff_sdk_shared_ram(void) { (void)sdk_thread(); return guest_ram; }
ff_virtual_memory *ff_sdk_shared_vm(void) { (void)sdk_thread(); return &virtual_memory; }
ff_boot_runtime *ff_sdk_shared_boot(void) { (void)sdk_thread(); return &boot; }
void ff_sdk_shared_stop(uint32_t target) { stop(2, target, 0); }
void ff_sdk_shared_read(ff_sdk_scalar *s)
{
    (void)sdk_thread();
    if (!s) stop(9, 0, 234);
    *s = (ff_sdk_scalar){g_eax,g_ecx,g_edx,g_ebx,g_esp,g_ebp,g_esi,g_edi,g_fs_base,(uint32_t)g_df};
}
void ff_sdk_shared_write(const ff_sdk_scalar *s)
{
    ff_boot_thread *t = sdk_thread();
    if (!s || s->fs_base != t->fs || s->df > 1u || s->esp < t->stack_low || s->esp > t->stack_top)
        stop(9, s ? s->esp : 0, 234);
    g_eax=s->eax; g_ecx=s->ecx; g_edx=s->edx; g_ebx=s->ebx; g_esp=s->esp;
    g_ebp=s->ebp; g_esi=s->esi; g_edi=s->edi; g_fs_base=s->fs_base; g_df=(int)s->df;
}
/* Stack/frame evidence only; arithmetic and memory operations stay original. */
static void sdk_prefix_enter(uint32_t guest,ff_sdk_scalar *scalar)
{
    ff_boot_thread *t=sdk_thread();
    const uint32_t main=guest==0x4d4f0u,second=sdk_invocation==1u,bytes=(main&&!second)?16u:8u;
    uint32_t *w=main?prefix_words:hash_words;
    if((sdk_enabled!=16u && (sdk_enabled!=17u && (sdk_enabled!=18u && (sdk_enabled!=19u && (sdk_enabled!=20u && (sdk_enabled!=21u && (sdk_enabled!=22u && (sdk_enabled!=23u && (sdk_enabled!=24u && (sdk_enabled!=25u && (sdk_enabled!=26u && (sdk_enabled!=27u && (sdk_enabled!=28u && (sdk_enabled!=29u && (sdk_enabled!=30u && (sdk_enabled!=31u && (sdk_enabled!=32u && (sdk_enabled!=33u && (sdk_enabled!=34u && (sdk_enabled!=35u && (sdk_enabled!=36u && (sdk_enabled!=37u && (sdk_enabled!=38u && (sdk_enabled!=39u && (sdk_enabled!=40u && (sdk_enabled!=41u && (sdk_enabled!=42u && (sdk_enabled!=43u && (sdk_enabled!=44u && sdk_enabled!=45u))))))))))))))))))))))))))))) || sdk_current_target!=0x48880u || w[0] || w[1] || g_df ||
       (g_esp&3u) || g_esp<t->stack_low || g_esp>t->stack_top ||
       bytes>t->stack_top-g_esp || ff_vm_check(&virtual_memory,g_esp,bytes,FF_VM_READ))stop(9,g_esp,guest);
    if(main && second) {
        if((sdk_enabled!=22u && (sdk_enabled!=23u && (sdk_enabled!=24u && (sdk_enabled!=25u && (sdk_enabled!=26u && (sdk_enabled!=27u && (sdk_enabled!=28u && (sdk_enabled!=29u && (sdk_enabled!=30u && (sdk_enabled!=31u && (sdk_enabled!=32u && (sdk_enabled!=33u && (sdk_enabled!=34u && (sdk_enabled!=35u && (sdk_enabled!=36u && (sdk_enabled!=37u && (sdk_enabled!=38u && (sdk_enabled!=39u && (sdk_enabled!=40u && (sdk_enabled!=41u && (sdk_enabled!=42u && (sdk_enabled!=43u && (sdk_enabled!=44u && sdk_enabled!=45u))))))))))))))))))))))) || tail_words[0]!=1u || tail_words[1] ||
           g_esp!=tail_words[4] || g_eax!=tail_words[7] || g_eax!=1u ||
           g_ebx!=parent_setup_words[4] || g_esi!=parent_setup_words[8] ||
           g_edi!=parent_setup_words[9] || g_ebp!=parent_setup_words[7] ||
           g_seh_ebp!=parent_setup_words[12] || g_fs_base!=invocation_evidence[0].continuation[39])stop(9,g_esp,guest);
        tail_words[1]=1u;
    }else if(main) {
        if(parent_fill_words[0]!=1u || parent_fill_words[1]!=1u || parent_setup_words[17]!=1u ||
           g_esp!=parent_fill_words[6]-4u || g_eax || g_ecx || g_edi!=0x2cc1b0u || hash_words[0])stop(9,g_esp,guest);
    } else if(prefix_words[0]!=1u || prefix_words[1] || g_esp!=prefix_words[2]-16u ||
              g_edi!=prefix_words[4] || g_ebx!=prefix_words[7] || g_esi!=prefix_words[8] ||
              g_ebp!=prefix_words[10] || g_seh_ebp!=prefix_words[12])stop(9,g_esp,guest);
    const uint32_t *frame=(const uint32_t *)ff_sdk_shared_memory(g_esp,bytes,FF_VM_READ,scalar);
    if(frame[0]!=(main?(second?0x4f7ffu:0x488adu):0x4d4fcu) || frame[1]!=(second?0x256410u:0x259470u) ||
       (main&&!second&&(frame[2]!=parent_setup_words[31] || frame[3]!=parent_setup_words[9])))stop(9,g_esp,guest);
    w[0]=1u;w[2]=g_esp;w[3]=frame[0];w[4]=frame[1];
    if(main&&!second){w[5]=frame[2];w[6]=frame[3];}
    w[7]=g_ebx;w[8]=g_esi;w[9]=g_edi;w[10]=g_ebp;w[11]=(uint32_t)g_df;
    w[12]=g_seh_ebp;w[22]=bytes;w[23]=g_eax;
}
static void sdk_prefix_exit(uint32_t guest,ff_sdk_scalar *scalar)
{
    ff_boot_thread *t=sdk_thread();uint32_t *w=guest==0x4d4f0u?prefix_words:hash_words;
    if((sdk_enabled!=16u && (sdk_enabled!=17u && (sdk_enabled!=18u && (sdk_enabled!=19u && (sdk_enabled!=20u && (sdk_enabled!=21u && (sdk_enabled!=22u && (sdk_enabled!=23u && (sdk_enabled!=24u && (sdk_enabled!=25u && (sdk_enabled!=26u && (sdk_enabled!=27u && (sdk_enabled!=28u && (sdk_enabled!=29u && (sdk_enabled!=30u && (sdk_enabled!=31u && (sdk_enabled!=32u && (sdk_enabled!=33u && (sdk_enabled!=34u && (sdk_enabled!=35u && (sdk_enabled!=36u && (sdk_enabled!=37u && (sdk_enabled!=38u && (sdk_enabled!=39u && (sdk_enabled!=40u && (sdk_enabled!=41u && (sdk_enabled!=42u && (sdk_enabled!=43u && (sdk_enabled!=44u && sdk_enabled!=45u))))))))))))))))))))))))))))) || sdk_current_target!=0x48880u || w[0]!=1u || w[1] ||
       g_esp!=w[2] || (g_esp&3u) || g_esp<t->stack_low || g_esp>t->stack_top ||
       4u>t->stack_top-g_esp || ff_vm_check(&virtual_memory,g_esp,4u,FF_VM_READ) ||
       g_ebx!=w[7] || g_esi!=w[8] || g_edi!=w[9] || g_ebp!=w[10] || g_df ||
       g_seh_ebp!=w[12] || (guest==0x4d4f0u&&hash_words[1]!=1u))stop(9,g_esp,guest);
    const uint32_t *frame=(const uint32_t *)ff_sdk_shared_memory(g_esp,4u,FF_VM_READ,scalar);
    if(frame[0]!=w[3])stop(9,g_esp,guest);
    w[13]=g_esp;w[14]=frame[0];w[15]=g_eax;w[16]=g_ebx;w[17]=g_esi;w[18]=g_edi;
    w[19]=g_ebp;w[20]=(uint32_t)g_df;w[21]=g_seh_ebp;w[1]=1u;
}
/* SCOPE09: retain the actual six-word caller; never fabricate a result. */
static void sdk_call_enter(ff_sdk_scalar *scalar)
{
    if(sdk_allocator_scope()){s43_call_enter(scalar);return;}
    ff_boot_thread *t=sdk_thread();
    if((sdk_enabled!=17u && (sdk_enabled!=18u && (sdk_enabled!=19u && (sdk_enabled!=20u && (sdk_enabled!=21u && (sdk_enabled!=22u && (sdk_enabled!=23u && (sdk_enabled!=24u && (sdk_enabled!=25u && (sdk_enabled!=26u && (sdk_enabled!=27u && (sdk_enabled!=28u && (sdk_enabled!=29u && (sdk_enabled!=30u && (sdk_enabled!=31u && (sdk_enabled!=32u && (sdk_enabled!=33u && (sdk_enabled!=34u && (sdk_enabled!=35u && (sdk_enabled!=36u && (sdk_enabled!=37u && (sdk_enabled!=38u && (sdk_enabled!=39u && (sdk_enabled!=40u && (sdk_enabled!=41u && (sdk_enabled!=42u && (sdk_enabled!=43u && (sdk_enabled!=44u && sdk_enabled!=45u)))))))))))))))))))))))))))) || (!sdk_scope42() && sdk_current_target!=0x48880u) || call_words[0] || call_words[1] ||
       prefix_words[0]!=1u || prefix_words[1] || hash_words[1]!=1u || g_df ||
       g_esp!=prefix_words[2]-36u || (g_esp&3u) || g_esp<t->stack_low || g_esp>t->stack_top ||
       28u>t->stack_top-g_esp || ff_vm_check(&virtual_memory,g_esp,28u,FF_VM_READ) ||
       g_ebx!=hash_words[15] || g_edi!=prefix_words[4] || g_esi!=prefix_words[8] ||
       g_ebp!=prefix_words[10] || g_seh_ebp!=prefix_words[12])stop(9,g_esp,0x4bf10u);
    const uint32_t *f=(const uint32_t *)ff_sdk_shared_memory(g_esp,28u,FF_VM_READ,scalar);
    const uint32_t expected[7]={0x4d539u,0x50u,0x25bfa8u,0x5bu,0x2001u,0x25bfccu,0u};
    for(uint32_t i=0;i<7u;++i)if(f[i]!=expected[i])stop(9,g_esp+4u*i,0x4bf10u);
    call_words[0]=1u;call_words[2]=g_esp;call_words[3]=f[0];
    for(uint32_t i=0;i<6u;++i)call_words[4u+i]=f[1u+i];
    call_words[10]=g_ebx;call_words[11]=g_esi;call_words[12]=g_edi;
    call_words[13]=g_ebp;call_words[14]=(uint32_t)g_df;call_words[15]=g_seh_ebp;
    call_words[25]=28u;call_words[26]=g_eax;call_words[27]=g_ecx;call_words[28]=g_edx;
}
static void sdk_call_exit(ff_sdk_scalar *scalar)
{
    if(sdk_allocator_scope()){s43_call_exit(scalar);return;}
    ff_boot_thread *t=sdk_thread();
    if((sdk_enabled!=17u && (sdk_enabled!=18u && (sdk_enabled!=19u && (sdk_enabled!=20u && (sdk_enabled!=21u && (sdk_enabled!=22u && (sdk_enabled!=23u && (sdk_enabled!=24u && (sdk_enabled!=25u && (sdk_enabled!=26u && (sdk_enabled!=27u && (sdk_enabled!=28u && (sdk_enabled!=29u && (sdk_enabled!=30u && (sdk_enabled!=31u && (sdk_enabled!=32u && (sdk_enabled!=33u && (sdk_enabled!=34u && (sdk_enabled!=35u && (sdk_enabled!=36u && (sdk_enabled!=37u && (sdk_enabled!=38u && (sdk_enabled!=39u && (sdk_enabled!=40u && (sdk_enabled!=41u && (sdk_enabled!=42u && (sdk_enabled!=43u && (sdk_enabled!=44u && sdk_enabled!=45u)))))))))))))))))))))))))))) || (!sdk_scope42() && sdk_current_target!=0x48880u) || call_words[0]!=1u || call_words[1] ||
       g_esp!=call_words[2] || (g_esp&3u) || g_esp<t->stack_low || g_esp>t->stack_top ||
       4u>t->stack_top-g_esp || ff_vm_check(&virtual_memory,g_esp,4u,FF_VM_READ) ||
       g_ebx!=call_words[10] || g_esi!=call_words[11] || g_edi!=call_words[12] ||
       g_ebp!=call_words[13] || g_df || g_seh_ebp!=call_words[15])stop(9,g_esp,0x4bf10u);
    const uint32_t *f=(const uint32_t *)ff_sdk_shared_memory(g_esp,4u,FF_VM_READ,scalar);
    if(f[0]!=call_words[3])stop(9,g_esp,0x4bf10u);
    call_words[16]=g_esp;call_words[17]=f[0];call_words[18]=g_eax;
    call_words[19]=g_ebx;call_words[20]=g_esi;call_words[21]=g_edi;call_words[22]=g_ebp;
    call_words[23]=(uint32_t)g_df;call_words[24]=g_seh_ebp;call_words[1]=1u;
}
/* Only the actual new4BF85 caller beneath outer48880; the old root is unchanged. */
static int sdk_nested_wait(void)
{ return (sdk_allocator_scope() && s43_call()->stage==S43_ALLOC) || (sdk_scope42() && a42[A42_STAGE]==2u) || (sdk_enabled>=18u && sdk_current_target==0x48880u); }
static const uint32_t *sdk_wait_frame(uint32_t bytes,ff_sdk_scalar *s)
{
    ff_boot_thread *t=sdk_thread();
    if((g_esp&3u) || g_esp<t->stack_low || g_esp>t->stack_top ||
       bytes>t->stack_top-g_esp || ff_vm_check(&virtual_memory,g_esp,bytes,FF_VM_READ))stop(9,g_esp,0x30943u);
    return (const uint32_t *)ff_sdk_shared_memory(g_esp,bytes,FF_VM_READ,s);
}
static uint32_t sdk_wait_object(uint32_t handle,uint32_t owner)
{
    (void)sdk_thread();
    if(!local_mutant_ready || !local_mutant_wait_enabled || handle!=0x8000cu ||
       local_mutants.handle_count>FF_LOCAL_MUTANT_CAPACITY || local_mutants.object_count>FF_LOCAL_MUTANT_CAPACITY)
        stop(9,handle,234u);
    uint32_t object=0,matches=0;
    for(uint32_t i=0;i<local_mutants.handle_count;++i)if(local_mutants.handles[i].handle==handle){
        ++matches;object=local_mutants.handles[i].object;
    }
    if(matches!=1u || object!=3u || object>local_mutants.object_count ||
       local_mutants.objects[object-1u].owner_thread!=owner)stop(9,handle,234u);
    return object;
}
static void sdk_wait_enter(uint32_t guest,ff_sdk_scalar *s)
{
    const uint32_t i=guest==0x30943u?0u:guest==0x3082du?1u:2u;
    const uint32_t bytes=i==1u?16u:12u;
    uint32_t *w=wait_frames[i];
    if(!sdk_nested_wait() || call_words[0]!=1u || call_words[1] ||
       prefix_words[0]!=1u || prefix_words[1] || w[0] || w[1] || g_df ||
       g_seh_ebp!=call_words[15])stop(9,g_esp,guest);
    const uint32_t *f=sdk_wait_frame(bytes,s);
    if(i==0u){
        if(wait_extra[W_STAGE] || g_esp!=call_words[2]-24u || f[0]!=0x4bf85u ||
           f[1]!=0x8000cu || f[2]!=UINT32_MAX || g_ebx!=(sdk_allocator_scope()?s43_request():0x50u) || g_edi!=(sdk_allocator_scope()?s43_total():0x80u) ||
           g_esi!=call_words[11] || g_ebp!=call_words[13] ||
           local_mutant_wait_attempts!=(sdk_allocator_scope()?s43_baseline(0u):(sdk_scope42()?sdk_scope42_wait_baseline(0u):sdk_invocation+1u)) || local_mutant_wait_completed!=(sdk_allocator_scope()?s43_baseline(1u):(sdk_scope42()?sdk_scope42_wait_baseline(1u):sdk_invocation+1u)) ||
           local_mutant_release_attempts!=(sdk_allocator_scope()?s43_baseline(2u):(sdk_scope42()?sdk_scope42_wait_baseline(2u):sdk_invocation+1u)) || local_mutant_release_completed!=(sdk_allocator_scope()?s43_baseline(3u):(sdk_scope42()?sdk_scope42_wait_baseline(3u):sdk_invocation+1u)))stop(9,g_esp,guest);
        const uint32_t handle=*(uint32_t *)ff_sdk_shared_memory(0x2cd794u,4u,FF_VM_READ,s);
        if(handle!=f[1])stop(9,handle,guest);
        wait_extra[W_GLOBAL]=handle;wait_extra[W_OBJECT]=sdk_wait_object(handle,0u);
        wait_extra[W_HANDLE_COUNT]=local_mutants.handle_count;wait_extra[W_OBJECT_COUNT]=local_mutants.object_count;
        wait_extra[W_ATTEMPTS_BEFORE]=local_mutant_wait_attempts;
        wait_extra[W_COMPLETED_BEFORE]=local_mutant_wait_completed;
        wait_extra[W_FS]=g_fs_base;
        wait_extra[W_STAGE]=1u;
    }else{
        const uint32_t *outer=wait_frames[0];
        if(wait_extra[W_STAGE]!=(i==1u?1u:2u) || g_esp!=outer[2]-(i==1u?16u:44u) ||
           f[0]!=(i==1u?0x30952u:0x3083fu) || f[2]!=UINT32_MAX ||
           g_ebx!=outer[7] || g_esi!=outer[8] || g_edi!=outer[9] ||
           g_ebp!=(i==1u?outer[10]:wait_frames[1][2]-4u))stop(9,g_esp,guest);
        if(i==1u){if(f[1]!=outer[4] || f[3])stop(9,g_esp,guest);}
        else{
            const uint32_t out=wait_frames[1][2]-12u;
            ff_boot_thread *t=sdk_thread();
            if(f[1]!=out || (out&3u) || out<t->stack_low || out>t->stack_top ||
               8u>t->stack_top-out || ff_vm_check(&virtual_memory,out,8u,FF_VM_READ|FF_VM_WRITE))stop(9,out,guest);
            const uint32_t *v=(const uint32_t *)ff_sdk_shared_memory(out,8u,FF_VM_READ,s);
            wait_extra[W_OUT]=out;wait_extra[W_OUT0]=v[0];wait_extra[W_OUT1]=v[1];
        }
        wait_extra[W_STAGE]=i+1u;
    }
    w[0]=1u;w[2]=g_esp;w[3]=f[0];w[4]=f[1];w[5]=f[2];if(i==1u)w[6]=f[3];
    w[7]=g_ebx;w[8]=g_esi;w[9]=g_edi;w[10]=g_ebp;w[11]=(uint32_t)g_df;
    w[12]=g_seh_ebp;w[22]=bytes;w[23]=g_eax;
}
static void sdk_wait_exit(uint32_t guest,ff_sdk_scalar *s)
{
    const uint32_t i=guest==0x30943u?0u:guest==0x3082du?1u:2u;
    uint32_t *w=wait_frames[i];
    const uint32_t stage=i==2u?3u:i==1u?5u:6u;
    if(!sdk_nested_wait() || wait_extra[W_STAGE]!=stage || w[0]!=1u || w[1] ||
       g_esp!=w[2] || g_eax || g_ebx!=w[7] || g_esi!=w[8] || g_edi!=w[9] ||
       g_ebp!=w[10] || g_df || g_seh_ebp!=w[12])stop(9,g_esp,guest);
    const uint32_t *f=sdk_wait_frame(w[22],s);
    if(f[0]!=w[3] || f[1]!=w[4] || f[2]!=w[5] || (i==1u && f[3]!=w[6]))stop(9,g_esp,guest);
    if(i==2u){
        const uint32_t *out=(const uint32_t *)ff_sdk_shared_memory(wait_extra[W_OUT],8u,FF_VM_READ,s);
        wait_extra[W_AFTER0]=out[0];wait_extra[W_AFTER1]=out[1];
        if(out[0]!=wait_extra[W_OUT0] || out[1]!=wait_extra[W_OUT1])stop(9,wait_extra[W_OUT],guest);
    }else if(sdk_wait_object(wait_frames[0][4],sdk_thread()->id)!=wait_extra[W_OBJECT])stop(9,g_esp,guest);
    w[13]=g_esp;w[14]=f[0];w[15]=g_eax;w[16]=g_ebx;w[17]=g_esi;w[18]=g_edi;
    w[19]=g_ebp;w[20]=(uint32_t)g_df;w[21]=g_seh_ebp;w[1]=1u;
    wait_extra[W_STAGE]=i==2u?4u:i==1u?6u:7u;
}
static void sdk_wait_kernel_before(ff_sdk_scalar *s)
{
    if(!sdk_nested_wait() || wait_extra[W_STAGE]!=4u || wait_extra[W_KERNEL_CALLS] ||
       wait_frames[2][1]!=1u || g_esp!=wait_frames[0][2]-52u || g_df ||
       g_ebx!=wait_frames[0][7] || g_esi || g_edi!=wait_frames[0][9] ||
       g_ebp!=wait_frames[1][2]-4u || g_seh_ebp!=call_words[15])stop(9,g_esp,234u);
    const uint32_t *f=sdk_wait_frame(20u,s);
    if(f[0]!=0x30850u || f[1]!=wait_frames[0][4] || f[2]!=1u || f[3] || f[4])stop(9,g_esp,234u);
    const uint32_t actual=*(uint32_t *)ff_sdk_shared_memory(0x2cd794u,4u,FF_VM_READ,s);
    if(actual!=f[1] || actual!=wait_extra[W_GLOBAL] || sdk_wait_object(actual,0u)!=wait_extra[W_OBJECT] ||
       local_mutant_wait_attempts!=wait_extra[W_ATTEMPTS_BEFORE] ||
       local_mutant_wait_completed!=wait_extra[W_COMPLETED_BEFORE])stop(9,g_esp,234u);
    wait_extra[W_KERNEL_CALLS]=1u;wait_extra[W_KERNEL_SP]=g_esp;wait_extra[W_KERNEL_RA]=f[0];
    wait_extra[W_HANDLE]=f[1];wait_extra[W_MODE]=f[2];wait_extra[W_ALERT]=f[3];wait_extra[W_TIMEOUT]=f[4];
    wait_extra[W_OWNER_BEFORE]=0u;wait_extra[W_EAX_BEFORE]=g_eax;
    wait_kernel_regs[0]=g_ebx;wait_kernel_regs[1]=g_esi;wait_kernel_regs[2]=g_edi;
    wait_kernel_regs[3]=g_ebp;wait_kernel_regs[4]=(uint32_t)g_df;wait_kernel_regs[5]=g_seh_ebp;
}
static void sdk_wait_kernel_after(ff_sdk_scalar *s)
{
    (void)s;
    if(!sdk_nested_wait() || wait_extra[W_STAGE]!=4u || wait_extra[W_KERNEL_CALLS]!=1u ||
       g_eax || g_esp!=wait_extra[W_KERNEL_SP]+20u || g_ebx!=wait_kernel_regs[0] ||
       g_esi!=wait_kernel_regs[1] || g_edi!=wait_kernel_regs[2] || g_ebp!=wait_kernel_regs[3] ||
       (uint32_t)g_df!=wait_kernel_regs[4] || g_seh_ebp!=wait_kernel_regs[5] ||
       g_fs_base!=wait_extra[W_FS] || local_mutant_wait_attempts!=wait_extra[W_ATTEMPTS_BEFORE]+1u ||
       local_mutant_wait_completed!=wait_extra[W_COMPLETED_BEFORE]+1u ||
       sdk_wait_object(wait_extra[W_HANDLE],sdk_thread()->id)!=wait_extra[W_OBJECT])stop(9,g_esp,234u);
    wait_extra[W_OWNER_AFTER]=sdk_thread()->id;wait_extra[W_EAX_AFTER]=g_eax;
    wait_extra[W_KERNEL_SP_AFTER]=g_esp;wait_extra[W_ATTEMPTS_AFTER]=local_mutant_wait_attempts;
    wait_extra[W_COMPLETED_AFTER]=local_mutant_wait_completed;wait_extra[W_STAGE]=5u;
}
static void sdk_wait_post(ff_sdk_scalar *s)
{
    if(!sdk_nested_wait() || wait_extra[W_STAGE]!=7u || wait_extra[W_POST_COUNT] ||
       wait_frames[0][1]!=1u || g_esp!=wait_frames[0][2]+12u || g_eax ||
       g_ebx!=wait_frames[0][7] || g_esi!=wait_frames[0][8] || g_edi!=wait_frames[0][9] ||
       g_ebp!=wait_frames[0][10] || g_df || g_seh_ebp!=call_words[15] ||
       sdk_wait_object(wait_frames[0][4],sdk_thread()->id)!=wait_extra[W_OBJECT])stop(9,g_esp,0x4bf85u);
    const uint32_t *saved=sdk_wait_frame(12u,s);
    if(saved[0]!=call_words[12] || saved[1]!=call_words[11] || saved[2]!=call_words[10])stop(9,g_esp,0x4bf85u);
    wait_extra[W_POST_COUNT]=1u;wait_extra[W_POST_SP]=g_esp;wait_extra[W_POST_TAG]=0x4bf85u;
    wait_extra[W_POST_EAX]=g_eax;wait_extra[W_POST_EBX]=g_ebx;wait_extra[W_POST_ESI]=g_esi;
    wait_extra[W_POST_EDI]=g_edi;wait_extra[W_POST_EBP]=g_ebp;wait_extra[W_POST_DF]=(uint32_t)g_df;
    wait_extra[W_POST_SEH]=g_seh_ebp;wait_extra[W_STAGE]=8u;
}
static const uint32_t *sdk_node_frame(uint32_t bytes,ff_sdk_scalar *s)
{
    ff_boot_thread *t=sdk_thread();
    if((g_esp&3u) || g_esp<t->stack_low || g_esp>t->stack_top ||
       bytes>t->stack_top-g_esp || ff_vm_check(&virtual_memory,g_esp,bytes,FF_VM_READ))stop(9,g_esp,0x4be40u);
    return (const uint32_t *)ff_sdk_shared_memory(g_esp,bytes,FF_VM_READ,s);
}
static void sdk_node_enter(ff_sdk_scalar *s)
{
    if((sdk_enabled!=19u && (sdk_enabled!=20u && (sdk_enabled!=21u && (sdk_enabled!=22u && (sdk_enabled!=23u && (sdk_enabled!=24u && (sdk_enabled!=25u && (sdk_enabled!=26u && (sdk_enabled!=27u && (sdk_enabled!=28u && (sdk_enabled!=29u && (sdk_enabled!=30u && (sdk_enabled!=31u && (sdk_enabled!=32u && (sdk_enabled!=33u && (sdk_enabled!=34u && (sdk_enabled!=35u && (sdk_enabled!=36u && (sdk_enabled!=37u && (sdk_enabled!=38u && (sdk_enabled!=39u && (sdk_enabled!=40u && (sdk_enabled!=41u && (sdk_enabled!=42u && (sdk_enabled!=43u && (sdk_enabled!=44u && sdk_enabled!=45u)))))))))))))))))))))))))) || (!sdk_allocator_scope() && !sdk_scope42() && sdk_current_target!=0x48880u) || node_words[0] || node_words[1] ||
       wait_extra[W_STAGE]!=8u || wait_extra[W_POST_COUNT]!=1u || call_words[0]!=1u || call_words[1] ||
       g_esp!=wait_extra[W_POST_SP]-8u || g_df || g_seh_ebp!=call_words[15] ||
       !g_eax || (g_eax&3u) || ff_vm_check(&virtual_memory,g_eax,0x20u,FF_VM_READ|FF_VM_WRITE) ||
       sdk_wait_object(wait_frames[0][4],sdk_thread()->id)!=wait_extra[W_OBJECT])stop(9,g_esp,0x4be40u);
    const uint32_t *f=sdk_node_frame(8u,s);
    if(f[0]!=0x4c048u || f[1]!=call_words[13])stop(9,g_esp,0x4be40u);
    node_words[0]=1u;node_words[2]=g_esp;node_words[3]=f[0];node_words[4]=f[1];
    node_words[7]=g_ebx;node_words[8]=g_esi;node_words[9]=g_edi;node_words[10]=g_ebp;
    node_words[11]=(uint32_t)g_df;node_words[12]=g_seh_ebp;node_words[22]=8u;node_words[23]=g_eax;
    node_words[34]=0x20u;node_words[37]=g_ecx;node_words[38]=g_edx;
}
static void sdk_node_exit(ff_sdk_scalar *s)
{
    if((sdk_enabled!=19u && (sdk_enabled!=20u && (sdk_enabled!=21u && (sdk_enabled!=22u && (sdk_enabled!=23u && (sdk_enabled!=24u && (sdk_enabled!=25u && (sdk_enabled!=26u && (sdk_enabled!=27u && (sdk_enabled!=28u && (sdk_enabled!=29u && (sdk_enabled!=30u && (sdk_enabled!=31u && (sdk_enabled!=32u && (sdk_enabled!=33u && (sdk_enabled!=34u && (sdk_enabled!=35u && (sdk_enabled!=36u && (sdk_enabled!=37u && (sdk_enabled!=38u && (sdk_enabled!=39u && (sdk_enabled!=40u && (sdk_enabled!=41u && (sdk_enabled!=42u && (sdk_enabled!=43u && (sdk_enabled!=44u && sdk_enabled!=45u)))))))))))))))))))))))))) || (!sdk_allocator_scope() && !sdk_scope42() && sdk_current_target!=0x48880u) || node_words[0]!=1u || node_words[1] ||
       g_esp!=node_words[2] || g_eax!=node_words[23] || g_ebx!=node_words[7] ||
       g_esi!=node_words[8] || g_edi!=node_words[9] || g_ebp!=node_words[10] || g_df ||
       g_seh_ebp!=node_words[12] ||
       sdk_wait_object(wait_frames[0][4],sdk_thread()->id)!=wait_extra[W_OBJECT])stop(9,g_esp,0x4be40u);
    const uint32_t *f=sdk_node_frame(8u,s);
    if(f[0]!=node_words[3] || f[1]!=node_words[4])stop(9,g_esp,0x4be40u);
    node_words[13]=g_esp;node_words[14]=f[0];node_words[15]=g_eax;node_words[16]=g_ebx;
    node_words[17]=g_esi;node_words[18]=g_edi;node_words[19]=g_ebp;
    node_words[20]=(uint32_t)g_df;node_words[21]=g_seh_ebp;node_words[41]=g_ecx;node_words[42]=g_edx;
    node_words[1]=1u;
}
static void sdk_node_post(ff_sdk_scalar *s)
{
    if((sdk_enabled!=19u && (sdk_enabled!=20u && (sdk_enabled!=21u && (sdk_enabled!=22u && (sdk_enabled!=23u && (sdk_enabled!=24u && (sdk_enabled!=25u && (sdk_enabled!=26u && (sdk_enabled!=27u && (sdk_enabled!=28u && (sdk_enabled!=29u && (sdk_enabled!=30u && (sdk_enabled!=31u && (sdk_enabled!=32u && (sdk_enabled!=33u && (sdk_enabled!=34u && (sdk_enabled!=35u && (sdk_enabled!=36u && (sdk_enabled!=37u && (sdk_enabled!=38u && (sdk_enabled!=39u && (sdk_enabled!=40u && (sdk_enabled!=41u && (sdk_enabled!=42u && (sdk_enabled!=43u && (sdk_enabled!=44u && sdk_enabled!=45u)))))))))))))))))))))))))) || (!sdk_allocator_scope() && !sdk_scope42() && sdk_current_target!=0x48880u) || node_words[0]!=1u || node_words[1]!=1u ||
       node_words[24] || g_esp!=node_words[2]+4u || g_eax!=node_words[23] ||
       g_ebx!=node_words[7] || g_esi!=node_words[8] || g_edi!=node_words[9] ||
       g_ebp!=node_words[10] || g_df || g_seh_ebp!=node_words[12] ||
       sdk_wait_object(wait_frames[0][4],sdk_thread()->id)!=wait_extra[W_OBJECT])stop(9,g_esp,0x4c048u);
    const uint32_t *saved=sdk_node_frame(4u,s);
    if(saved[0]!=node_words[4])stop(9,g_esp,0x4c048u);
    node_words[24]=1u;node_words[25]=g_esp;node_words[26]=0x4c048u;node_words[27]=g_eax;
    node_words[28]=g_ebx;node_words[29]=g_esi;node_words[30]=g_edi;node_words[31]=g_ebp;
    node_words[32]=(uint32_t)g_df;node_words[33]=g_seh_ebp;node_words[39]=g_ecx;node_words[40]=g_edx;
}
static int sdk_release_profile(void)
{return (sdk_allocator_scope() && s43_call()->stage==S43_ALLOC) || (sdk_scope42() && (a42[A42_STAGE]==2u || a42[A42_STAGE]==3u)) || ((sdk_enabled==20u || sdk_enabled==21u || sdk_enabled>=22u) && (sdk_scope42() || sdk_current_target==0x48880u));}
static const uint32_t *sdk_release_frame(uint32_t bytes,ff_sdk_scalar *s)
{
    ff_boot_thread *t=sdk_thread();
    if((g_esp&3u) || g_esp<t->stack_low || g_esp>t->stack_top ||
       bytes>t->stack_top-g_esp || ff_vm_check(&virtual_memory,g_esp,bytes,FF_VM_READ))stop(9,g_esp,0x3080du);
    return (const uint32_t *)ff_sdk_shared_memory(g_esp,bytes,FF_VM_READ,s);
}
static void sdk_release_enter(ff_sdk_scalar *s)
{
    if(sdk_allocator_scope()){s43_release_enter(s);return;}
    if(!sdk_release_profile() || (sdk_scope42() && a42[A42_STAGE]!=2u) || release_words[0] || release_words[1] || release_words[63] ||
       !local_mutant_release_enabled || node_words[24]!=1u || wait_extra[W_STAGE]!=8u ||
       call_words[0]!=1u || call_words[1] || g_esp!=call_words[2]-20u || g_df ||
       g_ebp!=call_words[13] || g_seh_ebp!=call_words[15] || g_ebx!=node_words[7] ||
       g_esi!=node_words[8] || g_edi!=node_words[9] || local_mutant_release_attempts!=(sdk_scope42()?sdk_scope42_wait_baseline(2u):sdk_invocation+1u) ||
       local_mutant_release_completed!=(sdk_scope42()?sdk_scope42_wait_baseline(3u):sdk_invocation+1u) || local_mutant_wait_attempts!=wait_extra[W_ATTEMPTS_AFTER] ||
       local_mutant_wait_completed!=wait_extra[W_COMPLETED_AFTER])stop(9,g_esp,0x3080du);
    const uint32_t *f=sdk_release_frame(8u,s);
    if(f[0]!=0x4c0dau || f[1]!=0x8000cu || f[1]!=wait_extra[W_HANDLE])stop(9,g_esp,0x3080du);
    const uint32_t handle=*(uint32_t *)ff_sdk_shared_memory(0x2cd794u,4u,FF_VM_READ,s);
    if(handle!=f[1] || sdk_wait_object(handle,sdk_thread()->id)!=wait_extra[W_OBJECT])stop(9,handle,221u);
    release_words[0]=1u;release_words[2]=g_esp;release_words[3]=f[0];release_words[4]=f[1];
    release_words[5]=g_eax;release_words[6]=g_ebx;release_words[7]=g_esi;release_words[8]=g_edi;
    release_words[9]=g_ebp;release_words[10]=(uint32_t)g_df;release_words[11]=g_seh_ebp;
    release_words[17]=wait_extra[W_OBJECT];release_words[23]=local_mutant_release_attempts;
    release_words[24]=local_mutant_release_completed;release_words[27]=handle;release_words[62]=g_fs_base;
    release_words[63]=1u;
}
static void sdk_release_kernel_before(ff_sdk_scalar *s)
{
    if(!sdk_release_profile() || (sdk_scope42() && a42[A42_STAGE]!=2u) || release_words[63]!=1u || release_words[12] ||
       g_esp!=release_words[2]-12u || g_ebx!=release_words[6] || g_esi!=release_words[7] ||
       g_edi!=release_words[8] || g_ebp!=release_words[9] || g_df || g_seh_ebp!=release_words[11] ||
       local_mutant_release_attempts!=release_words[23] || local_mutant_release_completed!=release_words[24])
        stop(9,g_esp,221u);
    const uint32_t *f=sdk_release_frame(12u,s);
    const uint32_t handle=*(uint32_t *)ff_sdk_shared_memory(0x2cd794u,4u,FF_VM_READ,s);
    if(f[0]!=0x30819u || f[1]!=release_words[4] || f[2] || handle!=f[1] ||
       sdk_wait_object(handle,sdk_thread()->id)!=release_words[17])stop(9,g_esp,221u);
    release_words[12]=1u;release_words[13]=g_esp;release_words[14]=f[0];release_words[15]=f[1];
    release_words[16]=f[2];release_words[18]=local_mutants.objects[release_words[17]-1u].owner_thread;
    release_words[20]=g_eax;
}
static void sdk_release_kernel_after(void)
{
    if(!sdk_release_profile() || (sdk_scope42() && a42[A42_STAGE]!=2u) || release_words[63]!=1u || release_words[12]!=1u || g_eax ||
       g_esp!=release_words[13]+12u || g_ebx!=release_words[6] || g_esi!=release_words[7] ||
       g_edi!=release_words[8] || g_ebp!=release_words[9] || g_df || g_seh_ebp!=release_words[11] ||
       g_fs_base!=release_words[62] || local_mutant_release_attempts!=release_words[23]+1u ||
       local_mutant_release_completed!=release_words[24]+1u ||
       sdk_wait_object(release_words[15],0u)!=release_words[17])stop(9,g_esp,221u);
    release_words[19]=local_mutants.objects[release_words[17]-1u].owner_thread;
    release_words[21]=g_eax;release_words[22]=g_esp;release_words[25]=local_mutant_release_attempts;
    release_words[26]=local_mutant_release_completed;release_words[63]=2u;
}
static void sdk_release_exit(ff_sdk_scalar *s)
{
    if(!sdk_release_profile() || (sdk_scope42() && a42[A42_STAGE]!=2u) || release_words[63]!=2u || release_words[0]!=1u || release_words[1] ||
       g_esp!=release_words[2] || g_eax!=1u || g_ebx!=release_words[6] || g_esi!=release_words[7] ||
       g_edi!=release_words[8] || g_ebp!=release_words[9] || g_df || g_seh_ebp!=release_words[11] ||
       sdk_wait_object(release_words[4],0u)!=release_words[17])stop(9,g_esp,0x3080du);
    const uint32_t *f=sdk_release_frame(8u,s);
    if(f[0]!=release_words[3] || f[1]!=release_words[4])stop(9,g_esp,0x3080du);
    release_words[1]=1u;release_words[28]=g_esp;release_words[29]=f[0];release_words[30]=g_eax;
    release_words[31]=g_ebx;release_words[32]=g_esi;release_words[33]=g_edi;release_words[34]=g_ebp;
    release_words[35]=(uint32_t)g_df;release_words[36]=g_seh_ebp;release_words[63]=3u;
}
static void sdk_release_post(ff_sdk_scalar *s)
{
    if(!sdk_release_profile() || (sdk_scope42() && a42[A42_STAGE]!=2u) || release_words[63]!=3u || release_words[1]!=1u || release_words[37] ||
       g_esp!=release_words[2]+8u || g_eax!=1u || g_ebx!=release_words[6] || g_esi!=release_words[7] ||
       g_edi!=release_words[8] || g_ebp!=release_words[9] || g_df || g_seh_ebp!=release_words[11] ||
       sdk_wait_object(release_words[4],0u)!=release_words[17])stop(9,g_esp,0x4c0dau);
    const uint32_t *f=sdk_release_frame(12u,s);
    if(f[0]!=call_words[12] || f[1]!=call_words[11] || f[2]!=call_words[10])stop(9,g_esp,0x4c0dau);
    release_words[37]=1u;release_words[38]=g_esp;release_words[39]=sdk_allocator_scope()?s43_call()->release_ra:0x4c0dau;release_words[40]=g_eax;
    release_words[41]=g_ebx;release_words[42]=g_esi;release_words[43]=g_edi;release_words[44]=g_ebp;
    release_words[45]=(uint32_t)g_df;release_words[46]=g_seh_ebp;release_words[63]=4u;
}
static void sdk_pointer_post(ff_sdk_scalar *s)
{
    if(sdk_scope43()){s43_pointer_post(s);return;}
    if(!sdk_release_profile() || (sdk_scope42() && a42[A42_STAGE]!=3u) || release_words[63]!=4u || release_words[47] || call_words[1]!=1u ||
       g_esp!=call_words[2]+4u || !g_eax || g_eax!=call_words[18] || g_ebx!=call_words[10] ||
       g_esi!=call_words[11] || g_edi!=call_words[12] || g_ebp!=call_words[13] || g_df ||
       g_seh_ebp!=call_words[15] || ff_vm_check(&virtual_memory,g_eax,0x50u,FF_VM_READ|FF_VM_WRITE) ||
       sdk_wait_object(release_words[4],0u)!=release_words[17])stop(9,g_esp,0x4d539u);
    const uint32_t *args=sdk_release_frame(24u,s);
    for(uint32_t i=0;i<6u;++i){
        if(args[i]!=call_words[4u+i])stop(9,g_esp+4u*i,0x4d539u);
        release_words[52u+i]=args[i];
    }
    if(sdk_scope42())sdk_scope42_pointer_proof(s);
    release_words[47]=1u;release_words[48]=g_esp;release_words[49]=0x4d539u;release_words[50]=g_eax;
    release_words[51]=call_words[1];release_words[58]=0x50u;release_words[59]=6u;
    release_words[60]=local_mutants.objects[release_words[17]-1u].owner_thread;
    release_words[61]=24u;release_words[63]=5u;
}
static const uint32_t *sdk_continuation_frame(uint32_t bytes,uint32_t tag,ff_sdk_scalar *s)
{
    ff_boot_thread *t=sdk_thread();
    if((g_esp&3u) || g_esp<t->stack_low || g_esp>t->stack_top ||
       bytes>t->stack_top-g_esp || ff_vm_check(&virtual_memory,g_esp,bytes,FF_VM_READ))stop(9,g_esp,tag);
    return (const uint32_t *)ff_sdk_shared_memory(g_esp,bytes,FF_VM_READ,s);
}
static void sdk_continuation_observe(uint32_t tag,ff_sdk_scalar *s)
{
    if(sdk_scope43()){if(tag!=0x4d555u)stop(9,tag,43u);s43_continuation(s);return;}
    uint32_t *w=continuation_words;
    if(sdk_scope42() && tag==0x4d555u) sdk_scope42_prewrite(s);
    if((sdk_enabled!=21u && (sdk_enabled!=22u && (sdk_enabled!=23u && (sdk_enabled!=24u && (sdk_enabled!=25u && (sdk_enabled!=26u && (sdk_enabled!=27u && (sdk_enabled!=28u && (sdk_enabled!=29u && (sdk_enabled!=30u && (sdk_enabled!=31u && (sdk_enabled!=32u && (sdk_enabled!=33u && (sdk_enabled!=34u && (sdk_enabled!=35u && (sdk_enabled!=36u && (sdk_enabled!=37u && (sdk_enabled!=38u && (sdk_enabled!=39u && (sdk_enabled!=40u && (sdk_enabled!=41u && (sdk_enabled!=42u && (sdk_enabled!=43u && (sdk_enabled!=44u && sdk_enabled!=45u)))))))))))))))))))))))) || (!sdk_scope42() && sdk_current_target!=0x48880u) || release_words[63]!=5u ||
       call_words[1]!=1u || hash_words[1]!=1u || g_df || g_ebp!=prefix_words[10] ||
       g_seh_ebp!=prefix_words[12] || g_fs_base!=release_words[62])stop(9,g_esp,tag);
    if(tag==0x4d555u){
        if(w[0] || w[1] || w[2] || w[3] || prefix_words[0]!=1u || prefix_words[1] ||
           g_esp!=prefix_words[2]-8u || g_eax!=call_words[18] || !g_eax ||
           g_ebx!=hash_words[15] || g_esi!=prefix_words[8] || g_edi!=prefix_words[4] ||
           ff_vm_check(&virtual_memory,g_eax,0x50u,FF_VM_READ|FF_VM_WRITE))stop(9,g_esp,tag);
        const uint32_t *f=sdk_continuation_frame(16u,tag,s);
        if(f[0]!=prefix_words[9] || f[1]!=prefix_words[7] || f[2]!=prefix_words[3] ||
           f[3]!=prefix_words[4])stop(9,g_esp,tag);
        w[0]=1u;w[3]=1u;w[4]=g_esp;
        for(uint32_t i=0;i<4u;++i)w[5u+i]=f[i];
        w[9]=g_eax;w[10]=g_ebx;w[11]=g_esi;w[12]=g_edi;w[13]=g_ebp;
        w[14]=(uint32_t)g_df;w[15]=g_seh_ebp;w[16]=g_fs_base;
    }else if(tag==0x488adu){
        if(w[0]!=1u || w[1] || w[2] || w[3]!=1u || prefix_words[1]!=1u ||
           g_esp!=prefix_words[2]+4u || g_eax!=1u || prefix_words[15]!=1u ||
           g_ebx!=prefix_words[7] || g_esi!=prefix_words[8] || g_edi!=prefix_words[9])stop(9,g_esp,tag);
        const uint32_t *f=sdk_continuation_frame(12u,tag,s);
        if(f[0]!=prefix_words[4] || f[1]!=prefix_words[5] || f[2]!=prefix_words[6])stop(9,g_esp,tag);
        w[1]=1u;w[3]=2u;w[17]=g_esp;w[18]=g_eax;
        for(uint32_t i=0;i<3u;++i)w[19u+i]=f[i];
        w[22]=g_ebx;w[23]=g_esi;w[24]=g_edi;w[25]=g_ebp;w[26]=(uint32_t)g_df;
        w[27]=g_seh_ebp;w[28]=g_fs_base;
    }else if(tag==0x488b9u){
        if(w[0]!=1u || w[1]!=1u || w[2] || w[3]!=2u || prefix_words[1]!=1u ||
           g_esp!=parent_setup_words[2] || g_esp!=prefix_words[2]+16u || g_eax!=1u ||
           g_ebx!=parent_setup_words[4] || g_esi!=parent_setup_words[8] ||
           g_edi!=parent_setup_words[9])stop(9,g_esp,tag);
        const uint32_t *f=sdk_continuation_frame(8u,tag,s);
        if(f[0]!=parent_setup_words[1] || f[0]!=0x4f7ffu || f[1]!=0x256410u)stop(9,g_esp,tag);
        w[2]=1u;w[3]=3u;w[29]=g_esp;w[30]=f[0];w[31]=f[1];w[32]=g_eax;
        w[33]=g_ebx;w[34]=g_esi;w[35]=g_edi;w[36]=g_ebp;w[37]=(uint32_t)g_df;
        w[38]=g_seh_ebp;w[39]=g_fs_base;
        if(sdk_enabled>=22u){
            if(sdk_invocation || tail_words[0] || local_mutant_wait_attempts!=2u ||
               local_mutant_wait_completed!=2u || local_mutant_release_attempts!=2u ||
               local_mutant_release_completed!=2u || sdk_wait_object(0x8000cu,0u)!=3u)stop(9,g_esp,tag);
            tail_words[0]=1u;tail_words[4]=g_esp;tail_words[5]=f[0];tail_words[6]=f[1];
            tail_words[7]=g_eax;tail_words[8]=local_mutant_wait_completed;
            tail_words[9]=local_mutant_release_completed;tail_words[14]=2u;
            sdk_invocation=1u;
        }

    }else stop(9,g_esp,tag);
}
static void sdk_tail_exit(ff_sdk_scalar *s)
{
    if((sdk_enabled!=22u && (sdk_enabled!=23u && (sdk_enabled!=24u && (sdk_enabled!=25u && (sdk_enabled!=26u && (sdk_enabled!=27u && (sdk_enabled!=28u && (sdk_enabled!=29u && (sdk_enabled!=30u && (sdk_enabled!=31u && (sdk_enabled!=32u && (sdk_enabled!=33u && (sdk_enabled!=34u && (sdk_enabled!=35u && (sdk_enabled!=36u && (sdk_enabled!=37u && (sdk_enabled!=38u && (sdk_enabled!=39u && (sdk_enabled!=40u && (sdk_enabled!=41u && (sdk_enabled!=42u && (sdk_enabled!=43u && (sdk_enabled!=44u && sdk_enabled!=45u))))))))))))))))))))))) || sdk_invocation!=1u || tail_words[0]!=1u || tail_words[1]!=1u ||
       tail_words[2] || prefix_words[0]!=1u || prefix_words[1]!=1u || hash_words[1]!=1u ||
       g_esp!=parent_setup_words[2] || g_eax!=1u || g_ebx!=parent_setup_words[4] ||
       g_esi!=parent_setup_words[8] || g_edi!=parent_setup_words[9] || g_ebp!=parent_setup_words[7] ||
       g_df || g_seh_ebp!=parent_setup_words[12] || g_fs_base!=invocation_evidence[0].continuation[39])stop(9,g_esp,0x48880u);
    const uint32_t *f=sdk_continuation_frame(8u,0x48880u,s);
    if(f[0]!=tail_words[5] || f[1]!=tail_words[6] || f[0]!=0x4f7ffu ||
       sdk_wait_object(0x8000cu,0u)!=3u)stop(9,g_esp,0x48880u);
    tail_words[2]=1u;tail_words[10]=g_eax;tail_words[15]=g_esp;
    /* Tail callee and its caller share this single real pre-RET frame. */
    recomp_trace_exit("sdk-shared-tail",0x48880u);
}
static void sdk_startup_enter(uint32_t guest,ff_sdk_scalar *s)
{
    uint32_t *w=startup_words;
    if((sdk_enabled!=23u && (sdk_enabled!=24u && (sdk_enabled!=25u && (sdk_enabled!=26u && (sdk_enabled!=27u && (sdk_enabled!=28u && (sdk_enabled!=29u && (sdk_enabled!=30u && (sdk_enabled!=31u && (sdk_enabled!=32u && (sdk_enabled!=33u && (sdk_enabled!=34u && (sdk_enabled!=35u && (sdk_enabled!=36u && (sdk_enabled!=37u && (sdk_enabled!=38u && (sdk_enabled!=39u && (sdk_enabled!=40u && (sdk_enabled!=41u && (sdk_enabled!=42u && (sdk_enabled!=43u && (sdk_enabled!=44u && sdk_enabled!=45u)))))))))))))))))))))) || sdk_current_target!=0x4f5e0u || w[49]!=1u || g_df ||
       g_ebx!=parent_setup_words[4] || g_esi!=parent_setup_words[8] ||
       g_ebp!=parent_setup_words[7] || g_seh_ebp!=parent_setup_words[12] ||
       g_fs_base!=invocation_evidence[0].continuation[39])stop(9,g_esp,guest);
    const uint32_t *f=sdk_continuation_frame(4u,guest,s);
    if(guest==0x4f5e0u){
        if(w[0] || w[13] || w[48] || g_esp!=w[50] || f[0]!=0x4f807u ||
           g_eax!=1u || g_edi!=parent_setup_words[9])stop(9,g_esp,guest);
        w[0]=1u;w[2]=g_esp;w[3]=f[0];w[4]=g_eax;w[5]=g_ebx;w[6]=g_esi;
        w[7]=g_edi;w[8]=g_ebp;w[9]=(uint32_t)g_df;w[10]=g_seh_ebp;w[11]=g_fs_base;w[12]=4u;
    }else if(guest==0x4eb50u){
        if(w[0]!=1u || w[13] || w[48]!=4u || w[25]!=1u || w[37]!=1u ||
           g_esp!=w[2]-0x20u || f[0]!=0x4f603u || g_eax || g_ecx || g_edi!=0x2d15e4u)stop(9,g_esp,guest);
        w[13]=1u;w[14]=g_esp;w[15]=f[0];w[16]=g_eax;w[17]=g_ebx;w[18]=g_esi;
        w[19]=g_edi;w[20]=g_ebp;w[21]=(uint32_t)g_df;w[22]=g_seh_ebp;w[23]=g_fs_base;
    }else stop(9,g_esp,guest);
}
static void sdk_startup_stos(uint32_t complete)
{
    uint32_t *w=startup_words;ff_sdk_scalar s;ff_sdk_shared_read(&s);
    if((sdk_enabled!=23u && (sdk_enabled!=24u && (sdk_enabled!=25u && (sdk_enabled!=26u && (sdk_enabled!=27u && (sdk_enabled!=28u && (sdk_enabled!=29u && (sdk_enabled!=30u && (sdk_enabled!=31u && (sdk_enabled!=32u && (sdk_enabled!=33u && (sdk_enabled!=34u && (sdk_enabled!=35u && (sdk_enabled!=36u && (sdk_enabled!=37u && (sdk_enabled!=38u && (sdk_enabled!=39u && (sdk_enabled!=40u && (sdk_enabled!=41u && (sdk_enabled!=42u && (sdk_enabled!=43u && (sdk_enabled!=44u && sdk_enabled!=45u)))))))))))))))))))))) || sdk_current_target!=0x4f5e0u || w[0]!=1u || w[13] ||
       complete>1u || w[48]>3u || g_esp!=w[2]-0x1cu || g_eax || g_df ||
       g_ebx!=w[5] || g_esi!=w[6] || g_ebp!=w[8] || g_seh_ebp!=w[10] || g_fs_base!=w[11])stop(9,g_esp,0x4f5e0u);
    const uint32_t i=w[48]/2u,base=24u+12u*i,start=i?0x2d15b8u:0x2d15e8u;
    const uint32_t count=i?11u:0x232eu,bytes=count*4u;
    if(!complete){
        if(w[48]!=2u*i || w[base] || w[base+1u] || g_edi!=start || g_ecx!=count ||
           ff_vm_check(&virtual_memory,start,bytes,FF_VM_READ|FF_VM_WRITE))stop(9,start,0x4f5e0u);
        w[base]=1u;w[base+2u]=start;w[base+3u]=count;w[base+4u]=4u;w[base+5u]=bytes;
        w[base+6u]=g_esp;w[base+7u]=g_eax;w[48]=2u*i+1u;
    }else{
        if(w[48]!=2u*i+1u || w[base]!=1u || w[base+1u] || g_ecx ||
           g_edi!=start+bytes || ff_vm_check(&virtual_memory,start,bytes,FF_VM_READ))stop(9,start,0x4f5e0u);
        const uint32_t *v=(const uint32_t *)ff_sdk_shared_memory(start,bytes,FF_VM_READ,&s);
        for(uint32_t n=0;n<count;++n)if(v[n])stop(9,start+4u*n,0x4f5e0u);
        w[base+1u]=1u;w[base+8u]=g_edi;w[base+9u]=g_ecx;w[base+10u]=count;
        w[48]=2u*i+2u;w[base+11u]=w[48];
    }
}
static int sdk_directory_profile(void)
{return sdk_enabled>=24u && sdk_current_target==0x4f5e0u;}
static int sdk_directory_root_profile(void)
{return (sdk_enabled==25u || sdk_enabled==26u || sdk_enabled==27u || (sdk_enabled==28u || (sdk_enabled==29u || (sdk_enabled==30u || (sdk_enabled==31u || (sdk_enabled==32u || (sdk_enabled==33u || (sdk_enabled==34u || (sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u)))))))))))))))))) && sdk_current_target==0x4f5e0u;}
static void sdk_directory_enter(ff_sdk_scalar *s)
{
    uint32_t *w=directory_words;
    if(!sdk_directory_profile() || w[0] || w[1] || startup_words[13]!=1u || startup_words[48]!=4u ||
       g_esp!=startup_words[14]-0x204u || g_df || g_ebx!=startup_words[17] ||
       g_esi!=startup_words[18] || g_edi!=startup_words[19] || g_ebp!=startup_words[20] ||
       g_seh_ebp!=startup_words[22] || g_fs_base!=startup_words[23])stop(9,g_esp,0x1e95dbu);
    const uint32_t *f=sdk_continuation_frame(12u,0x1e95dbu,s);
    if(f[0]!=0x4eb66u || f[1]!=0x25c4a8u || f[2]!=g_esp+0x44u || g_eax!=f[2])stop(9,g_esp,0x1e95dbu);
    w[0]=1u;w[2]=g_esp;w[3]=f[0];w[4]=f[1];w[5]=f[2];w[6]=g_eax;
    w[7]=g_ebx;w[8]=g_esi;w[9]=g_edi;w[10]=g_ebp;w[11]=(uint32_t)g_df;
    w[12]=g_seh_ebp;w[13]=g_fs_base;w[14]=12u;w[15]=1u;
}
static void sdk_directory_ansi_before(ff_sdk_scalar *s)
{
    uint32_t *w=directory_words;
    if(!sdk_directory_profile() || w[0]!=1u || w[15]!=1u || w[16] || g_esp!=w[2]-0x184u ||
       g_ebp!=w[2]-4u || g_ebx!=w[7] || g_esi!=w[8] || g_edi!=w[9] || g_df ||
       g_fs_base!=w[13] || g_seh_ebp!=w[12])stop(9,g_esp,289u);
    const uint32_t *f=sdk_continuation_frame(12u,289u,s);
    if(f[0]!=0x1e95f3u || f[1]!=g_ebp-8u || f[2]!=w[4] || g_eax!=f[1] ||
       ff_vm_check(&virtual_memory,f[1],8u,FF_VM_READ|FF_VM_WRITE))stop(9,g_esp,289u);
    w[16]=1u;w[17]=g_esp;w[18]=f[0];w[19]=f[1];w[20]=f[2];w[21]=g_eax;
    w[22]=g_ebp;w[23]=12u;w[15]=2u;
}
static void sdk_directory_ansi_after(ff_sdk_scalar *s)
{
    uint32_t *w=directory_words;
    if(!sdk_directory_profile() || w[15]!=2u || w[16]!=1u || w[24] || g_esp!=w[17]+12u ||
       g_eax!=w[21] || g_ebp!=w[22] || g_ebx!=w[7] || g_esi!=w[8] || g_edi!=w[9] ||
       g_df || g_fs_base!=w[13] || g_seh_ebp!=w[12])stop(9,g_esp,289u);
    const uint8_t *d=ff_sdk_shared_memory(w[19],8u,FF_VM_READ,s);
    const uint32_t length=(uint32_t)d[0]|((uint32_t)d[1]<<8),maximum=(uint32_t)d[2]|((uint32_t)d[3]<<8);
    const uint32_t buffer=(uint32_t)d[4]|((uint32_t)d[5]<<8)|((uint32_t)d[6]<<16)|((uint32_t)d[7]<<24);
    if(length>=65535u || maximum!=length+1u || buffer!=w[20])stop(9,w[19],289u);
    w[24]=1u;w[25]=g_esp;w[26]=g_eax;w[27]=g_ebp;w[28]=length;w[29]=maximum;
    w[30]=buffer;w[48]=8u;w[15]=3u;
}
static void sdk_directory_open_stop(ff_sdk_scalar *s)
{
    uint32_t *w=directory_words;
    if(!sdk_directory_profile() || w[15]!=3u || w[24]!=1u || w[32] ||
       g_esp!=w[2]-0x194u || g_ebp!=w[22] || g_ebx!=w[7] || g_esi!=w[8] || g_edi ||
       g_df || g_fs_base!=w[13] || g_seh_ebp!=w[12])stop(9,g_esp,202u);
    const uint32_t *f=sdk_continuation_frame(28u,202u,s);
    if(f[0]!=0x1e9683u || f[1]!=g_ebp+8u || f[2]!=0x100001u || f[3]!=g_ebp-0x1cu ||
       f[4]!=g_ebp-0x24u || f[5]!=3u || f[6]!=0x4021u || g_eax!=f[1] ||
       ff_vm_check(&virtual_memory,f[1],4u,FF_VM_WRITE) || ff_vm_check(&virtual_memory,f[4],8u,FF_VM_WRITE))stop(9,g_esp,202u);
    const uint32_t *attrs=(const uint32_t *)ff_sdk_shared_memory(f[3],12u,FF_VM_READ,s);
    if(attrs[0]!=0xfffffffdu || attrs[1]!=g_ebp-8u || attrs[2]!=0x40u)stop(9,f[3],202u);
    uint32_t token=0,found=0;
    for(uint32_t i=0;i<FF_KERNEL_BINDING_COUNT;++i)if(bindings[i].ordinal==202u){
        if(++found!=1u || bindings[i].slot!=0x2533d0u)stop(9,0x2533d0u,202u);
        token=FF_KERNEL_BASE+4u*i;
    }
    if(found!=1u || *(uint32_t *)ff_probe_guest_memory(0x2533d0u,4u,FF_VM_READ,0x1e967du)!=token)stop(9,0x2533d0u,202u);
    w[32]=1u;w[33]=g_esp;w[34]=f[0];w[35]=0x2533d0u;w[36]=token;
    for(uint32_t i=0;i<6u;++i)w[37u+i]=f[1u+i];
    w[43]=g_ebp;w[44]=g_eax;w[45]=(uint32_t)g_df;w[46]=g_seh_ebp;w[47]=g_fs_base;
    w[49]=attrs[0];w[50]=attrs[1];w[51]=attrs[2];w[15]=4u;
    if(sdk_enabled==24u)stop(1,token,202u); /* exact MODE24 stop */
    if(!sdk_directory_root_profile() || g_esp!=0x03c10ac4u ||
       f[1]!=0x03c10c5cu || f[2]!=0x00100001u || f[3]!=0x03c10c38u ||
       f[4]!=0x03c10c30u || f[5]!=3u || f[6]!=0x4021u)
        stop(9,g_esp,202u);
    /* The existing C service owns checked pointers, overlap, alias, mount,
       sharing, allocation and IOSB semantics. Record only the observed frame. */
    w[52]=f[1];w[53]=f[3];w[54]=f[4];w[56]=f[1];w[57]=f[4];
    w[58]=g_ebx;w[59]=g_esi;w[60]=g_edi;w[61]=g_ebp;w[62]=(uint32_t)g_df;w[63]=g_fs_base;
}
static void sdk_directory_open_after(ff_sdk_scalar *s)
{
    uint32_t *w=directory_words;
    if(!sdk_directory_root_profile() || w[15]!=4u || !w[32] ||
       g_esp!=w[33]+28u || g_ebx!=w[58] || g_esi!=w[59] || g_edi!=w[60] || g_ebp!=w[61] ||
       (uint32_t)g_df!=w[62] || g_fs_base!=w[63] || g_seh_ebp!=w[46])stop(9,g_esp,202u);
    const uint8_t *hb=ff_sdk_shared_memory(w[56],4u,FF_VM_READ,s);
    const uint8_t *ib=ff_sdk_shared_memory(w[57],8u,FF_VM_READ,s);
    const uint32_t handle=(uint32_t)hb[0]|((uint32_t)hb[1]<<8)|((uint32_t)hb[2]<<16)|((uint32_t)hb[3]<<24);
    const uint32_t status=(uint32_t)ib[0]|((uint32_t)ib[1]<<8)|((uint32_t)ib[2]<<16)|((uint32_t)ib[3]<<24);
    const uint32_t information=(uint32_t)ib[4]|((uint32_t)ib[5]<<8)|((uint32_t)ib[6]<<16)|((uint32_t)ib[7]<<24);
    if(g_eax!=status || (status==0u && (handle<0x00070000u || handle>=0x00080000u || information!=1u)))stop(9,w[57],202u);
    w[15]=5u;w[52]=status;w[53]=handle;w[54]=information;
}
static int sdk_query_frame_overlap(uint32_t address,uint32_t bytes)
{
    return bytes && (uint64_t)address<(uint64_t)g_esp+44u &&
        (uint64_t)g_esp<(uint64_t)address+bytes;
}
static void sdk_directory_query_first(ff_sdk_scalar *s)
{
    uint32_t *w=query_words;
    if((sdk_enabled!=26u && (sdk_enabled!=27u && (sdk_enabled!=28u && (sdk_enabled!=29u && (sdk_enabled!=30u && (sdk_enabled!=31u && (sdk_enabled!=32u && (sdk_enabled!=33u && (sdk_enabled!=34u && (sdk_enabled!=35u && (sdk_enabled!=36u && (sdk_enabled!=37u && (sdk_enabled!=38u && (sdk_enabled!=39u && (sdk_enabled!=40u && (sdk_enabled!=41u && (sdk_enabled!=42u && (sdk_enabled!=43u && (sdk_enabled!=44u && sdk_enabled!=45u))))))))))))))))))) || sdk_current_target!=0x4f5e0u || w[0] ||
       directory_words[15]!=5u || g_esp!=directory_words[33]+28u-44u ||
       g_ebx!=directory_words[58] || g_esi!=directory_words[59] ||
       g_edi!=directory_words[60] || g_ebp!=directory_words[61] ||
       (uint32_t)g_df!=directory_words[62] || g_fs_base!=directory_words[63] ||
       g_seh_ebp!=directory_words[46])stop(9,g_esp,207u);
    const uint32_t *frame=sdk_continuation_frame(44u,207u,s);
    if(frame[0]!=0x1e96adu || frame[1]!=directory_words[53] ||
       frame[2] || frame[3] || frame[4] || frame[5]!=g_ebp-0x24u ||
       frame[6]!=g_ebp-0x16cu || frame[7]!=0x148u || frame[8]!=1u ||
       frame[9]!=g_ebp-0x10u || frame[10] || FF_KERNEL_BINDING_COUNT+5u>64u)
        stop(9,g_esp,207u);
    if(ff_vm_check(&virtual_memory,0x2534f8u,4u,FF_VM_READ) ||
       *(uint32_t *)ff_probe_guest_memory(0x2534f8u,4u,FF_VM_READ,0x1e96a7u)!=0x800000cfu)
        stop(9,0x2534f8u,207u);
    if(sdk_query_frame_overlap(frame[5],8u) || sdk_query_frame_overlap(frame[6],frame[7]) ||
       sdk_query_frame_overlap(frame[9],8u))stop(9,g_esp,207u);
    const uint8_t *descriptor=ff_sdk_shared_memory(frame[9],8u,FF_VM_READ,s);
    const uint32_t length=(uint32_t)descriptor[0]|((uint32_t)descriptor[1]<<8);
    const uint32_t maximum=(uint32_t)descriptor[2]|((uint32_t)descriptor[3]<<8);
    const uint32_t mask=(uint32_t)descriptor[4]|((uint32_t)descriptor[5]<<8)|
        ((uint32_t)descriptor[6]<<16)|((uint32_t)descriptor[7]<<24);
    if(length || maximum!=3u || sdk_query_frame_overlap(mask,maximum))stop(9,frame[9],207u);
    w[0]=1u;w[2]=g_esp;w[3]=frame[0];w[4]=0x800000cfu;
    w[6]=frame[5];w[7]=frame[6];w[8]=frame[7];w[9]=frame[9];
    for(uint32_t i=0;i<10u;++i)w[10u+i]=frame[1u+i];
    w[20]=g_ebx;w[21]=g_esi;w[22]=g_edi;w[23]=g_ebp;
    w[24]=(uint32_t)g_df;w[25]=g_fs_base;w[26]=g_seh_ebp;
    w[37]=fatx_volume.file_count;w[38]=fatx_volume.active;w[39]=fatx_volume.opened;w[40]=fatx_volume.closed;
    w[47]=length;w[48]=maximum;w[49]=mask;w[50]=44u;
    uint32_t *row=kernel_record(207u,10u);
    row[1]=ff_fatx_query_directory_first(&fatx_volume,row+2);
    w[5]=row[1];
    if(row[1])stop(9,row[1],207u);
    g_eax=row[1];g_esp+=44u;(void)sdk_thread();ff_sdk_shared_read(s);
    if(g_esp!=w[2]+44u || g_ebx!=w[20] || g_esi!=w[21] || g_edi!=w[22] || g_ebp!=w[23] ||
       (uint32_t)g_df!=w[24] || g_fs_base!=w[25] || g_seh_ebp!=w[26] || g_eax)
        stop(9,g_esp,207u);
    const uint32_t *io=(const uint32_t *)ff_sdk_shared_memory(w[6],8u,FF_VM_READ,s);
    if(io[0] || io[1]<64u || io[1]>w[8])stop(9,w[6],207u);
    const uint32_t *out=(const uint32_t *)ff_sdk_shared_memory(w[7],io[1],FF_VM_READ,s);
    if(out[0] || out[1] || out[15]!=12u || io[1]!=64u+out[15])stop(9,w[7],207u);
    for(uint32_t i=2u;i<10u;++i)if(out[i])stop(9,w[7]+4u*i,207u);
    w[27]=g_esp;w[28]=g_eax;w[29]=io[0];w[30]=io[1];w[31]=out[15];w[32]=out[14];
    w[33]=out[10];w[34]=out[11];w[35]=out[12];w[36]=out[13];w[45]=out[0];w[46]=out[1];
    w[41]=fatx_volume.file_count;w[42]=fatx_volume.active;w[43]=fatx_volume.opened;w[44]=fatx_volume.closed;
    if(w[37]!=w[41] || w[38]!=w[42] || w[39]!=w[43] || w[40]!=w[44])stop(9,w[7],207u);
    for(uint32_t i=0;i<8u;++i)row[12u+i]=w[29u+i];
    w[1]=1u;
    if((sdk_enabled==32u || (sdk_enabled==33u || (sdk_enabled==34u || (sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u)))))))))))))) {
        uint32_t *v=next_query_words;
        if(v[0] || directory_cursor.consumed || directory_cursor.finished)stop(9,g_esp,207u);
        v[1]=ff_fatx_single_cursor_begin(&fatx_volume,w[10],(const uint8_t *)out,io[1],&directory_cursor);
        if(v[1])stop(9,v[1],207u);
        v[2]=w[10];v[3]=w[7];v[4]=io[1];v[5]=directory_cursor.initial_size;
        v[6]=directory_cursor.index;v[7]=directory_cursor.file_id;v[8]=directory_cursor.mount;v[9]=directory_cursor.device;
        v[10]=(uint32_t)directory_cursor.mount_revision;v[11]=(uint32_t)(directory_cursor.mount_revision>>32);
        v[12]=w[2];v[13]=w[3];v[47]=directory_cursor.consumed;v[49]=kernel_call_count;v[0]=1u;
    }
}
static int sdk_copy_profile(void){return (sdk_enabled==27u || (sdk_enabled==28u || (sdk_enabled==29u || (sdk_enabled==30u || (sdk_enabled==31u || (sdk_enabled==32u || (sdk_enabled==33u || (sdk_enabled==34u || (sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u)))))))))))))))))) && sdk_current_target==0x4f5e0u;}
static void sdk_shared_remaining_budget(uint32_t reserve,uint32_t guest)
{
    if(pfifo_consumer_enabled && pfifo_consumer_steps &&
       (pfifo_consumer_vm_checks>=FF_PFIFO_CONSUMER_MAX_VM_CHECKS ||
        reserve>=FF_PFIFO_CONSUMER_MAX_VM_CHECKS-pfifo_consumer_vm_checks)) {
        pfifo_consumer_pusher.classification=FF_PFIFO_PUSH_BUDGET;
        pfifo_consumer_budget_hit=1;stop(13,guest,reserve);
    }
}
static void sdk_copy_enter(ff_sdk_scalar *s)
{
    uint32_t *w=copy_words;ff_boot_thread *t=sdk_thread();
    if(!sdk_copy_profile() || w[0] || !query_words[1] || g_df ||
       g_esp!=0x03c10ad4u || g_ebx!=query_words[20] || g_esi || g_edi ||
       g_ebp!=query_words[23] || g_fs_base!=query_words[25] || g_seh_ebp!=query_words[26])stop(9,g_esp,0x1e956fu);
    const uint32_t *f=sdk_continuation_frame(12u,0x1e956fu,s);
    const uint32_t source=f[1],destination=f[2];
    if(f[0]!=0x1e96d3u || source!=0x03c10ae8u || destination!=0x03c10c9cu ||
       g_eax!=source || g_esp<t->stack_low+12u || g_esp>t->stack_top-12u ||
       source<t->stack_low || source>t->stack_top-76u ||
       destination<t->stack_low || destination>t->stack_top-305u ||
       (uint64_t)source+76u>destination || (uint64_t)g_esp+12u>source ||
       ff_vm_check(&virtual_memory,g_esp-12u,24u,FF_VM_READ|FF_VM_WRITE) ||
       ff_vm_check(&virtual_memory,source,76u,FF_VM_READ) ||
       ff_vm_check(&virtual_memory,destination,305u,FF_VM_READ|FF_VM_WRITE))stop(9,g_esp,0x1e956fu);
    memcpy(copy_source,ff_sdk_shared_memory(source,76u,FF_VM_READ,s),76u);
    memcpy(copy_before,ff_sdk_shared_memory(destination,305u,FF_VM_READ|FF_VM_WRITE,s),305u);
    if((uint32_t)copy_source[60]!=12u || copy_source[61] || copy_source[62] || copy_source[63])stop(9,source+60u,0x1e956fu);
    memcpy(copy_expected,copy_before,305u);
    static const uint32_t fields[9]={0x38u,8u,12u,16u,20u,24u,28u,44u,40u};
    for(uint32_t i=0;i<9u;++i)memcpy(copy_expected+4u*i,copy_source+fields[i],4u);
    memcpy(copy_expected+44u,copy_source+64u,12u);copy_expected[56]=0;copy_expected[304]=0;
    sdk_shared_remaining_budget(96u,0x1e956fu);
    w[0]=1u;w[5]=g_esp;w[6]=f[0];w[7]=source;w[8]=destination;
    w[9]=g_eax;w[10]=g_ecx;w[11]=g_edx;w[12]=g_ebx;w[13]=g_esi;w[14]=g_edi;
    w[15]=g_ebp;w[16]=(uint32_t)g_df;w[17]=g_fs_base;w[18]=g_seh_ebp;
    w[19]=12u;w[90]=pfifo_consumer_vm_checks;w[91]=96u;
}
void ff_sdk_shared_record_movs(ff_sdk_scalar *s,uint32_t width,uint32_t complete)
{
    ff_sdk_shared_write(s);(void)sdk_thread();uint32_t *w=copy_words;
    if(!sdk_copy_profile() || w[0]!=1u || w[1] || complete>1u ||
       g_esp!=w[5]-12u || g_eax!=w[7] || g_edx!=w[8] || g_ebx!=12u ||
       g_ebp!=w[15] || g_df || g_fs_base!=w[17] || g_seh_ebp!=w[18])stop(9,g_esp,0x1e956fu);
    if(width==4u && !complete) {
        if(w[20] || g_ecx!=3u || g_esi!=w[7]+64u || g_edi!=w[8]+44u ||
           ff_vm_check(&virtual_memory,g_esi,12u,FF_VM_READ) ||
           ff_vm_check(&virtual_memory,g_edi,12u,FF_VM_WRITE))stop(9,g_esi,0x1e95bdu);
        sdk_shared_remaining_budget(8u,0x1e956fu);w[23]=g_esi;w[24]=g_edi;w[25]=g_ecx;w[26]=g_esp;w[20]=1u;
    } else if(width==4u && complete) {
        if(w[20]!=1u || g_ecx || g_esi!=w[7]+76u || g_edi!=w[8]+56u)stop(9,g_esi,0x1e95bdu);
        if(memcmp(ff_sdk_shared_memory(w[8]+44u,12u,FF_VM_READ,s),copy_source+64u,12u))stop(9,w[8],0x1e95bdu);
        w[27]=g_esi;w[28]=g_edi;w[29]=g_ecx;w[21]=12u;w[20]=2u;
    } else if(width==1u && !complete) {
        /* Architectural ECX zero: no guest VM check or memory access. */
        if(w[20]!=2u || g_ecx || g_esi!=w[7]+76u || g_edi!=w[8]+56u)stop(9,g_esi,0x1e95c4u);
        w[30]=g_esi;w[31]=g_edi;w[32]=g_ecx;w[33]=g_esp;w[20]=3u;
    } else if(width==1u && complete) {
        if(w[20]!=3u || g_ecx || g_esi!=w[30] || g_edi!=w[31])stop(9,g_esi,0x1e95c4u);
        w[34]=g_esi;w[35]=g_edi;w[36]=g_ecx;w[22]=1u;w[20]=4u;
    } else stop(9,width,0x1e956fu);
}
static void sdk_copy_exit(ff_sdk_scalar *s)
{
    uint32_t *w=copy_words;
    if(!sdk_copy_profile() || w[0]!=1u || w[1] || w[20]!=4u ||
       g_esp!=w[5] || g_eax!=12u || g_ecx || g_edx!=w[8] ||
       g_ebx!=w[12] || g_esi!=w[13] || g_edi!=w[14] || g_ebp!=w[15] ||
       g_df || g_fs_base!=w[17] || g_seh_ebp!=w[18])stop(9,g_esp,0x1e956fu);
    const uint32_t *f=sdk_continuation_frame(12u,0x1e956fu,s);
    if(f[0]!=w[6] || f[1]!=w[7] || f[2]!=w[8] ||
       memcmp(ff_sdk_shared_memory(w[8],305u,FF_VM_READ,s),copy_expected,305u) ||
       memcmp(ff_sdk_shared_memory(w[7],76u,FF_VM_READ,s),copy_source,76u))stop(9,w[8],0x1e956fu);
    w[37]=g_esp;w[38]=g_eax;w[39]=g_ecx;w[40]=g_edx;w[41]=g_ebx;
    w[42]=g_esi;w[43]=g_edi;w[44]=g_ebp;w[45]=(uint32_t)g_df;w[46]=g_fs_base;w[47]=g_seh_ebp;
    w[51]=1u;w[52]=9u;w[53]=12u;w[54]=2u;w[55]=255u;w[1]=1u;
}
static void sdk_copy_post(ff_sdk_scalar *s)
{
    (void)s;uint32_t *w=copy_words;(void)sdk_thread();
    if(!sdk_copy_profile() || !w[1] || w[2] || g_esp!=w[5]+12u || g_eax!=12u ||
       g_ecx || g_edx!=w[8] || g_ebx!=w[12] || g_esi!=w[13] || g_edi!=w[14] ||
       g_ebp!=w[15] || g_df || g_fs_base!=w[17] || g_seh_ebp!=w[18])stop(9,g_esp,0x1e96d3u);
    w[48]=g_esp;w[49]=g_eax;w[50]=1u;w[2]=1u;
}
static void sdk_directory_exit(ff_sdk_scalar *s)
{
    uint32_t *w=copy_words;
    if(!sdk_copy_profile() || !w[2] || w[3] || g_esp!=directory_words[2] ||
       g_eax!=directory_words[53] || g_ebx!=directory_words[7] || g_esi!=directory_words[8] ||
       g_edi!=directory_words[9] || g_ebp!=directory_words[10] || g_df ||
       g_fs_base!=directory_words[13] || g_seh_ebp!=directory_words[12])stop(9,g_esp,0x1e95dbu);
    const uint32_t *f=sdk_continuation_frame(12u,0x1e95dbu,s);
    if(f[0]!=directory_words[3] || f[1]!=directory_words[53] || f[2]!=directory_words[5])stop(9,g_esp,0x1e95dbu);
    w[56]=g_esp;w[57]=g_eax;w[58]=g_ebx;w[59]=g_esi;w[60]=g_edi;
    w[61]=g_ebp;w[62]=(uint32_t)g_df;w[63]=g_fs_base;w[64]=g_seh_ebp;
    w[65]=f[0];w[66]=f[1];w[67]=f[2];w[3]=1u;
}
static void sdk_directory_post(ff_sdk_scalar *s)
{
    (void)s;uint32_t *w=copy_words;(void)sdk_thread();
    if(!sdk_copy_profile() || !w[3] || w[4] || g_esp!=directory_words[2]+12u ||
       g_eax!=directory_words[53] || g_ebx!=directory_words[7] || g_esi!=directory_words[8] ||
       g_edi!=directory_words[9] || g_ebp!=directory_words[10] || g_df ||
       g_fs_base!=directory_words[13] || g_seh_ebp!=directory_words[12])stop(9,g_esp,0x4eb66u);
    w[68]=g_esp;w[69]=g_eax;w[70]=g_ebx;w[71]=g_esi;w[72]=g_edi;
    w[73]=g_ebp;w[74]=(uint32_t)g_df;w[75]=g_fs_base;w[76]=g_seh_ebp;w[4]=1u;
}
static int sdk_gate_helper_profile(void){return (sdk_enabled==29u || (sdk_enabled==30u || (sdk_enabled==31u || (sdk_enabled==32u || (sdk_enabled==33u || (sdk_enabled==34u || (sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u)))))))))))))))) && sdk_current_target==0x4f5e0u;}
static void sdk_gate_helper_enter(ff_sdk_scalar *s)
{
    uint32_t *w=gate_helper_words;ff_boot_thread *t=sdk_thread();
    if(!sdk_gate_helper_profile() || w[0] || record_log_words[36]!=1u ||
       g_esp!=0x03c10c50u || g_eax!=record_log_words[11] || g_ecx!=0x03c10cc8u ||
       g_ebp!=record_log_words[4] || g_ebx!=record_log_words[8] ||
       g_esi!=record_log_words[9] || g_edi!=record_log_words[10] || g_df ||
       g_fs_base!=copy_words[75] || g_seh_ebp!=record_log_words[32] ||
       g_esp<t->stack_low+12u || g_esp>t->stack_top-12u ||
       ff_vm_check(&virtual_memory,g_esp-12u,24u,FF_VM_READ|FF_VM_WRITE))stop(9,g_esp,0x4e960u);
    sdk_shared_remaining_budget(64u,0x4e960u);
    const uint32_t *f=sdk_continuation_frame(12u,0x4e960u,s);
    if(f[0]!=0x4eb8fu || f[1]!=0x25c468u || f[2]!=0x03c10cc8u ||
       f[2]!=copy_words[8]+44u || (uint64_t)g_esp+12u>f[2])stop(9,g_esp,0x4e960u);
    const uint8_t *name=ff_sdk_shared_memory(f[2],13u,FF_VM_READ,s);
    if(memcmp(name,copy_source+64u,12u) || name[12])stop(9,f[2],0x4e960u);
    const uint8_t *format=ff_sdk_shared_memory(f[1],8u,FF_VM_READ,s);
    uint32_t length=0u,percent=0u;
    while(length<8u && format[length]){if(format[length]=='%')++percent;++length;}
    /* Metadata only: do not interpret conversions or assume the control zero. */
    const uint32_t control=*(uint32_t *)ff_sdk_shared_memory(0x2da2a0u,4u,FF_VM_READ,s);
    w[3]=control;w[4]=g_esp;w[5]=f[0];w[6]=f[1];w[7]=f[2];w[8]=g_esp+8u;
    w[9]=g_eax;w[10]=g_ecx;w[11]=g_edx;w[12]=g_ebx;w[13]=g_esi;
    w[14]=g_edi;w[15]=g_ebp;w[16]=(uint32_t)g_df;w[17]=g_fs_base;w[18]=g_seh_ebp;
    w[19]=length;w[20]=percent;w[21]=12u;w[22]=1u;w[23]=0x2da2a0u;
    w[24]=64u;w[25]=pfifo_consumer_vm_checks;w[0]=1u;
}
static void sdk_gate_helper_exit(ff_sdk_scalar *s)
{
    uint32_t *w=gate_helper_words;
    if(!sdk_gate_helper_profile() || w[0]!=1u || w[1] || w[3]!=0u ||
       g_esp!=w[4] || g_eax || g_ecx!=w[10] || g_edx!=w[11] ||
       g_ebx!=w[12] || g_esi!=w[13] || g_edi!=w[14] || g_ebp!=w[15] ||
       g_df || g_fs_base!=w[17] || g_seh_ebp!=w[18])stop(9,g_esp,0x4e960u);
    const uint32_t *f=sdk_continuation_frame(12u,0x4e960u,s);
    if(f[0]!=w[5] || f[1]!=w[6] || f[2]!=w[7])stop(9,g_esp,0x4e960u);
    w[26]=g_esp;w[27]=g_eax;w[28]=g_ecx;w[29]=g_edx;w[30]=g_ebx;
    w[31]=g_esi;w[32]=g_edi;w[33]=g_ebp;w[34]=(uint32_t)g_df;w[35]=g_fs_base;w[36]=g_seh_ebp;
    w[37]=f[0];w[38]=f[1];w[39]=f[2];w[1]=1u;
}
static void sdk_gate_helper_post(ff_sdk_scalar *s)
{
    (void)s;(void)sdk_thread();uint32_t *w=gate_helper_words;
    if(!sdk_gate_helper_profile() || w[1]!=1u || w[2] ||
       g_esp!=w[4]+4u || g_eax || g_ecx!=w[10] || g_edx!=w[11] ||
       g_ebx!=w[12] || g_esi!=w[13] || g_edi!=w[14] || g_ebp!=w[15] ||
       g_df || g_fs_base!=w[17] || g_seh_ebp!=w[18])stop(9,g_esp,0x4eb8fu);
    w[40]=1u;w[41]=g_esp;w[42]=g_eax;w[43]=g_ecx;w[44]=g_edx;w[45]=g_ebx;
    w[46]=g_esi;w[47]=g_edi;w[48]=g_ebp;w[49]=(uint32_t)g_df;w[50]=g_fs_base;w[51]=g_seh_ebp;w[2]=1u;
}
static int sdk_compare_profile(void){return (sdk_enabled==30u || (sdk_enabled==31u || (sdk_enabled==32u || (sdk_enabled==33u || (sdk_enabled==34u || (sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u))))))))))))))) && sdk_current_target==0x4f5e0u;}
static uint32_t sdk_compare_fold(uint32_t value){return value>=65u && value<=90u?value+32u:value;}
static void sdk_compare_enter(ff_sdk_scalar *s)
{
    if(!sdk_compare_profile() || compare_started>=2u || compare_started!=compare_returned)stop(2,0x1e056du,0);
    compare_index=compare_started;
    uint32_t *w=compare_words;ff_boot_thread *t=sdk_thread();
    if(compare_index && (compare_evidence[0].words[2]!=1u || !compare_evidence[0].words[40] || compare_returned!=1u))stop(2,0x1e056du,0);
    if(!sdk_compare_profile() || w[0] || gate_helper_words[2]!=1u ||
       g_esp!=0x03c10c50u || g_ebp!=record_log_words[4] || g_df ||
       g_fs_base!=copy_words[75] || g_seh_ebp!=record_log_words[32] ||
       g_esp<t->stack_low+24u || g_esp>t->stack_top-12u ||
       ff_vm_check(&virtual_memory,g_esp-24u,36u,FF_VM_READ|FF_VM_WRITE))stop(9,g_esp,0x1e056du);
    sdk_shared_remaining_budget(256u,0x1e056du);
    const uint32_t *f=sdk_continuation_frame(12u,0x1e056du,s);
    const uint32_t expected_return=compare_index?0x4ebebu:0x4ebd1u;
    const uint32_t expected_literal=compare_index?0x25c440u:0x25c450u;
    if(f[0]!=expected_return || f[1]!=0x03c10cc8u || f[2]!=expected_literal ||
       f[1]!=copy_words[8]+44u || (uint64_t)g_esp+12u>f[1])stop(2,0x1e056du,0);
    memcpy(compare_name,ff_sdk_shared_memory(f[1],13u,FF_VM_READ,s),13u);
    memcpy(compare_literal,ff_sdk_shared_memory(f[2],16u,FF_VM_READ,s),16u);
    if(memcmp(compare_name,copy_source+64u,12u) || compare_name[12])stop(9,f[1],0x1e056du);
    uint32_t literal_length=0u;while(literal_length<16u && compare_literal[literal_length])++literal_length;
    if(literal_length==16u)stop(9,f[2],0x1e056du);
    const uint32_t control=*(uint32_t *)ff_sdk_shared_memory(0x315ac4u,4u,FF_VM_READ,s);
    uint32_t result=0u;
    /* Independent source-derived sign oracle; it checks, never assigns EAX. */
    for(uint32_t i=0;i<=12u && i<=literal_length;++i){
        uint32_t a=sdk_compare_fold(compare_name[i]),b=sdk_compare_fold(compare_literal[i]);
        if(a!=b){result=a<b?UINT32_MAX:1u;break;}if(!a)break;
    }
    w[3]=control;w[4]=g_esp;w[5]=f[0];w[6]=f[1];w[7]=f[2];w[8]=12u;w[9]=literal_length;
    w[10]=result;w[11]=control==0u;w[12]=g_eax;w[13]=g_ecx;w[14]=g_edx;
    w[15]=g_ebx;w[16]=g_esi;w[17]=g_edi;w[18]=g_ebp;w[19]=(uint32_t)g_df;
    w[20]=g_fs_base;w[21]=g_seh_ebp;w[22]=pfifo_consumer_vm_checks;w[23]=256u;
    w[50]=compare_name[0];w[51]=compare_literal[0];w[52]=13u;w[53]=literal_length+1u;
    w[54]=1u;w[57]=0x315ac4u;w[58]=36u;w[59]=compare_index;w[0]=1u;++compare_started;
}
static void sdk_compare_exit(ff_sdk_scalar *s)
{
    uint32_t *w=compare_words;
    if(!sdk_compare_profile() || compare_index>=2u || compare_started!=compare_index+1u || compare_returned!=compare_index ||
       w[0]!=1u || w[1] || w[3]!=0u || !w[11] ||
       g_esp!=w[4] || g_eax!=w[10] || (g_ecx&0xffffff00u)!=(w[13]&0xffffff00u) ||
       g_edx!=w[14] || g_ebx!=w[15] || g_esi!=w[16] || g_edi!=w[17] || g_ebp!=w[18] ||
       g_df || g_fs_base!=w[20] || g_seh_ebp!=w[21])stop(9,g_esp,0x1e056du);
    const uint32_t *f=sdk_continuation_frame(12u,0x1e056du,s);
    if(f[0]!=w[5] || f[1]!=w[6] || f[2]!=w[7] ||
       memcmp(ff_sdk_shared_memory(w[6],13u,FF_VM_READ,s),compare_name,13u) ||
       memcmp(ff_sdk_shared_memory(w[7],16u,FF_VM_READ,s),compare_literal,16u))stop(9,g_esp,0x1e056du);
    w[24]=g_esp;w[25]=g_eax;w[26]=g_ecx;w[27]=g_edx;w[28]=g_ebx;w[29]=g_esi;w[30]=g_edi;
    w[31]=g_ebp;w[32]=(uint32_t)g_df;w[33]=g_fs_base;w[34]=g_seh_ebp;
    w[35]=f[0];w[36]=f[1];w[37]=f[2];w[55]=w[56]=1u;w[1]=1u;
}
static void sdk_compare_post(uint32_t guest,ff_sdk_scalar *s)
{
    (void)s;(void)sdk_thread();uint32_t *w=compare_words;
    if(!sdk_compare_profile() || compare_index>=2u || compare_started!=compare_index+1u || compare_returned!=compare_index ||
       guest!=w[5] || w[1]!=1u || w[2] || g_esp!=w[4]+4u || g_eax!=w[25] ||
       g_ecx!=w[26] || g_edx!=w[14] || g_ebx!=w[15] || g_esi!=w[16] ||
       g_edi!=w[17] || g_ebp!=w[18] || g_df || g_fs_base!=w[20] || g_seh_ebp!=w[21])stop(9,g_esp,0x4ebd1u);
    w[38]=1u;w[39]=g_esp;w[40]=g_eax;w[41]=g_ecx;w[42]=g_edx;w[43]=g_ebx;
    w[44]=g_esi;w[45]=g_edi;w[46]=g_ebp;w[47]=(uint32_t)g_df;w[48]=g_fs_base;w[49]=g_seh_ebp;w[2]=1u;++compare_returned;
}

static int sdk_next_directory_profile(void){return (sdk_enabled==31u || (sdk_enabled==32u || (sdk_enabled==33u || (sdk_enabled==34u || (sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u)))))))))))))) && sdk_current_target==0x4f5e0u;}
static void sdk_next_directory_enter(ff_sdk_scalar *scalar)
{
    uint32_t *w=next_directory_words;ff_boot_thread *t=sdk_thread();
    const uint32_t *previous=compare_evidence[1].words;
    if(!sdk_next_directory_profile() || w[0] || compare_started!=2u || compare_returned!=2u ||
       previous[2]!=1u || previous[40] || g_eax || g_esp!=0x03c10c50u ||
       g_ebx!=previous[43] || g_esi!=previous[44] || g_edi!=previous[45] || g_ebp!=previous[46] ||
       g_ecx!=previous[41] || g_df || g_fs_base!=previous[48] || g_seh_ebp!=previous[49] ||
       g_esp<t->stack_low+0x184u || g_esp>t->stack_top-12u ||
       ff_vm_check(&virtual_memory,g_esp-0x184u,0x190u,FF_VM_READ|FF_VM_WRITE))stop(9,g_esp,0x1e96e8u);
    sdk_shared_remaining_budget(128u,0x1e96e8u);
    const uint32_t *f=sdk_continuation_frame(12u,0x1e96e8u,scalar);
    if(f[0]!=0x4ecd4u || f[1]!=0x70038u || f[1]!=directory_words[53] ||
       f[2]!=0x03c10c9cu || f[2]!=copy_words[8] || g_ebp!=f[1] || g_edx!=f[2] ||
       ff_vm_check(&virtual_memory,f[2],305u,FF_VM_READ|FF_VM_WRITE))stop(9,g_esp,0x1e96e8u);
    w[3]=g_esp;w[4]=f[0];w[5]=f[1];w[6]=f[2];w[7]=g_ebp;w[8]=g_ebx;
    w[9]=g_esi;w[10]=g_edi;w[11]=g_edx;w[12]=g_ecx;w[13]=(uint32_t)g_df;
    w[14]=g_fs_base;w[15]=g_seh_ebp;w[16]=128u;w[17]=0x190u;w[41]=g_eax;w[0]=1u;
}

static void sdk_directory_next_complete(ff_sdk_scalar *scalar,const uint32_t *frame)
{
    uint32_t *v=next_query_words,*w=next_directory_words;
    if((sdk_enabled!=32u && (sdk_enabled!=33u && (sdk_enabled!=34u && (sdk_enabled!=35u && (sdk_enabled!=36u && (sdk_enabled!=37u && (sdk_enabled!=38u && (sdk_enabled!=39u && (sdk_enabled!=40u && (sdk_enabled!=41u && (sdk_enabled!=42u && (sdk_enabled!=43u && (sdk_enabled!=44u && sdk_enabled!=45u))))))))))))) || !sdk_next_directory_profile() || v[0]!=1u || v[1] || v[14] || v[16] ||
       !directory_cursor.consumed || directory_cursor.finished || directory_cursor.index!=1u ||
       directory_cursor.handle!=frame[1] || frame[1]!=v[2] ||
       !query_words[1] || !copy_words[1] || !record_log_words[1] || compare_returned!=2u ||
       sdk_query_frame_overlap(frame[5],8u) || sdk_query_frame_overlap(frame[6],frame[7]))stop(9,g_esp,207u);
    sdk_shared_remaining_budget(128u,207u);
    memcpy(next_query_before,ff_sdk_shared_memory(frame[6],328u,FF_VM_READ,scalar),328u);
    memcpy(next_iosb_before,ff_sdk_shared_memory(frame[5],8u,FF_VM_READ,scalar),8u);
    ff_fatx_directory_entry current;
    if(ff_fatx_directory_at(&fatx_volume,directory_cursor.mount,NULL,0u,0u,0u,&current))stop(9,g_esp,207u);
    v[14]=1u;v[17]=g_esp;v[25]=frame[5];v[26]=frame[6];v[27]=frame[7];
    v[39]=fatx_volume.file_count;v[40]=fatx_volume.active;v[41]=fatx_volume.opened;v[42]=fatx_volume.closed;
    v[50]=kernel_call_count;v[54]=current.size;v[55]=current.first_cluster;v[56]=current.attributes;
    uint32_t *row=kernel_record(207u,10u);
    row[1]=ff_fatx_query_directory_next_single(&fatx_volume,row+2,&directory_cursor);
    v[15]=row[1];
    if(row[1]!=0x80000006u || !directory_cursor.finished)stop(9,row[1],207u);
    g_eax=row[1];g_esp+=44u;(void)sdk_thread();ff_sdk_shared_read(scalar);
    if(g_esp!=w[18]+44u || g_eax!=v[15] || g_ebp!=w[34] || g_esi!=w[35] ||
       g_ebx!=w[36] || g_edi!=w[37] || g_ecx!=w[43] || g_edx!=w[44] ||
       g_df || g_fs_base!=w[39] || g_seh_ebp!=w[40])stop(9,g_esp,207u);
    memcpy(next_query_after,ff_sdk_shared_memory(v[26],328u,FF_VM_READ,scalar),328u);
    memcpy(next_iosb_after,ff_sdk_shared_memory(v[25],8u,FF_VM_READ,scalar),8u);
    const uint32_t *io=(const uint32_t *)ff_sdk_shared_memory(v[25],8u,FF_VM_READ,scalar);
    if(io[0]!=v[15] || io[1] || memcmp(next_query_before,next_query_after,328u))stop(9,v[25],207u);
    v[18]=g_esp;v[19]=g_eax;v[20]=io[0];v[21]=io[1];v[22]=directory_cursor.finished;
    v[23]=directory_cursor.index;v[24]=1u;v[30]=g_ebp;v[31]=g_esi;v[32]=g_ebx;v[33]=g_edi;
    v[34]=g_ecx;v[35]=g_edx;v[36]=(uint32_t)g_df;v[37]=g_fs_base;v[38]=g_seh_ebp;
    v[43]=fatx_volume.file_count;v[44]=fatx_volume.active;v[45]=fatx_volume.opened;v[46]=fatx_volume.closed;
    if(v[39]!=v[43] || v[40]!=v[44] || v[41]!=v[45] || v[42]!=v[46])stop(9,v[25],207u);
    v[51]=kernel_call_count;v[52]=1u;
    row[12]=io[0];row[13]=io[1];row[14]=directory_cursor.index;row[15]=directory_cursor.finished;
    v[16]=1u;
}

static void sdk_next_directory_query_stop(ff_sdk_scalar *scalar)
{
    uint32_t *w=next_directory_words;
    if(!sdk_next_directory_profile() || w[0]!=1u || w[1] || w[2] || !query_words[1] ||
       g_esp!=w[3]-0x184u || g_ebp!=w[3]-4u || g_esi ||
       g_ebx!=w[8] || g_edi!=w[10] || g_ecx!=w[12] || g_edx!=w[11] ||
       g_df || g_fs_base!=w[14] || g_seh_ebp!=w[15])stop(9,g_esp,207u);
    const uint32_t *f=sdk_continuation_frame(44u,207u,scalar);
    const uint32_t expected[11]={0x1e9714u,w[5],0u,0u,0u,w[3]-12u,w[3]-0x154u,0x148u,1u,0u,0u};
    for(uint32_t i=0;i<11u;++i)if(f[i]!=expected[i])stop(9,g_esp,207u);
    const uint32_t raw=*(uint32_t *)ff_sdk_shared_memory(0x2534f8u,4u,FF_VM_READ,scalar);
    if(raw!=0x800000cfu || g_eax!=f[5] ||
       ff_vm_check(&virtual_memory,f[5],8u,FF_VM_WRITE) ||
       ff_vm_check(&virtual_memory,f[6],f[7],FF_VM_WRITE))stop(9,g_esp,207u);
    w[18]=g_esp;w[19]=f[0];w[20]=raw;w[21]=207u;w[22]=10u;w[23]=44u;
    for(uint32_t i=0;i<10u;++i)w[24u+i]=f[1u+i];
    w[34]=g_ebp;w[35]=g_esi;w[36]=g_ebx;w[37]=g_edi;w[38]=(uint32_t)g_df;
    w[39]=g_fs_base;w[40]=g_seh_ebp;w[42]=g_eax;w[43]=g_ecx;w[44]=g_edx;
    w[45]=fatx_volume.file_count;w[46]=fatx_volume.active;w[47]=fatx_volume.opened;w[48]=fatx_volume.closed;
    w[49]=1u;w[1]=1u;
    if(sdk_enabled==31u)stop(1,raw,207u); /* Preserve the accepted M31 terminal request. */
    sdk_directory_next_complete(scalar,f);
}


static int sdk_error_profile(void){return (sdk_enabled==33u || (sdk_enabled==34u || (sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u)))))))))))) && sdk_current_target==0x4f5e0u;}
static uint32_t sdk_error_read32(uint32_t address,ff_sdk_scalar *s)
{
    const uint8_t *p=ff_sdk_shared_memory(address,4u,FF_VM_READ,s);
    return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);
}
static void sdk_error_preserved(const uint32_t *w,uint32_t offset,uint32_t tag)
{
    if(g_ebx!=w[offset] || g_esi!=w[offset+1u] || g_edi!=w[offset+2u] || g_ebp!=w[offset+3u] ||
       (uint32_t)g_df!=w[offset+4u] || g_fs_base!=w[offset+5u] || g_seh_ebp!=w[offset+6u])stop(9,g_esp,tag);
}
static void sdk_error_snapshot(uint32_t *w,uint32_t offset)
{
    w[offset]=g_ebx;w[offset+1u]=g_esi;w[offset+2u]=g_edi;w[offset+3u]=g_ebp;
    w[offset+4u]=(uint32_t)g_df;w[offset+5u]=g_fs_base;w[offset+6u]=g_seh_ebp;
}
static void sdk_error_enter(uint32_t guest,ff_sdk_scalar *s)
{
    ff_boot_thread *t=sdk_thread();uint32_t *e=error_words,*v=tls_words;
    if(!sdk_error_profile() || g_fs_base!=t->fs || dpc_frame.active || irql_pair.active || g_df)
        stop(9,g_esp,guest);
    sdk_shared_remaining_budget(256u,guest);
    if(guest==0x31155u){
        if(e[0] || !next_query_words[16] || next_query_words[15]!=0x80000006u ||
           g_esp!=0x03c10af4u || g_eax!=0x80000006u ||
           g_ebp!=next_directory_words[34] || g_esi!=next_directory_words[9] ||
           g_ebx!=next_directory_words[8] || g_edi!=next_directory_words[10] ||
           g_fs_base!=next_directory_words[14] || g_seh_ebp!=next_directory_words[15] ||
           g_esp<t->stack_low+8u || g_esp>t->stack_top-8u ||
           ff_vm_check(&virtual_memory,g_esp-8u,16u,FF_VM_READ|FF_VM_WRITE))stop(9,g_esp,guest);
        const uint32_t *f=sdk_continuation_frame(8u,guest,s);
        if(f[0]!=0x1e971fu || f[1]!=0x80000006u)stop(9,g_esp,guest);
        e[3]=g_esp;e[4]=f[0];e[5]=f[1];e[6]=g_eax;e[7]=g_ecx;e[8]=g_edx;
        sdk_error_snapshot(e,9u);e[16]=16u;e[48]=kernel_call_count;e[50]=t->id;e[51]=1u;e[0]=1u;
    }else{
        if(guest!=0x31127u || e[0]!=1u || e[24]!=1u || e[28] || v[0] ||
           g_esp!=0x03c10aecu || g_eax!=18u || g_edx!=18u)stop(9,g_esp,guest);
        sdk_error_preserved(e,9u,guest);
        const uint32_t *f=sdk_continuation_frame(8u,guest,s);
        if(f[0]!=0x31167u || f[1]!=18u)stop(9,g_esp,guest);
        if(t->object>UINT32_MAX-0x22cu || t->fs!=t->object+0x200u ||
           t->stack_low>t->tls || t->tls>t->stack_top || t->tls_size!=20u ||
           (uint64_t)t->tls+t->tls_size!=t->stack_top ||
           ff_vm_check(&virtual_memory,t->tls,20u,FF_VM_READ|FF_VM_WRITE) ||
           ff_vm_check(&virtual_memory,t->fs,0x2cu,FF_VM_READ) ||
           ff_vm_check(&virtual_memory,t->object+0x28u,4u,FF_VM_READ))stop(9,t->tls,guest);
        const uint32_t irql=*ff_sdk_shared_memory(t->fs+0x24u,1u,FF_VM_READ,s);
        if(irql>=2u)stop(9,irql,guest);
        const uint32_t object=sdk_error_read32(t->fs+0x28u,s);
        const uint32_t table=sdk_error_read32(t->fs+4u,s);
        const uint32_t index=sdk_error_read32(0x2bb130u,s);
        const int64_t slot64=(int64_t)table+4*(int64_t)(int32_t)index;
        if(object!=t->object || table!=t->stack_top || sdk_error_read32(t->fs+8u,s)!=t->stack_low ||
           sdk_error_read32(t->object+0x28u,s)!=t->tls || slot64<0 || slot64>UINT32_MAX ||
           (uint32_t)slot64!=t->tls || (uint32_t)(table+index*4u)!=(uint32_t)slot64)stop(9,table,guest);
        const uint32_t slot=(uint32_t)slot64,record=sdk_error_read32(slot,s);
        if(record!=t->tls+4u || (uint64_t)record+8u>(uint64_t)t->tls+t->tls_size ||
           ff_vm_check(&virtual_memory,record+4u,4u,FF_VM_READ|FF_VM_WRITE))stop(9,record,guest);
        memcpy(error_tls_before,ff_sdk_shared_memory(t->tls,20u,FF_VM_READ,s),20u);
        v[3]=g_esp;v[4]=f[0];v[5]=f[1];v[6]=g_eax;v[7]=g_ecx;v[8]=g_edx;sdk_error_snapshot(v,9u);
        v[16]=irql;v[17]=object;v[18]=table;v[19]=index;v[20]=(uint32_t)(int32_t)index;
        v[21]=slot;v[22]=record;v[23]=record+4u;v[24]=sdk_error_read32(record+4u,s);v[25]=f[1];
        v[26]=t->tls;v[27]=t->tls_size;v[28]=t->id;v[29]=1u;v[0]=1u;
    }
}
static void sdk_error_kernel(ff_sdk_scalar *s)
{
    uint32_t *e=error_words;ff_boot_thread *t=sdk_thread();
    if(!sdk_error_profile() || e[0]!=1u || e[17] || e[24] || e[52] || g_fs_base!=t->fs ||
       g_esp!=0x03c10aecu || g_eax!=e[6] || g_ecx!=e[7] || g_edx!=e[8] ||
       dpc_frame.active || irql_pair.active)stop(9,g_esp,301u);
    sdk_error_preserved(e,9u,301u);
    const uint32_t *f=sdk_continuation_frame(8u,301u,s);
    if(f[0]!=0x3115fu || f[1]!=0x80000006u || FF_KERNEL_BINDING_COUNT<=2u ||
       bindings[2].ordinal!=301u || bindings[2].slot!=0x253370u ||
       sdk_error_read32(0x253370u,s)!=FF_KERNEL_BASE+8u || !recomp_lookup_kernel(FF_KERNEL_BASE+8u))stop(9,g_esp,301u);
    e[18]=g_esp;e[19]=f[0];e[20]=f[1];e[21]=0x253370u;e[22]=FF_KERNEL_BASE+8u;e[23]=g_eax;e[17]=1u;
    /* Bounded owned service contract verified against pinned SDK: NO_MORE_FILES -> ERROR_NO_MORE_FILES.
       This models the reached conversion only; all other statuses/callers remain guarded. */
    uint32_t *row=kernel_record(301u,1u);
    if(row[2]!=0x80000006u)stop(9,row[2],301u);
    row[1]=18u;g_eax=row[1];g_esp+=8u;e[27]=row[1];e[49]=kernel_call_count;e[52]=1u;
    sdk_error_preserved(e,9u,301u);ff_sdk_shared_read(s);
}
static void sdk_error_exit(uint32_t guest,ff_sdk_scalar *s)
{
    uint32_t *e=error_words,*v=tls_words,*o=error_outer_words,*n=next_directory_words;
    ff_boot_thread *t=sdk_thread();
    if(!sdk_error_profile() || e[0]!=1u || g_fs_base!=t->fs || dpc_frame.active || irql_pair.active)stop(2,guest,0);
    if(guest==0x3115fu){
        if(e[24] || e[52]!=1u || g_esp!=e[3] || g_eax!=18u || g_ecx!=e[7] || g_edx!=e[8] || e[49]!=e[48]+1u)stop(9,g_esp,guest);
        sdk_error_preserved(e,9u,guest);e[25]=g_esp;e[26]=g_eax;e[24]=1u;
    }else if(guest==0x31127u){
        if(v[0]!=1u || v[1] || g_esp!=v[3] || g_eax!=v[22] || g_ecx!=v[5] || g_edx!=18u)stop(9,g_esp,guest);
        sdk_error_preserved(v,9u,guest);
        const uint32_t *f=sdk_continuation_frame(8u,guest,s);
        if(f[0]!=v[4] || f[1]!=v[5])stop(9,g_esp,guest);
        memcpy(error_tls_after,ff_sdk_shared_memory(v[26],20u,FF_VM_READ,s),20u);
        uint8_t expected[20];memcpy(expected,error_tls_before,20u);
        const uint32_t offset=v[23]-v[26];
        if(offset>16u)stop(9,v[23],guest);
        for(uint32_t i=0;i<4u;++i)expected[offset+i]=(uint8_t)(v[5]>>(8u*i));
        if(memcmp(expected,error_tls_after,20u) || sdk_error_read32(v[23],s)!=18u ||
           *ff_sdk_shared_memory(t->fs+0x24u,1u,FF_VM_READ,s)!=v[16] ||
           sdk_error_read32(t->fs+4u,s)!=v[18] || sdk_error_read32(0x2bb130u,s)!=v[19] ||
           sdk_error_read32(v[21],s)!=v[22])stop(9,v[23],guest);
        v[31]=g_esp;v[32]=g_eax;v[33]=g_ecx;v[34]=g_edx;sdk_error_snapshot(v,35u);
        v[42]=sdk_error_read32(v[23],s);v[43]=v[16];v[44]=v[18];v[45]=v[19];v[46]=v[22];v[47]=1u;v[1]=1u;
    }else if(guest==0x31167u){
        if(v[1]!=1u || v[2] || e[28] || g_esp!=e[3] || g_eax!=v[22] || g_ecx!=18u || g_edx!=18u)stop(9,g_esp,guest);
        sdk_error_preserved(e,9u,guest);v[2]=1u;e[28]=1u;e[29]=g_esp;e[30]=g_eax;e[31]=g_ecx;e[32]=g_edx;
    }else if(guest==0x31155u){
        if(e[1] || e[28]!=1u || g_esp!=e[3] || g_eax!=18u || g_ecx!=18u || g_edx!=18u)stop(9,g_esp,guest);
        sdk_error_preserved(e,9u,guest);
        const uint32_t *f=sdk_continuation_frame(8u,guest,s);
        if(f[0]!=e[4] || f[1]!=e[5])stop(9,g_esp,guest);
        e[33]=g_esp;e[34]=g_eax;e[35]=g_ecx;e[36]=g_edx;sdk_error_snapshot(e,37u);e[1]=1u;
    }else if(guest==0x1e971fu){
        if(e[1]!=1u || e[2] || g_esp!=e[3]+8u || g_eax!=18u || g_ecx!=18u || g_edx!=18u)stop(9,g_esp,guest);
        sdk_error_preserved(e,9u,guest);e[44]=g_esp;e[45]=g_eax;e[46]=g_ecx;e[47]=g_edx;e[2]=1u;
    }else if(guest==0x1e96e8u){
        if(e[2]!=1u || o[0] || g_esp!=n[3] || g_eax || g_ecx!=18u || g_edx!=18u ||
           g_ebx!=n[8] || g_esi!=n[9] || g_edi!=n[10] || g_ebp!=n[7] || g_df ||
           g_fs_base!=n[14] || g_seh_ebp!=n[15] || sdk_error_read32(v[23],s)!=18u)stop(9,g_esp,guest);
        const uint32_t *f=sdk_continuation_frame(12u,guest,s);
        if(f[0]!=n[4] || f[1]!=n[5] || f[2]!=n[6])stop(9,g_esp,guest);
        o[2]=n[3];o[3]=g_esp;o[4]=g_eax;o[5]=g_ebx;o[6]=g_esi;o[7]=g_edi;o[8]=g_ebp;
        o[9]=g_ecx;o[10]=g_edx;o[11]=(uint32_t)g_df;o[12]=g_fs_base;o[13]=g_seh_ebp;
        o[14]=f[0];o[15]=f[1];o[16]=f[2];o[0]=1u;
    }else if(guest==0x4ecd4u){
        if(o[0]!=1u || o[1] || g_esp!=o[3]+12u || g_eax || g_ecx!=18u || g_edx!=18u ||
           g_ebx!=o[5] || g_esi!=o[6] || g_edi!=o[7] || g_ebp!=o[8] ||
           (uint32_t)g_df!=o[11] || g_fs_base!=o[12] || g_seh_ebp!=o[13] || sdk_error_read32(v[23],s)!=18u)stop(9,g_esp,guest);
        o[17]=g_esp;o[18]=g_eax;o[19]=g_ebx;o[20]=g_esi;o[21]=g_edi;o[22]=g_ebp;
        o[23]=g_ecx;o[24]=g_edx;o[25]=(uint32_t)g_df;o[26]=g_fs_base;o[27]=g_seh_ebp;o[28]=1u;o[1]=1u;
    }else stop(2,guest,0);
}


static int sdk_close_profile(void){return (sdk_enabled==34u || (sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u))))))))))) && sdk_current_target==0x4f5e0u;}
extern void ff_title_0003417A(void);
/* A checked scalar handoff to a retained original C provider. Its own entry/exit
 * observations and kernel service remain unchanged; never duplicate guest traces. */
void ff_sdk_shared_c_close(ff_sdk_scalar *s)
{
    ff_sdk_shared_write(s);ff_boot_thread *t=sdk_thread();uint32_t *w=close_words;
    if(!sdk_close_profile() || w[0] || error_outer_words[1]!=1u || error_outer_words[28]!=1u ||
       g_esp!=0x03c10c54u || g_eax || g_ecx!=18u || g_edx!=18u || g_ebp!=0x70038u ||
       g_fs_base!=t->fs || dpc_frame.active || irql_pair.active || g_df ||
       g_esp<t->stack_low+8u || startup_words[14]!=0x03c10e5cu ||
       startup_words[14]>t->stack_top-4u ||
       ff_vm_check(&virtual_memory,g_esp-8u,0x214u,FF_VM_READ|FF_VM_WRITE))stop(9,g_esp,0x3417au);
    if(g_ebx!=error_outer_words[19] || g_esi!=error_outer_words[20] || g_edi!=error_outer_words[21] ||
       g_ebp!=error_outer_words[22] || (uint32_t)g_df!=error_outer_words[25] ||
       g_fs_base!=error_outer_words[26] || g_seh_ebp!=error_outer_words[27])stop(9,g_esp,0x3417au);
    sdk_shared_remaining_budget(256u,0x3417au);
    const uint32_t *f=sdk_continuation_frame(8u,0x3417au,s);
    if(f[0]!=0x4ece2u || f[1]!=0x70038u || f[1]!=directory_cursor.handle ||
       !directory_cursor.finished || directory_cursor.index!=1u ||
       directory_cursor.owner!=&fatx_volume || directory_cursor.backing!=&local_disk ||
       directory_cursor.namespace_owner!=&object_namespace || !fatx_ready || !namespace_ready || !disk_ready ||
       fatx_volume.disk!=&local_disk || fatx_volume.space!=&object_namespace ||
       local_disk.memory!=&virtual_memory || object_namespace.memory!=&virtual_memory ||
       virtual_memory.ram!=guest_ram || fatx_volume.mount_count!=1u || fatx_volume.file_count!=1u ||
       fatx_volume.active!=1u || fatx_volume.opened!=16u || fatx_volume.closed!=15u ||
       kernel_call_count>=FF_KERNEL_CAPACITY || t->tls_size!=20u ||
       memcmp(ff_sdk_shared_memory(t->tls,20u,FF_VM_READ,s),error_tls_after,20u))stop(9,g_esp,0x3417au);
    uint32_t file_index=UINT32_MAX,matches=0u;
    for(uint32_t i=0;i<FF_FATX_FILES;++i)if(fatx_volume.files[i].handle==f[1]){file_index=i;++matches;}
    if(matches!=1u)stop(9,f[1],0x3417au);
    const ff_fatx_file *file=&fatx_volume.files[file_index];
    if(file->id!=directory_cursor.file_id || file->id!=0x5000000eu || file->mount!=directory_cursor.mount ||
       file->mount!=1u || file->references!=1u || file->kind!=0u || file->access!=0x100001u ||
       file->share!=3u || file->options!=0x4021u || file->mount>fatx_volume.mount_count)stop(9,f[1],0x3417au);
    const uint32_t mi=file->mount-1u;const ff_fatx_mount *mount=&fatx_volume.mounts[mi];
    uint32_t count=0u;
    if(mount->id!=file->mount || mount->device!=directory_cursor.device || mount->device!=11u ||
       mount->serial!=directory_cursor.serial || mount->offset!=directory_cursor.mount_offset ||
       mount->bytes!=directory_cursor.mount_bytes || mount->data_offset!=directory_cursor.mount_data_offset ||
       mount->root_cluster!=directory_cursor.root_cluster || mount->root_clusters!=directory_cursor.root_clusters ||
       mount->cluster_bytes!=directory_cursor.cluster_bytes || mount->entry_bytes!=directory_cursor.entry_bytes ||
       mount->revision!=local_disk.revision || mount->file_objects!=1u ||
       ff_fatx_root_count(&fatx_volume,mount->id,&count) || count!=1u)stop(9,f[1],0x3417au);
    uint32_t token=0u,index=UINT32_MAX;matches=0u;
    for(uint32_t i=0;i<FF_KERNEL_BINDING_COUNT;++i)if(bindings[i].ordinal==187u){
        if(bindings[i].slot!=0x253378u)stop(9,bindings[i].slot,187u);
        index=i;token=FF_KERNEL_BASE+4u*i;++matches;
    }
    if(matches!=1u || sdk_error_read32(0x253378u,s)!=token || recomp_lookup_kernel(token)!=close_handle)stop(9,token,187u);
    w[3]=g_esp;w[4]=f[0];w[5]=f[1];w[6]=g_eax;w[7]=g_ecx;w[8]=g_edx;sdk_error_snapshot(w,9u);
    w[16]=0x214u;w[17]=0x253378u;w[18]=token;w[19]=index;w[20]=file_index;w[21]=file->id;
    w[22]=mi;w[23]=mount->id;w[24]=mount->device;w[25]=file->references;
    w[26]=fatx_volume.file_count;w[27]=fatx_volume.active;w[28]=fatx_volume.opened;w[29]=fatx_volume.closed;
    w[30]=kernel_call_count;w[46]=mount->file_objects;w[48]=(uint32_t)mount->revision;w[49]=(uint32_t)(mount->revision>>32);
    for(uint32_t i=0;i<6u;++i)w[50u+i]=ff_probe_abi_violation_word(i);
    for(uint32_t i=0;i<3u;++i)w[64u+i]=sdk_error_read32(w[3]+8u+4u*i,s);
    w[67]=sdk_error_read32(startup_words[14],s);
    if(w[64]!=startup_words[19] || w[65]!=startup_words[18] || w[66]!=startup_words[20] || w[67]!=startup_words[15])stop(9,g_esp,0x3417au);
    ff_fatx_volume expected;memcpy(&expected,&fatx_volume,sizeof expected);
    expected.file_count--;expected.active--;expected.closed++;expected.mounts[mi].file_objects--;
    memset(&expected.files[file_index],0,sizeof expected.files[file_index]);
    w[79]=t->id;w[0]=1u;
    ff_title_0003417A(); /* Original instructions and existing typed close service, unchanged. */
    if(sdk_thread()!=t || g_esp!=w[3]+8u || g_eax!=1u || g_ecx!=w[7] || g_edx!=w[8] ||
       kernel_call_count!=w[30]+1u || dpc_frame.active || irql_pair.active)stop(9,g_esp,0x3417au);
    sdk_error_preserved(w,9u,0x3417au);
    for(uint32_t i=0;i<6u;++i){w[56u+i]=ff_probe_abi_violation_word(i);if(w[56u+i]!=w[50u+i])stop(9,g_esp,0x3417au);}
    const uint32_t *row=kernel_calls[w[30]];
    if(row[0]!=187u || row[1] || row[2]!=w[5] || memcmp(&expected,&fatx_volume,sizeof expected))stop(9,g_esp,187u);
    w[31]=kernel_call_count;w[32]=g_esp;w[33]=g_eax;w[34]=g_ecx;w[35]=g_edx;sdk_error_snapshot(w,36u);
    w[43]=row[0];w[44]=row[1];w[45]=row[2];w[47]=fatx_volume.mounts[mi].file_objects;w[63]=1u;
    ff_sdk_shared_read(s); /* Publish actual C-return scalar state before any SDK-accounted reread. */
    if(memcmp(ff_sdk_shared_memory(t->tls,20u,FF_VM_READ,s),error_tls_after,20u))stop(9,t->tls,0x3417au);
    w[62]=1u;w[1]=1u;ff_sdk_shared_read(s);
}
static void sdk_close_observe(uint32_t guest,ff_sdk_scalar *s)
{
    uint32_t *w=close_words,*o=close_parent_words;(void)sdk_thread();
    if(!sdk_close_profile() || w[0]!=1u || w[1]!=1u)stop(2,guest,0);
    if(guest==0x4ece2u){
        if(w[2] || g_esp!=w[32] || g_eax!=1u || g_ecx!=w[7] || g_edx!=w[8])stop(9,g_esp,guest);
        sdk_error_preserved(w,9u,guest);w[68]=g_esp;w[69]=g_eax;w[70]=g_ecx;w[71]=g_edx;sdk_error_snapshot(w,72u);w[2]=1u;
    }else if(guest==0x4eb50u){
        if(w[2]!=1u || o[0] || g_esp!=startup_words[14] || g_eax!=1u ||
           g_ebx!=w[9] || g_esi!=w[65] || g_edi!=w[64] || g_ebp!=w[66] ||
           g_ecx!=w[7] || g_edx!=w[8] || g_df || g_fs_base!=w[14] || g_seh_ebp!=w[15])stop(9,g_esp,guest);
        const uint32_t *f=sdk_continuation_frame(4u,guest,s);
        if(f[0]!=w[67] || f[0]!=0x4f603u)stop(9,g_esp,guest);
        o[2]=startup_words[14];o[3]=g_esp;o[4]=g_eax;o[5]=g_ebx;o[6]=g_esi;o[7]=g_edi;o[8]=g_ebp;
        o[9]=g_ecx;o[10]=g_edx;o[11]=(uint32_t)g_df;o[12]=g_fs_base;o[13]=g_seh_ebp;o[14]=f[0];o[0]=1u;
    }else if(guest==0x4f603u){
        if(o[0]!=1u || o[1] || g_esp!=o[3]+4u || g_eax!=o[4] ||
           g_ebx!=o[5] || g_esi!=o[6] || g_edi!=o[7] || g_ebp!=o[8] ||
           g_ecx!=o[9] || g_edx!=o[10] || (uint32_t)g_df!=o[11] || g_fs_base!=o[12] || g_seh_ebp!=o[13])stop(9,g_esp,guest);
        o[15]=g_esp;o[16]=g_eax;o[17]=g_ebx;o[18]=g_esi;o[19]=g_edi;o[20]=g_ebp;
        o[21]=g_ecx;o[22]=g_edx;o[23]=(uint32_t)g_df;o[24]=g_fs_base;o[25]=g_seh_ebp;o[26]=1u;o[1]=1u;
    }else stop(2,guest,0);
}




/* M37: original C301 call and unchanged C31155/31127 continuation. */
static ff_guest_cpu mapped_cpu_before;
static int sdk_denied_active(void){return (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u)))))))) && stdio_active && sdk_current_target==0x4f5e0u;}
static void sdk_denied_preserved(void)
{
    if(g_ebx!=mapped_cpu_before.r_ebx || g_esi!=mapped_cpu_before.r_esi ||
       g_edi!=mapped_cpu_before.r_edi || g_ebp!=mapped_cpu_before.r_ebp ||
       g_seh_ebp!=mapped_cpu_before.r_seh_ebp || g_df!=mapped_cpu_before.r_df ||
       g_fs_base!=mapped_cpu_before.r_fs_base || dpc_frame.active || irql_pair.active)
        stop(9,g_esp,301u);
}
static void sdk_denied_tls(uint32_t initial)
{
    ff_boot_thread *t=sdk_thread();uint32_t *w=mapped_words;ff_sdk_scalar a;ff_sdk_shared_read(&a);
    if(g_fs_base!=t->fs || g_df || dpc_frame.active || irql_pair.active ||
       t->tls_size!=20u || !t->tls || (uint64_t)t->tls+20u!=t->stack_top ||
       ff_vm_check(&virtual_memory,t->tls,20u,FF_VM_READ|FF_VM_WRITE) ||
       ff_vm_check(&virtual_memory,t->fs,0x2cu,FF_VM_READ) ||
       ff_vm_check(&virtual_memory,t->object+0x28u,4u,FF_VM_READ))stop(9,t->tls,301u);
    const uint32_t irql=*ff_sdk_shared_memory(t->fs+0x24u,1u,FF_VM_READ,&a);
    const uint32_t object=sdk_error_read32(t->fs+0x28u,&a),top=sdk_error_read32(t->fs+4u,&a);
    const uint32_t low=sdk_error_read32(t->fs+8u,&a),index=sdk_error_read32(0x2bb130u,&a);
    const int64_t slot=(int64_t)top+4*(int64_t)(int32_t)index;
    if(irql>=2u || object!=t->object || top!=t->stack_top || low!=t->stack_low ||
       sdk_error_read32(t->object+0x28u,&a)!=t->tls || slot<0 || slot>UINT32_MAX ||
       (uint32_t)slot!=t->tls || (uint32_t)(top+index*4u)!=(uint32_t)slot)stop(9,top,301u);
    const uint32_t record=sdk_error_read32((uint32_t)slot,&a);
    if(record!=t->tls+4u || (uint64_t)record+8u>(uint64_t)t->tls+20u ||
       ff_vm_check(&virtual_memory,record+4u,4u,FF_VM_READ|FF_VM_WRITE))stop(9,record,301u);
    if(initial){
        w[16]=t->id;w[17]=t->fs;w[18]=object;w[19]=top;w[20]=low;w[21]=index;
        w[22]=t->tls;w[23]=record+4u;w[24]=sdk_error_read32(record+4u,&a);w[25]=20u;w[26]=record;w[27]=irql;
        memcpy(mapped_tls_before,ff_sdk_shared_memory(t->tls,20u,FF_VM_READ,&a),20u);
    }else if(w[16]!=t->id || w[17]!=t->fs || w[18]!=object || w[19]!=top ||
        w[20]!=low || w[21]!=index || w[22]!=t->tls || w[23]!=record+4u || w[26]!=record || w[27]!=irql)
        stop(9,record,301u);
}
static void sdk_denied_snapshot(uint32_t offset)
{
    uint32_t *w=mapped_words;ff_boot_thread *t=sdk_thread();
    if(offset>112u || g_esp<t->stack_low || g_esp>t->tls-8u || (g_esp&3u) ||
       ff_vm_check(&virtual_memory,g_esp,8u,FF_VM_READ))stop(9,g_esp,301u);
    const uint32_t *f=(const uint32_t *)(guest_ram+g_esp);
    const uint32_t actual[16]={g_esp,g_eax,g_ecx,g_edx,g_ebx,g_esi,g_edi,g_ebp,(uint32_t)g_df,
        g_fs_base,g_seh_ebp,*(const uint32_t *)(guest_ram+g_fs_base),f[0],f[1],kernel_call_count,1u};
    memcpy(w+offset,actual,sizeof actual);
}
static void sdk_denied_tls_result(void)
{
    ff_sdk_scalar a;ff_sdk_shared_read(&a);uint32_t *w=mapped_words;
    sdk_denied_tls(0u);
    memcpy(mapped_tls_after,ff_sdk_shared_memory(w[22],20u,FF_VM_READ,&a),20u);
    uint8_t expected[20];memcpy(expected,mapped_tls_before,20u);
    const uint32_t offset=w[23]-w[22];if(offset>16u)stop(9,w[23],301u);
    for(uint32_t i=0;i<4u;++i)expected[offset+i]=(uint8_t)(5u>>(8u*i));
    if(memcmp(expected,mapped_tls_after,20u) || sdk_error_read32(w[23],&a)!=5u)stop(9,w[23],301u);
    w[123]=1u;w[124]=5u;
}
static uint32_t sdk_denied_map(const uint32_t *row,uint32_t *mapped)
{
    if(!sdk_denied_active() || row[2]!=0xc0000022u)return 0u;
    uint32_t *w=mapped_words;ff_boot_thread *t=sdk_thread();
    if(w[0] || row[0]!=301u || !title_disc.bound || title_disc.provider!=1u || title_disc.device!=4u ||
       title_disc.denied!=1u || disc_words[24]!=1u || stdio_words[0]!=1u || stdio_words[1] ||
       g_esp!=0x03c10d0cu || g_ebp!=0x03c10d44u || g_seh_ebp!=g_ebp || g_fs_base!=t->fs ||
       g_df || dpc_frame.active || irql_pair.active || g_esp<t->stack_low || g_esp>t->tls-16u ||
       ff_vm_check(&virtual_memory,g_esp,16u,FF_VM_READ|FF_VM_WRITE))stop(9,g_esp,301u);
    const uint32_t *f=(const uint32_t *)(guest_ram+g_esp);
    if(f[0]!=0x3115fu || f[1]!=row[2] || f[2]!=0x1e9518u || f[3]!=row[2] ||
       FF_KERNEL_BINDING_COUNT<=2u || bindings[2].ordinal!=301u || bindings[2].slot!=0x253370u)
        stop(9,g_esp,301u);
    ff_sdk_scalar a;ff_sdk_shared_read(&a);
    if(sdk_error_read32(0x253370u,&a)!=FF_KERNEL_BASE+8u ||
       recomp_lookup_kernel(FF_KERNEL_BASE+8u)!=status_to_dos_error)stop(9,0x253370u,301u);
    sdk_denied_tls(1u);
    w[8]=row[2];w[9]=5u;w[10]=g_esp;w[11]=f[0];w[12]=f[1];w[13]=g_esp+8u;w[14]=f[2];w[15]=f[3];
    w[28]=kernel_call_count;w[30]=g_ebp;w[31]=g_seh_ebp;w[36]=2u;w[37]=0x253370u;w[38]=FF_KERNEL_BASE+8u;
    ff_capture_cpu(&mapped_cpu_before);w[120]=1u;w[121]=title_disc.denied;w[127]=1u;w[0]=1u;
    *mapped=5u;return 1u; /* Existing handler performs and records the actual return. */
}
static void sdk_denied_return(const uint32_t *row)
{
    if(!sdk_denied_active() || row[2]!=0xc0000022u || !mapped_words[0])return;
    uint32_t *w=mapped_words;ff_guest_cpu actual,expected;ff_capture_cpu(&actual);
    memcpy(&expected,&mapped_cpu_before,sizeof expected);expected.r_eax=5u;expected.r_esp+=8u;
    if(w[1] || row[0]!=301u || row[1]!=5u || memcmp(&actual,&expected,sizeof actual))stop(9,g_esp,301u);
    sdk_denied_tls(0u);ff_sdk_scalar a;ff_sdk_shared_read(&a);
    memcpy(mapped_tls_kernel,ff_sdk_shared_memory(w[22],20u,FF_VM_READ,&a),20u);
    const uint32_t *f=(const uint32_t *)(guest_ram+w[10]);
    if(memcmp(mapped_tls_before,mapped_tls_kernel,20u) || f[0]!=w[11] || f[1]!=w[12] || f[2]!=w[14] || f[3]!=w[15])
        stop(9,w[10],301u);
    w[29]=kernel_call_count;w[32]=1u;w[33]=g_eax;w[34]=g_esp;w[35]=1u;w[39]=1u;w[1]=1u;
}
static void sdk_denied_trace(uint32_t kind,uint32_t address)
{
    s44_trace(kind,address);
    s45_trace(kind,address);
    if(!sdk_denied_active() || mapped_words[1]!=1u || (address!=0x31127u && address!=0x31155u))return;
    uint32_t *w=mapped_words;sdk_denied_preserved();sdk_denied_tls(0u);
    if(kind==1u && address==0x31127u){
        if(w[2] || g_esp!=w[10] || g_eax!=5u || g_edx!=5u)stop(9,g_esp,address);
        const uint32_t *f=(const uint32_t *)(guest_ram+g_esp);
        if(f[0]!=0x31167u || f[1]!=5u)stop(9,g_esp,address);
        ff_sdk_scalar a;ff_sdk_shared_read(&a);
        if(memcmp(mapped_tls_before,ff_sdk_shared_memory(w[22],20u,FF_VM_READ,&a),20u))stop(9,w[22],address);
        sdk_denied_snapshot(40u);w[2]=1u;
    }else if(kind==2u && address==0x31127u){
        if(w[2]!=1u || w[3] || g_esp!=w[10] || g_eax!=w[26] || g_ecx!=5u || g_edx!=5u)stop(9,g_esp,address);
        sdk_denied_tls_result();sdk_denied_snapshot(56u);w[3]=1u;
    }else if(kind==2u && address==0x31155u){
        if(w[4]!=1u || w[5] || g_esp!=w[13] || g_eax!=5u || g_ecx!=5u || g_edx!=5u)stop(9,g_esp,address);
        sdk_denied_tls_result();sdk_denied_snapshot(88u);w[5]=1u;
    }else stop(9,address,301u);
}
static void sdk_denied_esp(const char *name,const char *tag)
{
    s44_esp(name,tag);
    s45_ledger_esp(name,tag);
    if(sdk_denied_active() && (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u)))))) && dosmap_words[24]){if(!strcmp(name,"ff_title_001E75C8") && !strcmp(tag,"after call 0x001E4F99")){ff_probe_sdk_dosmap_continuation(0u);return;}if(!strcmp(name,"ff_title_001E75C8") && !strcmp(tag,"pop ecx")){ff_probe_sdk_dosmap_continuation(1u);return;}if(!strcmp(name,"ff_title_001E75C8") && !strcmp(tag,"pop edi")){ff_probe_sdk_dosmap_continuation(2u);return;}}
    if(!sdk_denied_active() || mapped_words[1]!=1u)return;
    uint32_t *w=mapped_words;
    if(!strcmp(name,"ff_title_001E75C8") && !strcmp(tag,"after call 0x000310FF")){ff_probe_sdk_tls_getter_continuation();return;}
    if(!strcmp(name,"ff_title_00031155") && !strcmp(tag,"after call 0x00031127")){
        sdk_denied_preserved();
        if(w[3]!=1u || w[4] || g_esp!=w[13] || g_eax!=w[26] || g_ecx!=5u || g_edx!=5u)stop(9,g_esp,0x31167u);
        sdk_denied_tls_result();sdk_denied_snapshot(72u);w[4]=1u;
    }else if(!strcmp(name,"ff_title_001E93E4") && !strcmp(tag,"after call 0x00031155")){
        sdk_denied_preserved();
        if(w[5]!=1u || w[6] || g_esp!=w[13]+8u || g_eax!=5u || g_ecx!=5u || g_edx!=5u)stop(9,g_esp,0x1e9518u);
        sdk_denied_tls_result();sdk_denied_snapshot(104u);w[6]=1u;
    }
}

/* M38 scoped TLS getter observation/provider support. */
static ff_guest_cpu tls_getter_cpu_before, tls_getter_cpu_after;
static uint32_t tls_getter_fs_byte, tls_getter_entry_eflags;
uint32_t ff_sdk_shared_read_eflags(void) { return g_eflags; }
void ff_sdk_shared_write_eflags(uint32_t flags) { g_eflags=flags; }
uint32_t ff_probe_sdk_tls_getter_begin(void)
{
    ff_boot_thread *t=sdk_thread();uint32_t *w=tls_getter_words;
    if((sdk_enabled!=38u && (sdk_enabled!=39u && (sdk_enabled!=40u && (sdk_enabled!=41u && (sdk_enabled!=42u && (sdk_enabled!=43u && (sdk_enabled!=44u && sdk_enabled!=45u))))))) || !sdk_denied_active() || !mapped_words[1] || !mapped_words[2] ||
       !mapped_words[3] || !mapped_words[4] || !mapped_words[5] || !mapped_words[6] ||
       w[0] || w[1] || w[2] || g_esp!=0x03c10d64u || g_fs_base!=t->fs || g_df ||
       dpc_frame.active || irql_pair.active || g_esp<t->stack_low || g_esp>=t->tls ||
       4u>t->tls-g_esp || (g_esp&3u) || ff_vm_check(&virtual_memory,g_esp,4u,FF_VM_READ) ||
       ff_vm_check(&virtual_memory,t->fs,0x2cu,FF_VM_READ))stop(9,g_esp,0x310ffu);
    ff_sdk_scalar s;ff_sdk_shared_read(&s);
    const uint32_t ra=sdk_error_read32(g_esp,&s);
    if(ra!=0x001e778cu)stop(9,g_esp,0x310ffu);
    sdk_denied_tls(0u);
    const uint8_t *irql=ff_sdk_shared_memory(t->fs+0x24u,1u,FF_VM_READ,&s);
    if(!irql)stop(9,t->fs+0x24u,0x310ffu);
    tls_getter_fs_byte=*irql;
    if(tls_getter_fs_byte<2u && ff_vm_check(&virtual_memory,t->fs+0x28u,4u,FF_VM_READ))
        stop(9,t->fs+0x28u,0x310ffu);
    w[29]=pfifo_consumer_vm_checks;
    sdk_shared_remaining_budget(256u,0x310ffu);w[31]=256u;
    if(ff_vm_check(&virtual_memory,0x310ffu,40u,FF_VM_EXECUTE) ||
       ff_vm_check(&virtual_memory,g_esp,8u,FF_VM_READ))stop(9,g_esp,0x310ffu);
    w[32]=40u;
    const uint32_t entry_sp=g_esp;
    recomp_trace_enter("sdk-tls-getter",0x310ffu);
    ff_sdk_shared_read(&s);
    if(g_esp!=entry_sp || g_fs_base!=t->fs || g_df || sdk_error_read32(g_esp,&s)!=ra)
        stop(9,g_esp,0x310ffu);
    tls_getter_entry_eflags=g_eflags;ff_capture_cpu(&tls_getter_cpu_before);
    memcpy(tls_getter_tls_before,ff_sdk_shared_memory(t->tls,20u,FF_VM_READ,&s),20u);
    w[0]=1u;w[3]=tls_getter_fs_byte;w[4]=g_esp;w[5]=ra;w[6]=g_eax;w[7]=g_ecx;
    w[8]=g_eflags;w[9]=g_ebp;w[10]=g_seh_ebp;w[11]=g_fs_base;w[12]=t->id;
    w[13]=t->stack_low;w[14]=t->stack_top;w[15]=t->tls;w[16]=t->tls_size;
    w[17]=sdk_error_read32(0x2bb130u,&s);w[18]=mapped_words[23];w[19]=mapped_words[26];
    w[20]=sdk_error_read32(mapped_words[23],&s);w[21]=1u;
    return 1u;
}
uint32_t ff_probe_sdk_tls_getter_fs_byte(void) { return tls_getter_fs_byte; }
uint32_t ff_probe_sdk_tls_getter_eflags(void) { return tls_getter_entry_eflags; }
uint32_t ff_probe_sdk_tls_getter_armed(void)
{ return tls_getter_words[0]==1u && tls_getter_words[1]==0u && tls_getter_words[2]==0u; }
void ff_probe_sdk_tls_getter_complete(uint32_t eax,uint32_t ecx,uint32_t esp,uint32_t flags)
{
    uint32_t *w=tls_getter_words;ff_guest_cpu expected;
    if(w[0]!=1u || w[1] || w[2] || !mapped_words[1])stop(9,g_esp,0x310ffu);
    ff_capture_cpu(&tls_getter_cpu_after);memcpy(&expected,&tls_getter_cpu_before,sizeof expected);
    expected.r_eax=w[20];expected.r_ecx=w[14];expected.r_esp=w[4]+4u;expected.r_eflags=flags;
    if(eax!=expected.r_eax || ecx!=expected.r_ecx || esp!=expected.r_esp ||
       g_eax!=expected.r_eax || g_ecx!=expected.r_ecx || g_esp!=expected.r_esp ||
       g_eflags!=flags || memcmp(&tls_getter_cpu_after,&expected,sizeof expected))
        stop(9,g_esp,0x310ffu);
    ff_sdk_scalar s;ff_sdk_shared_read(&s);sdk_denied_tls(0u);
    if(memcmp(tls_getter_tls_before,ff_sdk_shared_memory(w[15],20u,FF_VM_READ,&s),20u) ||
       sdk_error_read32(w[18],&s)!=w[20])stop(9,w[18],0x310ffu);
    recomp_trace_exit("sdk-tls-getter-post-ret",0x310ffu);
    w[30]=pfifo_consumer_vm_checks;
    w[1]=1u;w[22]=1u;w[23]=1u;w[24]=1u;w[25]=flags;w[26]=eax;w[27]=ecx;w[28]=esp;
}
void ff_probe_sdk_tls_getter_continuation(void)
{
    uint32_t *w=tls_getter_words;ff_guest_cpu after;
    if(w[0]!=1u || w[1]!=1u || w[2] || g_esp!=w[4]+4u || g_eax!=w[26] ||
       g_ecx!=w[27] || g_ebp!=w[9] || g_seh_ebp!=w[10] || g_fs_base!=w[11])
        stop(9,g_esp,0x310ffu);
    ff_capture_cpu(&after);
    if(memcmp(&after,&tls_getter_cpu_after,sizeof after))stop(9,g_esp,0x310ffu);
    w[2]=1u;
}

/* M39 exact DOS mapper proof hooks; no guessed outputs are written. */
static ff_guest_cpu dosmap_cpu_before,dosmap_cpu_after;
static ff_guest_cpu dosmap_child_cpu_before[2];
static uint8_t dosmap_child_ptd_before[2][132];
static uint32_t dosmap_entry_eflags;
uint32_t ff_probe_sdk_dosmap_precheck(void)
{
    ff_boot_thread *t=sdk_thread();ff_sdk_scalar s;ff_sdk_shared_read(&s);
    if(dosmap_words[0] || dosmap_words[19] || dosmap_words[24] || (sdk_enabled!=39u && (sdk_enabled!=40u && (sdk_enabled!=41u && (sdk_enabled!=42u && (sdk_enabled!=43u && (sdk_enabled!=44u && sdk_enabled!=45u)))))) || !sdk_denied_active() || sdk_current_target!=0x4f5e0u ||
       !tls_getter_words[1] || !tls_getter_words[2] || g_esp!=0x03c10d60u ||
       g_fs_base!=t->fs || g_df || dpc_frame.active || irql_pair.active ||
       g_esp<t->stack_low || g_esp>t->stack_top || 16u>g_esp-t->stack_low || 8u>t->stack_top-g_esp ||
       ff_vm_check(&virtual_memory,g_esp-16u,24u,FF_VM_READ|FF_VM_WRITE) ||
       ff_vm_check(&virtual_memory,0x001e4f99u,115u,FF_VM_EXECUTE) ||
       ff_vm_check(&virtual_memory,0x001e2038u,130u,FF_VM_EXECUTE) ||
       ff_vm_check(&virtual_memory,0x002b7888u,360u,FF_VM_READ))return 0u;
    sdk_denied_tls(0u);
    const uint32_t ra=sdk_error_read32(g_esp,&s),arg=sdk_error_read32(g_esp+4u,&s);
    const uint8_t *irql=ff_sdk_shared_memory(t->fs+0x24u,1u,FF_VM_READ,&s);
    const uint32_t ptd=sdk_error_read32(t->tls+12u,&s);
    return ra==0x001e7792u && arg==5u && irql && *irql==0u && ptd==0x00330690u &&
       ff_vm_check(&virtual_memory,ptd,132u,FF_VM_READ|FF_VM_WRITE)==0;
}
uint32_t ff_probe_sdk_dosmap_begin(void)
{
    uint32_t *w=dosmap_words;ff_boot_thread *t=sdk_thread();ff_sdk_scalar s;
    if(w[0] || w[19] || w[24])stop(9,g_esp,0x001e4f99u);
    if((sdk_enabled!=39u && (sdk_enabled!=40u && (sdk_enabled!=41u && (sdk_enabled!=42u && (sdk_enabled!=43u && (sdk_enabled!=44u && sdk_enabled!=45u)))))) || !sdk_denied_active() || sdk_current_target!=0x4f5e0u ||
       !tls_getter_words[1] || !tls_getter_words[2] || g_esp!=0x03c10d60u ||
       g_fs_base!=t->fs || g_df || dpc_frame.active || irql_pair.active ||
       g_esp<t->stack_low || g_esp>t->stack_top || 16u>g_esp-t->stack_low || 8u>t->stack_top-g_esp ||
       ff_vm_check(&virtual_memory,g_esp-16u,24u,FF_VM_READ|FF_VM_WRITE) ||
       ff_vm_check(&virtual_memory,0x001e4f99u,115u,FF_VM_EXECUTE) ||
       ff_vm_check(&virtual_memory,0x001e2038u,130u,FF_VM_EXECUTE) ||
       ff_vm_check(&virtual_memory,0x002b7888u,360u,FF_VM_READ))stop(9,g_esp,0x001e4f99u);
    ff_sdk_shared_read(&s);
    sdk_denied_tls(0u);
    const uint32_t ra=sdk_error_read32(g_esp,&s),arg=sdk_error_read32(g_esp+4u,&s);
    if(ra!=0x001e7792u || arg!=5u)stop(9,g_esp,0x001e4f99u);
    const uint8_t *irql=ff_sdk_shared_memory(t->fs+0x24u,1u,FF_VM_READ,&s);
    if(!irql || *irql!=0u)stop(9,t->fs+0x24u,0x001e2038u);
    const uint32_t ptd=sdk_error_read32(t->tls+12u,&s);
    if(ptd!=0x00330690u || ff_vm_check(&virtual_memory,ptd,132u,FF_VM_READ|FF_VM_WRITE))stop(9,ptd,0x001e2038u);
    uint8_t *before=ff_sdk_shared_memory(ptd,132u,FF_VM_READ,&s);
    const uint8_t *table=ff_sdk_shared_memory(0x002b7888u,360u,FF_VM_READ,&s);
    if(!before || !table)stop(9,ptd,0x001e4f99u);
    memcpy(dosmap_ptd_before,before,132u);
    w[0]=1u;w[1]=g_esp;w[2]=ra;w[3]=arg;w[4]=t->id;w[5]=t->fs;w[6]=t->tls;w[7]=ptd;
    w[8]=*irql;w[9]=0x002b7888u;w[10]=360u;w[11]=0x001e4f99u;w[12]=115u;
    w[13]=0x001e2038u;w[14]=130u;
    const uint32_t table_key=sdk_error_read32(0x002b7888u+4u*4u*2u,&s);
    w[15]=sdk_error_read32(0x002b7888u+4u*4u*2u+4u,&s);
    if(table_key!=5u || w[15]!=13u)stop(9,0x002b7888u,0x001e4f99u);
    for(uint32_t i=0;i<4u;++i)if(sdk_error_read32(0x002b7888u+8u*i,&s)==5u)stop(9,0x002b7888u+8u*i,0x001e4f99u);
    w[16]=sdk_error_read32(ptd+8u,&s);w[17]=sdk_error_read32(ptd+12u,&s);
    w[18]=pfifo_consumer_vm_checks;
    sdk_shared_remaining_budget(256u,0x001e4f99u);
    recomp_trace_enter("sdk-dosmaperr",0x001e4f99u);
    ff_capture_cpu(&dosmap_cpu_before);dosmap_entry_eflags=g_eflags;
    w[19]=1u;return 1u;
}
uint32_t ff_probe_sdk_dosmap_child(uint32_t target,const ff_sdk_scalar *current)
{
    uint32_t *w=dosmap_words;ff_boot_thread *t=sdk_thread();ff_sdk_scalar s;
    if(current){s=*current;ff_sdk_shared_write(current);}else memset(&s,0,sizeof s);
    if(!current || w[0]!=1u || !w[19] || w[1]!=0x03c10d60u || s.fs_base!=t->fs || s.df)stop(9,s.esp,target);
    if(target==0x001e4f99u){if(w[27]++ || s.esp!=w[1] || sdk_error_read32(s.esp,&s)!=w[2])stop(9,s.esp,target);return 1u;}
    if(target==0x001e2038u){
        const uint8_t *irql=ff_sdk_shared_memory(t->fs+0x24u,1u,FF_VM_READ,&s);
        if(s.esp!=0x03c10d58u || s.esp<t->stack_low || s.esp>t->stack_top || 4u>t->stack_top-s.esp ||
           ff_vm_check(&virtual_memory,s.esp,4u,FF_VM_READ) || !irql || *irql!=0u ||
           sdk_error_read32(t->tls+12u,&s)!=w[7])stop(9,t->fs+0x24u,target);
        if((w[20]==0u && sdk_error_read32(s.esp,&s)!=0x001e4f9fu) ||
           (w[20]==1u && sdk_error_read32(s.esp,&s)!=0x001e4fd4u) || w[20]>1u ||
           w[7]==0u || w[8]!=0u)stop(9,s.esp,target);
        const uint32_t n=w[20],base=32u+48u*n,ra=sdk_error_read32(s.esp,&s);
        if(w[29]!=n || w[base] || 8u>s.esp-t->stack_low || 8u>t->stack_top-s.esp ||
           ff_vm_check(&virtual_memory,s.esp-8u,16u,FF_VM_READ|FF_VM_WRITE) ||
           ff_vm_check(&virtual_memory,target,130u,FF_VM_EXECUTE))stop(9,s.esp,target);
        sdk_shared_remaining_budget(256u,target);
        recomp_trace_enter("sdk-ptd-accessor",target);
        ff_sdk_shared_read(&s);(void)sdk_thread();
        const uint8_t *after_irql=ff_sdk_shared_memory(t->fs+0x24u,1u,FF_VM_READ,&s);
        if(s.esp!=0x03c10d58u || s.fs_base!=t->fs || s.df || !after_irql || *after_irql ||
           sdk_error_read32(s.esp,&s)!=ra || sdk_error_read32(t->tls+12u,&s)!=w[7])stop(9,s.esp,target);
        ff_capture_cpu(&dosmap_child_cpu_before[n]);
        memcpy(dosmap_child_ptd_before[n],ff_sdk_shared_memory(w[7],132u,FF_VM_READ,&s),132u);
        w[base]=1u;w[base+2u]=s.esp;w[base+3u]=ra;w[base+4u]=s.eax;w[base+5u]=s.ecx;
        w[base+6u]=s.edx;w[base+7u]=s.esi;w[base+8u]=s.edi;w[base+9u]=g_eflags;
        ++w[20];return 1u;
    }
    if(target==0x001e6aafu || target==0x000343f3u || target==0x001e564eu)
        stop(9,g_esp,target);
    return 0u;
}
void ff_probe_sdk_dosmap_child_return(const ff_sdk_scalar *actual)
{
    uint32_t *w=dosmap_words;
    if(!actual || w[20]<1u || w[20]>2u || w[29]+1u!=w[20])stop(9,g_esp,0x001e2038u);
    ff_sdk_shared_write(actual);ff_boot_thread *t=sdk_thread();
    const uint32_t n=w[20]-1u,base=32u+48u*n;
    if(w[base]!=1u || w[base+1u])stop(9,g_esp,0x001e2038u);
    ff_guest_cpu after,expected;ff_capture_cpu(&after);
    memcpy(&expected,&dosmap_child_cpu_before[n],sizeof expected);
    expected.r_eax=w[7];expected.r_ecx=t->stack_top;expected.r_esp=w[base+2u]+4u;
    if(actual->eax!=w[7] || actual->ecx!=t->stack_top || actual->esp!=expected.r_esp ||
       memcmp(&after,&expected,sizeof after))stop(9,g_esp,0x001e2038u);
    ff_sdk_scalar s;ff_sdk_shared_read(&s);sdk_denied_tls(0u);
    if(sdk_error_read32(w[base+2u],&s)!=w[base+3u] ||
       memcmp(dosmap_child_ptd_before[n],ff_sdk_shared_memory(w[7],132u,FF_VM_READ,&s),132u) ||
       memcmp(tls_getter_tls_before,ff_sdk_shared_memory(w[6],20u,FF_VM_READ,&s),20u))stop(9,w[7],0x001e2038u);
    recomp_trace_exit("sdk-ptd-accessor-post-ret",0x001e2038u);
    w[base+1u]=1u;w[base+16u]=actual->esp;w[base+17u]=actual->eax;w[base+18u]=actual->ecx;
    w[base+19u]=g_eflags;w[base+20u]=1u;++w[29];
}
void ff_probe_sdk_dosmap_complete(const ff_sdk_scalar *actual)
{
    uint32_t *w=dosmap_words;ff_sdk_scalar s;ff_guest_cpu after;
    if(!actual || w[0]!=1u || w[19]!=1u || w[20]!=2u || w[29]!=2u || w[27]!=1u || actual->esp!=w[1]+4u ||
       actual->eax!=w[7] || actual->ecx!=w[15] || actual->edx!=dosmap_cpu_before.r_edx ||
       actual->ebx!=dosmap_cpu_before.r_ebx || actual->ebp!=dosmap_cpu_before.r_ebp ||
       actual->esi!=dosmap_cpu_before.r_esi || actual->edi!=dosmap_cpu_before.r_edi ||
       actual->fs_base!=w[5] || actual->df)stop(9,actual?actual->esp:0u,0x001e4f99u);
    ff_sdk_shared_read(&s);sdk_denied_tls(0u);
    uint8_t *ptd=ff_sdk_shared_memory(w[7],132u,FF_VM_READ,&s);
    if(!ptd || memcmp(ptd,dosmap_ptd_before,8u) ||
       sdk_error_read32(w[7]+8u,&s)!=w[15] || memcmp(ptd+16u,dosmap_ptd_before+16u,116u) ||
       sdk_error_read32(w[7]+12u,&s)!=5u ||
       memcmp(tls_getter_tls_before,ff_sdk_shared_memory(w[6],20u,FF_VM_READ,&s),20u))stop(9,w[7],0x001e4f99u);
    memcpy(dosmap_ptd_after,ptd,132u);ff_capture_cpu(&after);
    ff_guest_cpu expected;memcpy(&expected,&dosmap_cpu_before,sizeof expected);
    expected.r_eax=w[7];expected.r_ecx=w[15];expected.r_esp=w[1]+4u;expected.r_eflags=dosmap_entry_eflags;
    if(memcmp(&after,&expected,sizeof expected))stop(9,w[7],0x001e4f99u);
    /* This function's caller overwrites arithmetic flags before its next
       original read; preserve the incoming C flag image, never synthesize flags. */
    (void)after;(void)dosmap_entry_eflags;
    recomp_trace_exit("sdk-dosmaperr-post-ret",0x001e4f99u);
    w[21]=actual->eax;w[22]=actual->ecx;w[23]=actual->esp;w[24]=1u;
}
void ff_probe_sdk_dosmap_continuation(uint32_t kind)
{
    uint32_t *w=dosmap_words;
    if(w[24]!=1u)stop(9,g_esp,0x001e4f99u);
    if(kind==0u){if(g_esp!=0x03c10d64u || g_eax!=w[21] || g_ecx!=w[22])stop(9,g_esp,0x001e4f99u);w[26]=1u;}
    else if(kind==1u){if(!w[26] || g_esp!=0x03c10d64u || g_eax!=w[21] || g_ecx!=w[22])stop(9,g_esp,0x001e4f99u);w[25]=1u;}
    else if(kind==2u){
        if(!w[26] || !w[25] || g_esp!=0x03c10d68u || g_ecx!=5u ||
           g_eax!=g_esi || (int32_t)g_eax>=0)stop(9,g_esp,0x001e4f99u);
        w[28]=1u;
    }else stop(9,g_esp,0x001e4f99u);
}

/* Host validates/retains the actual local Blob; C stages an opaque capability.
 * No readiness mutation before original namespace/link history is complete. */
uint32_t ff_probe_sdk_disc_prepare(uint32_t provider,uint32_t low,uint32_t high)
{
    const uint64_t bytes=low|((uint64_t)high<<32);
    if(provider!=1u || bytes!=2084569088ull || !namespace_ready || kernel_call_count ||
       sdk_enabled || sdk_active || stdio_active || disc_words[0] || disc_words[1] || disc_words[26] || title_disc.bound)
        return FF_BOOT_UNSUPPORTED;
    ff_namespace_resolution physical={0};
    if(ff_namespace_resolve(&object_namespace,"\\Device\\CdRom0",14u,&physical) ||
       physical.object!=4u || physical.remaining_bytes)return FF_BOOT_UNSUPPORTED;
    disc_words[0]=1u;disc_words[1]=provider;disc_words[2]=low;disc_words[3]=high;
    disc_words[5]=physical.object;
    return 0u;
}
uint32_t ff_probe_sdk_disc_word(uint32_t index)
{
    if(index==4u)return title_disc.bound;
    if(index==5u)return title_disc.bound?title_disc.device:disc_words[5];
    if(index==6u)return title_disc.denied;
    return index<32u?disc_words[index]:0u;
}
uint32_t ff_probe_sdk_disc_release(uint32_t provider)
{
    if(!disc_words[0] || provider!=disc_words[1])return FF_BOOT_UNSUPPORTED;
    if(title_disc.bound){
        ff_namespace_resolution res={0};
        if(title_disc.space!=&object_namespace || title_disc.provider!=provider ||
           ff_namespace_resolve(&object_namespace,"\\??\\D:",6u,&res) || res.remaining_bytes ||
           res.object!=title_disc.device)return FF_BOOT_UNSUPPORTED;
        const ff_namespace_object *d=0;
        for(uint32_t i=0;i<FF_NS_OBJECTS;++i)if(object_namespace.objects[i].id==res.object)d=&object_namespace.objects[i];
        if(!d || d->kind!=FF_NS_DEVICE || d->device_kind!=FF_NS_LOCAL_DISC ||
           d->media_ready!=1u || d->provider_object!=provider)return FF_BOOT_UNSUPPORTED;
    }
    ff_disc_reset(&title_disc);disc_words[0]=0u;disc_words[26]=1u;return 0u;
}
static void sdk_disc_bind_reached(void)
{
    (void)sdk_thread();
    if((sdk_enabled!=36u && (sdk_enabled!=37u && (sdk_enabled!=38u && (sdk_enabled!=39u && (sdk_enabled!=40u && (sdk_enabled!=41u && (sdk_enabled!=42u && (sdk_enabled!=43u && (sdk_enabled!=44u && sdk_enabled!=45u))))))))) || sdk_current_target!=0x4f5e0u || disc_words[0]!=1u ||
       disc_words[1]!=1u || title_disc.bound || disc_words[26] || !namespace_ready ||
       object_namespace.memory!=&virtual_memory || virtual_memory.ram!=guest_ram)
        stop(9,0u,190u);
    ff_namespace_resolution res={0};
    if(ff_namespace_resolve(&object_namespace,"\\??\\D:",6u,&res) || res.object!=4u || res.remaining_bytes)
        stop(9,res.object,190u);
    ff_object_namespace expected;memcpy(&expected,&object_namespace,sizeof expected);
    uint32_t matches=0u;
    for(uint32_t i=0;i<FF_NS_OBJECTS;++i)if(expected.objects[i].id==4u){
        ff_namespace_object *d=&expected.objects[i];
        if(d->kind!=FF_NS_DEVICE || d->device_kind!=FF_NS_LOCAL_DISC || d->media_ready || d->provider_object)
            stop(9,4u,190u);
        d->media_ready=1u;d->provider_object=disc_words[1];++matches;
    }
    if(matches!=1u)stop(9,matches,190u);
    const uint32_t status=ff_disc_bind(&title_disc,&object_namespace,disc_words[1],
        disc_words[2]|((uint64_t)disc_words[3]<<32));
    if(status || title_disc.device!=4u || memcmp(&expected,&object_namespace,sizeof expected))stop(9,status,190u);
    disc_words[27]=1u;
}
static ff_guest_cpu disc_cpu_before;
static uint32_t sdk_disc_provider(const uint32_t *args,uint32_t *status)
{
    if((sdk_enabled!=36u && (sdk_enabled!=37u && (sdk_enabled!=38u && (sdk_enabled!=39u && (sdk_enabled!=40u && (sdk_enabled!=41u && (sdk_enabled!=42u && (sdk_enabled!=43u && (sdk_enabled!=44u && sdk_enabled!=45u))))))))) || !stdio_active)return 0u;
    ff_boot_thread *t=sdk_thread();
    if(sdk_current_target!=0x4f5e0u || disc_words[0]!=1u || !title_disc.bound ||
       title_disc.space!=&object_namespace || disc_words[7] || g_esp!=0x03c10cf4u ||
       g_ebp!=0x03c10d44u || g_seh_ebp!=g_ebp || g_fs_base!=t->fs || g_df ||
       g_esp<t->stack_low || g_esp>t->tls-40u ||
       ff_vm_check(&virtual_memory,g_esp,40u,FF_VM_READ|FF_VM_WRITE))stop(9,g_esp,190u);
    const uint32_t *f=(const uint32_t *)(guest_ram+g_esp);
    const uint32_t expected[9]={0x03c10d40u,0x40100080u,0x03c10d24u,0x03c10d30u,0u,0x80u,3u,5u,0x60u};
    if(f[0]!=0x1e950cu || memcmp(args,expected,sizeof expected) || memcmp(f+1,args,sizeof expected) ||
       kernel_call_count!=stdio_words[27]+8u || ff_vm_check(&virtual_memory,args[0],4u,FF_VM_READ|FF_VM_WRITE) ||
       ff_vm_check(&virtual_memory,args[3],8u,FF_VM_READ|FF_VM_WRITE))stop(9,g_esp,190u);
    disc_words[10]=g_esp;disc_words[11]=f[0];disc_words[12]=*(const uint32_t *)(guest_ram+args[0]);
    disc_words[16]=kernel_call_count;disc_words[17]=g_eax;disc_words[18]=g_ebp;disc_words[19]=g_seh_ebp;
    disc_words[20]=g_fs_base;disc_words[21]=(uint32_t)g_df;disc_words[22]=1u;
    ff_capture_cpu(&disc_cpu_before);
    *status=ff_disc_create(&title_disc,args);
    if(*status!=0xc0000022u || title_disc.denied!=1u)stop(9,*status,190u);
    disc_words[7]=1u;disc_words[8]=*status;disc_words[23]=1u;
    return 1u; /* Existing190handler owns actual EAX/ESP return and telemetry. */
}
static void sdk_disc_return(const uint32_t *row)
{
    if((sdk_enabled!=36u && (sdk_enabled!=37u && (sdk_enabled!=38u && (sdk_enabled!=39u && (sdk_enabled!=40u && (sdk_enabled!=41u && (sdk_enabled!=42u && (sdk_enabled!=43u && (sdk_enabled!=44u && sdk_enabled!=45u))))))))) || !stdio_active || !disc_words[7])return;
    if(disc_words[24] || row[0]!=190u || row[1]!=0xc0000022u || g_eax!=row[1] ||
       g_esp!=disc_words[10]+40u)stop(9,g_esp,190u);
    ff_guest_cpu actual,expected;ff_capture_cpu(&actual);memcpy(&expected,&disc_cpu_before,sizeof expected);
    expected.r_eax=0xc0000022u;expected.r_esp+=40u;
    if(memcmp(&actual,&expected,sizeof actual))stop(9,g_esp,190u);
    const uint32_t *handle=(const uint32_t *)(guest_ram+row[2]);
    const uint32_t *iosb=(const uint32_t *)(guest_ram+row[5]);
    if(*handle!=disc_words[12] || iosb[0]!=0xc0000022u || iosb[1])stop(9,row[5],190u);
    disc_words[9]=g_esp;disc_words[13]=*handle;disc_words[14]=iosb[0];disc_words[15]=iosb[1];disc_words[24]=1u;
}

static const uint8_t stdio_expected_inputs[19]={100,58,92,95,84,95,69,95,83,95,84,46,95,95,95,0,119,98,0};

static int sdk_stdio_profile(void){return (sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u)))))))))) && sdk_current_target==0x4f5e0u;}
static uint32_t sdk_stdio_critical(ff_boot_runtime *rt,uint32_t address,uint32_t enter)
{
    if(!stdio_active)return enter?ff_boot_enter_critical(rt,address):ff_boot_leave_critical(rt,address);
    uint32_t *w=stdio_words;ff_boot_thread *t=sdk_thread();
    if(!sdk_stdio_profile() || rt!=&boot || g_fs_base!=t->fs || dpc_frame.active || irql_pair.active ||
       g_esp<t->stack_low || g_esp>t->tls-8u || (g_esp&3u) ||
       ff_vm_check(&virtual_memory,g_esp,8u,FF_VM_READ) || !address || (address&3u) ||
       ff_vm_check(&virtual_memory,address,28u,FF_VM_READ|FF_VM_WRITE) || w[61]>=32u){++w[124];return FF_BOOT_UNSUPPORTED;}
    const uint32_t *f=(const uint32_t *)(guest_ram+g_esp);
    if(f[1]!=address || ff_vm_check(&virtual_memory,f[0],1u,FF_VM_EXECUTE)){++w[124];return FF_BOOT_UNSUPPORTED;}
    uint32_t *e=w+128u+16u*w[61]++;const uint32_t *p=(const uint32_t *)(guest_ram+address);
    e[0]=enter?277u:294u;e[1]=address;e[2]=g_esp;e[3]=f[0];e[4]=p[4];e[5]=p[5];e[6]=p[6];
    e[11]=t->id;e[12]=kernel_call_count;e[13]=f[1];e[14]=1u;
    const uint32_t result=enter?ff_boot_enter_critical(rt,address):ff_boot_leave_critical(rt,address);
    e[7]=result;e[8]=p[4];e[9]=p[5];e[10]=p[6];e[15]=1u;return result;
}
static uint32_t sdk_stdio_enter(ff_boot_runtime *rt,uint32_t address){return sdk_stdio_critical(rt,address,1u);}
static uint32_t sdk_stdio_leave(ff_boot_runtime *rt,uint32_t address){return sdk_stdio_critical(rt,address,0u);}
extern void ff_title_001E01AD(void);
static void sdk_stdio_lock_preflight(uint32_t slot,uint8_t *snapshot,ff_sdk_scalar *s)
{
    const uint32_t pointer=sdk_error_read32(slot,s);
    if(!pointer || (pointer&3u) || ff_vm_check(&virtual_memory,pointer,28u,FF_VM_READ|FF_VM_WRITE))stop(9,pointer,277u);
    memcpy(snapshot,ff_sdk_shared_memory(pointer,28u,FF_VM_READ,s),28u);
    const uint32_t *v=(const uint32_t *)(guest_ram+pointer);
    if(v[4]!=UINT32_MAX || v[5] || v[6])stop(9,pointer,277u);
}
void ff_sdk_shared_c_1e01ad(ff_sdk_scalar *s)
{
    ff_sdk_shared_write(s);ff_boot_thread *t=sdk_thread();uint32_t *w=stdio_words;
    if(!sdk_stdio_profile() || w[0] || stdio_active || close_parent_words[1]!=1u ||
       g_esp!=0x03c10e54u || g_ebp!=0x03c10e58u || g_seh_ebp!=g_ebp || g_fs_base!=t->fs ||
       dpc_frame.active || irql_pair.active || g_df || t->tls_size!=20u ||
       g_esp<t->stack_low+0x1000u || g_esp>t->tls-12u ||
       ff_vm_check(&virtual_memory,t->stack_low,g_esp+12u-t->stack_low,FF_VM_READ|FF_VM_WRITE))stop(9,g_esp,0x1e01adu);
    if(g_ebx!=close_parent_words[17] || g_esi!=close_parent_words[18] || g_edi!=close_parent_words[19] ||
       g_ebp!=close_parent_words[20] || (uint32_t)g_df!=close_parent_words[23] ||
       g_fs_base!=close_parent_words[24] || g_seh_ebp!=close_parent_words[25])stop(9,g_esp,0x1e01adu);
    sdk_shared_remaining_budget(1024u,0x1e01adu);
    const uint32_t *f=sdk_continuation_frame(12u,0x1e01adu,s);
    if(f[0]!=0x4f612u || f[1]!=0x25c908u || f[2]!=0x258ec4u)stop(9,g_esp,0x1e01adu);
    memcpy(stdio_inputs,ff_sdk_shared_memory(f[1],16u,FF_VM_READ,s),16u);
    memcpy(stdio_inputs+16u,ff_sdk_shared_memory(f[2],3u,FF_VM_READ,s),3u);
    if(memcmp(stdio_inputs,stdio_expected_inputs,19u))stop(9,f[1],0x1e01adu);
    const uint32_t table=sdk_error_read32(0x31d960u,s),count=sdk_error_read32(0x31d964u,s);
    if(count!=512u || table!=0x330cd0u || ff_vm_check(&virtual_memory,table,2048u,FF_VM_READ))stop(9,table,0x1e52f6u);
    for(uint32_t i=0;i<4u;++i){
        const uint32_t pointer=sdk_error_read32(table+4u*i,s);
        if(pointer!=0x2b7608u+32u*i || ff_vm_check(&virtual_memory,pointer,32u,FF_VM_READ|(i==3u?FF_VM_WRITE:0u)))stop(9,pointer,0x1e52f6u);
        const uint32_t flag=*ff_sdk_shared_memory(pointer+12u,1u,FF_VM_READ,s);
        if((i<3u && !(flag&0x83u)) || (i==3u && (flag&0x83u)))stop(9,pointer,0x1e52f6u);
        w[64u+i]=pointer;w[68u+i]=flag;
    }
    const uint32_t lock1=sdk_error_read32(0x2b7b60u,s),lock19=sdk_error_read32(0x2b7bf0u,s);
    if(lock1!=0x315b1cu || lock19!=0x330c40u)stop(9,lock1,277u);
    sdk_stdio_lock_preflight(0x2b7b60u,stdio_locks_before,s);
    sdk_stdio_lock_preflight(0x2b7bf0u,stdio_locks_before+28u,s);
    uint32_t bound=0u;
    for(uint32_t i=0;i<FF_KERNEL_BINDING_COUNT;++i)if(bindings[i].ordinal==277u || bindings[i].ordinal==294u){
        const uint32_t slot=bindings[i].ordinal==277u?0x2533f0u:0x2533ecu;
        if(bindings[i].slot!=slot || sdk_error_read32(slot,s)!=FF_KERNEL_BASE+4u*i || !recomp_lookup_kernel(FF_KERNEL_BASE+4u*i))stop(9,slot,bindings[i].ordinal);
        ++bound;
    }
    if(bound!=2u)stop(9,bound,277u);
    memcpy(stdio_object_before,ff_sdk_shared_memory(0x2b7668u,32u,FF_VM_READ,s),32u);
    w[3]=g_esp;w[4]=f[0];w[5]=f[1];w[6]=f[2];w[7]=g_eax;w[8]=g_ecx;w[9]=g_edx;
    w[10]=g_ebx;w[11]=g_esi;w[12]=g_edi;w[13]=g_ebp;w[14]=(uint32_t)g_df;w[15]=g_fs_base;w[16]=g_seh_ebp;
    w[17]=sdk_error_read32(g_fs_base,s);w[18]=t->id;w[19]=table;w[20]=count;w[21]=3u;w[22]=0x2b7668u;
    w[23]=lock1;w[24]=lock19;w[25]=t->stack_low;w[26]=g_esp+12u-t->stack_low;w[27]=kernel_call_count;
    for(uint32_t i=0;i<6u;++i)w[29u+i]=ff_probe_abi_violation_word(i);
    w[56]=0x03c10e40u;if((sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u))))))w[74]=0x001e01a2u;w[57]=1u;w[59]=g_eflags;w[63]=1u;
    w[72]=sdk_error_read32(0x2b7b64u,s);w[73]=sdk_error_read32(0x2b7bf4u,s);
    ff_guest_cpu before,after;ff_capture_cpu(&before);
    if((sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u))))))))))sdk_disc_bind_reached();
    w[0]=1u;w[58]=1u;stdio_active=1u;
    ff_title_001E01AD(); /* Unchanged original providers; unsupported stops propagate. */
    (void)sdk_thread();ff_capture_cpu(&after);ff_sdk_shared_read(s);
    w[28]=kernel_call_count;w[36]=g_esp;w[37]=g_eax;w[38]=g_ecx;w[39]=g_edx;w[40]=g_ebx;w[41]=g_esi;w[42]=g_edi;
    w[43]=g_ebp;w[44]=g_seh_ebp;w[45]=(uint32_t)g_df;w[46]=g_fs_base;w[47]=sdk_error_read32(g_fs_base,s);
    for(uint32_t i=0;i<6u;++i)w[50u+i]=ff_probe_abi_violation_word(i);
    if(g_esp!=w[3]+4u || g_ebx!=w[10] || g_esi!=w[11] || g_edi!=w[12] || g_df ||
       g_fs_base!=w[15] || g_seh_ebp!=w[16] || g_ebp!=w[56] || w[47]!=w[17] ||
       sdk_error_read32(w[56],s)!=((sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u)))))?w[74]:w[13]) || sdk_error_read32(w[56]+4u,s)!=0x1e01bcu ||
       sdk_error_read32(w[3],s)!=w[4] || sdk_error_read32(w[3]+4u,s)!=w[5] || sdk_error_read32(w[3]+8u,s)!=w[6])stop(9,g_esp,0x1e01adu);
    if(before.r_mxcsr!=after.r_mxcsr || before.r_fp_top!=after.r_fp_top || before.r_fp_cmp!=after.r_fp_cmp ||
       before.r_fp_control_word!=after.r_fp_control_word || before.r_fp_cc!=after.r_fp_cc || before.r_force_return!=after.r_force_return ||
       memcmp(before.fp_values,after.fp_values,sizeof before.fp_values) || memcmp(before.xmm_bytes,after.xmm_bytes,sizeof before.xmm_bytes) ||
       memcmp(before.mm_bytes,after.mm_bytes,sizeof before.mm_bytes))stop(9,g_esp,0x1e01adu);
    w[48]=sdk_error_read32(w[56],s);w[60]=g_eflags;
    /* Generated C epilog publishes restored local EBP only to g_seh_ebp.
       After all source-derived restoration checks, convert adapter representation.
       No original instruction/body or other guest register is rewritten. */
    g_ebp=w[13];w[49]=g_ebp;ff_sdk_shared_read(s);
    memcpy(stdio_object_after,ff_sdk_shared_memory(w[22],32u,FF_VM_READ,s),32u);
    stdio_active=0u;w[1]=1u;ff_sdk_shared_read(s);
}
static void sdk_stdio_post(ff_sdk_scalar *s)
{
    uint32_t *w=stdio_words;(void)sdk_thread();
    if(!sdk_stdio_profile() || w[1]!=1u || w[2] || stdio_active || g_esp!=w[36] || g_eax!=w[37] ||
       g_ecx!=w[38] || g_edx!=w[39] || g_ebx!=w[10] || g_esi!=w[11] || g_edi!=w[12] || g_ebp!=w[13] ||
       (uint32_t)g_df!=w[14] || g_fs_base!=w[15] || g_seh_ebp!=w[16])stop(9,g_esp,0x4f612u);
    w[76]=g_esp;w[77]=g_eax;w[78]=g_ecx;w[79]=g_edx;w[80]=g_ebx;w[81]=g_esi;w[82]=g_edi;w[83]=g_ebp;
    w[84]=(uint32_t)g_df;w[85]=g_fs_base;w[86]=g_seh_ebp;w[2]=1u;(void)s;
}

void ff_sdk_shared_enter(uint32_t guest, ff_sdk_scalar *s)
{
    const uint32_t scope41_guest=((sdk_scope44() && allocator44.stage==S43_ALLOC && (guest==0x4bf10u || guest==0x4be40u)) || ((sdk_scope43() && (guest==s43_call()->kind || guest==0x4dc30u || (s43_call()->stage==S43_ALLOC && (guest==0x4bf10u || guest==0x4be40u)))) || ((sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u)))) && sdk_current_target==0x4f5e0u && scope41_words[S41_ACTIVE]==1u && ((guest==0x4d4f0u && scope41_words[S41_STAGE]==1u) || (guest==0x4dc30u && scope41_words[S41_STAGE]==3u)))));
    if (!scope41_guest && !((sdk_current_target==0x30943u && (guest==0x30943u || guest==0x3082du || guest==0x311f9u)) || (sdk_current_target==0x3055au && guest==0x3055au) || (sdk_current_target==0xa6fa0u && guest==0xa6fa0u) || (sdk_current_target==0x4f890u && guest==0x4f890u) || (sdk_current_target==0x48880u && guest==0x48880u) || (sdk_enabled>=7u && sdk_current_target==0x48880u && guest==0x49d40u) || (sdk_enabled>=8u && sdk_current_target==0x48880u && guest==0x24d379u) || (sdk_enabled>=10u && sdk_current_target==0x48880u && guest==0x49990u) || (sdk_enabled>=11u && sdk_current_target==0x48880u && guest==0x24d37eu) || (sdk_enabled>=14u && sdk_current_target==0x48880u && guest==0x1e893cu) || (sdk_enabled>=16u && sdk_current_target==0x48880u && (guest==0x4d4f0u || guest==0x4dc30u)) || ((sdk_enabled>=17u && sdk_current_target==0x48880u && guest==0x4bf10u) || (sdk_scope42() && a42[A42_STAGE]==2u && guest==0x4bf10u)) || (sdk_nested_wait() && (guest==0x30943u || guest==0x3082du || guest==0x311f9u)) || ((sdk_enabled>=19u && sdk_current_target==0x48880u && guest==0x4be40u) || (sdk_scope42() && a42[A42_STAGE]==2u && guest==0x4be40u)) || (sdk_release_profile() && guest==0x3080du) || (sdk_enabled>=23u && sdk_current_target==0x4f5e0u && (guest==0x4f5e0u || guest==0x4eb50u)) || (sdk_directory_profile() && guest==0x1e95dbu) || (sdk_copy_profile() && guest==0x1e956fu) || (sdk_gate_helper_profile() && guest==0x4e960u) || (sdk_compare_profile() && guest==0x1e056du) || (sdk_next_directory_profile() && guest==0x1e96e8u) || (sdk_error_profile() && (guest==0x31155u || guest==0x31127u)))) stop(2,guest,0);
    ff_sdk_shared_write(s);
    if(guest==0x31155u || guest==0x31127u)sdk_error_enter(guest,s);
    if(guest==0x1e95dbu)sdk_directory_enter(s);
    if(guest==0x1e956fu)sdk_copy_enter(s);
    if(guest==0x4e960u)sdk_gate_helper_enter(s);
    if(guest==0x1e056du)sdk_compare_enter(s);
    if(guest==0x1e96e8u)sdk_next_directory_enter(s);
    if(guest==0x4f5e0u || guest==0x4eb50u)sdk_startup_enter(guest,s);
    if(guest==0x3080du && sdk_release_profile())sdk_release_enter(s);
    if(guest==0x4be40u)sdk_node_enter(s);
    if(sdk_nested_wait() && (guest==0x30943u || guest==0x3082du || guest==0x311f9u))sdk_wait_enter(guest,s);
    if(guest==0x4bf10u)sdk_call_enter(s);
    if(sdk_scope43() && (guest==s43_call()->kind || guest==0x4dc30u))s43_enter(guest,s);
    else if(guest==0x4d4f0u || guest==0x4dc30u){if(scope41_guest)ff_probe_sdk_scope41_enter(guest,s);else sdk_prefix_enter(guest,s);}
    if (guest==0x48880u && sdk_enabled>=15u) {
        ff_boot_thread *t=sdk_thread();
        if (parent_setup_words[0] || (g_esp&3u) || g_esp<t->stack_low || g_esp>t->stack_top ||
            8u>t->stack_top-g_esp || ff_vm_check(&virtual_memory,g_esp,8u,FF_VM_READ)) stop(9,g_esp,0x48880u);
        const uint32_t *frame=(const uint32_t *)ff_sdk_shared_memory(g_esp,8u,FF_VM_READ,s);
        if (frame[0]!=0x4f7ffu || frame[1]!=1u) stop(9,g_esp,0x48880u);
        parent_setup_words[0]=1u;parent_setup_words[1]=frame[0];parent_setup_words[2]=g_esp;
        parent_setup_words[3]=g_eax;parent_setup_words[4]=g_ebx;parent_setup_words[5]=g_ecx;
        parent_setup_words[6]=g_edx;parent_setup_words[7]=g_ebp;parent_setup_words[8]=g_esi;
        parent_setup_words[9]=g_edi;parent_setup_words[10]=(uint32_t)g_df;
        parent_setup_words[11]=0u; /* Caller-local EFLAGS is not present in ff_sdk_scalar. */
        parent_setup_words[12]=g_seh_ebp;
        parent_setup_words[31]=frame[1];
    }
    if (guest==0x49d40u) {
        ff_boot_thread *t=sdk_thread();
        if ((g_esp&3u) || g_esp<t->stack_low || g_esp>t->stack_top ||
            12u>t->stack_top-g_esp || ff_vm_check(&virtual_memory,g_esp,12u,FF_VM_READ))
            stop(9,g_esp,0);
        const uint32_t *frame=(const uint32_t *)(sdk_enabled>=15u?
            ff_sdk_shared_memory(g_esp,12u,FF_VM_READ,s):guest_ram+g_esp);
        const uint32_t saved_edi_word=sdk_enabled>=15u?parent_setup_words[9]:1u;
        if (frame[0]!=0x48895u || frame[1]!=1u || frame[2]!=saved_edi_word) stop(9,g_esp,0);
        if (sdk_enabled>=15u) {
            if (sdk_current_target!=0x48880u || parent_setup_words[0]!=1u || parent_setup_words[13])
                stop(9,g_esp,0x49d40u);
            parent_setup_words[13]=1u;parent_setup_words[14]=g_esp;parent_setup_words[15]=frame[0];
        }
    }
    if (guest==0x24d379u) {
        ff_boot_thread *t=sdk_thread();
        if ((g_esp&3u) || g_esp<t->stack_low || g_esp>t->stack_top ||
            32u>t->stack_top-g_esp || ff_vm_check(&virtual_memory,g_esp,32u,FF_VM_READ) ||
            MEM32(g_esp)!=0x49d7bu || MEM32(g_esp+4u)!=2u || MEM32(g_esp+8u)!=g_esp+16u ||
            MEM32(g_esp+16u)!=0x24c2bcu || MEM32(g_esp+20u)!=4u ||
            MEM32(g_esp+24u)!=0x24c238u || MEM32(g_esp+28u)!=4u)
            stop(9,g_esp,0);
    }
    if (guest==0x49990u) {
        ff_boot_thread *t=sdk_thread();
        if ((g_esp&3u) || g_esp<t->stack_low || g_esp>t->stack_top ||
            4u>t->stack_top-g_esp || ff_vm_check(&virtual_memory,g_esp,4u,FF_VM_READ) ||
            host_words[3]!=1u) stop(9,g_esp,0);
        const uint32_t entry_ret=sdk_enabled>=13u?
            *(uint32_t *)ff_sdk_shared_memory(g_esp,4u,FF_VM_READ,s):MEM32(g_esp);
        if(entry_ret!=0x49d80u) stop(9,g_esp,0);
        if (sdk_enabled>=13u) {
            if (title_return_words[0] || getter_completed!=0u || log_bridge_words[1]!=0u ||
                getter_active || irql_pair.active || irql_pair.raise_count || irql_pair.lower_count)
                stop(9,g_esp,0x49990u);
            title_return_words[0]=1u;
            title_return_words[3]=g_esp;title_return_words[4]=g_ebx;
            title_return_words[5]=g_esi;title_return_words[6]=g_edi;
            title_return_words[7]=g_ebp;title_return_words[8]=(uint32_t)g_df;
        }
    }
    if (guest==0x24d37eu) {
        ff_boot_thread *t=sdk_thread();
        if ((g_esp&3u) || g_esp<t->stack_low || g_esp>t->stack_top ||
            8u>t->stack_top-g_esp || ff_vm_check(&virtual_memory,g_esp,8u,FF_VM_READ) ||
            !irql_imports || host_words[3]!=1u || getter_active) stop(9,g_esp,0);
        const uint32_t *frame=(const uint32_t *)((sdk_enabled>=13u)?
            ff_sdk_shared_memory(g_esp,8u,FF_VM_READ,s):guest_ram+g_esp);
        const uint32_t caller=frame[0],type=frame[1];
        if (caller==0x499c7u && type==0x24c2bcu && getter_completed==0u && !irql_pair.raise_count) {
            getter_active=1u;getter_active_id=1u;
            irql_words[0]=g_esp;
        } else if (sdk_enabled>=13u && caller==0x49a5bu && type==0x24c238u &&
                   getter_completed==1u && log_bridge_words[1]==1u &&
                   irql_pair.raise_count==1u && irql_pair.lower_count==1u) {
            getter_active=1u;getter_active_id=2u;
            second_query_words[0]=g_esp;second_query_words[1]=caller;second_query_words[2]=type;
        } else stop(9,g_esp,0);
        if (sdk_enabled>=13u) sdk_require_zero_xpp(type,s);
    }
    if (guest==0x1e893cu && sdk_enabled>=14u && sdk_current_target==0x48880u) {
        ff_boot_thread *t=sdk_thread();
        if (!tick_enabled || tick_words[TICK_ATTEMPTS] || tick_words[TICK_PRE_RET_VALIDATED] ||
            tick_stage || title_return_words[2]!=1u || !timers_ready ||
            (g_esp&3u) || g_esp<t->stack_low || g_esp>t->stack_top ||
            4u>t->stack_top-g_esp || ff_vm_check(&virtual_memory,g_esp,4u,FF_VM_READ) ||
            sdk_tick_cell_mapping(1u) || ff_vm_check(&virtual_memory,SDK_TICK_SLOT,4u,FF_VM_READ) ||
            ff_vm_check(&virtual_memory,SDK_TICK_CELL,4u,FF_VM_READ|FF_VM_WRITE)) stop(9,g_esp,0x1e893cu);
        ++tick_words[TICK_ATTEMPTS];tick_stage=1u;tick_entry_esp=g_esp;
        tick_words[TICK_ENTRY_ESP]=g_esp;tick_words[TICK_ENTRY_EAX]=g_eax;
        tick_words[TICK_ENTRY_EBX]=g_ebx;tick_words[TICK_ENTRY_ESI]=g_esi;
        tick_words[TICK_ENTRY_EDI]=g_edi;tick_words[TICK_ENTRY_EBP]=g_ebp;
        tick_words[TICK_ENTRY_DF]=(uint32_t)g_df;tick_entry_eflags=g_eflags;
        tick_words[TICK_ENTRY_EFLAGS]=g_eflags;tick_entry_seh=g_seh_ebp;
        tick_words[TICK_ENTRY_SEH]=g_seh_ebp;
        const uint32_t *frame=(const uint32_t *)ff_sdk_shared_memory(g_esp,4u,FF_VM_READ,s);
        if (frame[0]!=0x49d85u || tick_entry_frame_reads!=1u) stop(9,g_esp,0x1e893cu);
        tick_words[TICK_ENTRY_RET]=frame[0];
        const uint32_t *slot=(const uint32_t *)ff_sdk_shared_memory(SDK_TICK_SLOT,4u,FF_VM_READ,s);
        const uint32_t *cell=(const uint32_t *)ff_sdk_shared_memory(SDK_TICK_CELL,4u,FF_VM_READ,s);
        tick_extra_words[TICK_ENTRY_SLOT_VALUE]=*slot;tick_extra_words[TICK_ENTRY_CELL_VALUE]=*cell;
        if (tick_entry_recheck_reads!=2u || *slot!=SDK_TICK_CELL || *cell!=0u) stop(9,SDK_TICK_CELL,*cell);
    }
    ++sdk_words[10];
    recomp_trace_enter("sdk-shared", guest);
    ff_sdk_shared_read(s);
    if (guest==0x1e893cu && sdk_enabled>=14u && sdk_current_target==0x48880u) {
        if (!timers_ready || !timers.clock_ready || tick_stage!=1u ||
            g_esp!=tick_entry_esp || g_ebx!=tick_words[TICK_ENTRY_EBX] ||
            g_esi!=tick_words[TICK_ENTRY_ESI] || g_edi!=tick_words[TICK_ENTRY_EDI] ||
            g_ebp!=tick_words[TICK_ENTRY_EBP] || g_df!=0 ||
            g_eflags!=tick_entry_eflags || g_seh_ebp!=tick_entry_seh)
            stop(9,g_esp,0x1e893cu);
        const uint64_t now=timers.interrupt_time;
        tick_words[TICK_CLOCK_LO]=(uint32_t)now;tick_words[TICK_CLOCK_HI]=(uint32_t)(now>>32);
        tick_words[TICK_MILLISECONDS]=(uint32_t)(now/10000u);
        tick_stage=2u;
    }
}
void ff_sdk_shared_exit(uint32_t guest, ff_sdk_scalar *s)
{
    if(guest==0x4f6c2u && sdk_enabled==45u) {
        ff_sdk_shared_write(s);s45_post(s);ff_sdk_shared_read(s);return;
    }
    if(sdk_scope43() && (guest==s43_call()->kind || guest==0x4dc30u)) {
        ff_sdk_shared_write(s);s43_pre_ret(guest,0u,s);ff_sdk_shared_read(s);return;
    }
    if(sdk_scope43() && guest==0x4d539u) {
        ff_scope43_call *c=s43_call();
        if(c->kind!=0x4d4f0u||!c->pointer_ready||release_words[63]!=5u||s->esp!=c->allocator_entry.esp+4u||s->eax!=c->q)stop(9,s->esp,guest);
        return;
    }

    if(ff_probe_sdk_scope41_active() && (guest==0x4d4f0u || guest==0x4dc30u)){ff_sdk_shared_write(s);ff_probe_sdk_scope41_pre_ret(guest,s);ff_sdk_shared_read(s);return;}
    if(guest==0x4f612u){ff_sdk_shared_write(s);sdk_stdio_post(s);ff_sdk_shared_read(s);return;}
    if(guest==0x4ece2u || guest==0x4f603u || (guest==0x4eb50u && sdk_close_profile())){
        ff_sdk_shared_write(s);sdk_close_observe(guest,s);
        if(guest==0x4eb50u)recomp_trace_exit("sdk-shared",guest);
        ff_sdk_shared_read(s);return;
    }
    if(guest==0x31155u || guest==0x31127u || guest==0x3115fu || guest==0x31167u || guest==0x1e971fu || guest==0x1e96e8u || guest==0x4ecd4u){
        if(!sdk_error_profile())stop(2,guest,0);
        ff_sdk_shared_write(s);sdk_error_exit(guest,s);
        if(guest==0x31155u || guest==0x31127u || guest==0x1e96e8u)recomp_trace_exit("sdk-shared",guest);
        ff_sdk_shared_read(s);return;
    }
    if(guest==0x1e056du || guest==0x4ebd1u || guest==0x4ebebu) {
        if(!sdk_compare_profile())stop(2,guest,0);
        ff_sdk_shared_write(s);
        if(guest==0x1e056du){sdk_compare_exit(s);recomp_trace_exit("sdk-shared",guest);}
        else sdk_compare_post(guest,s);
        ff_sdk_shared_read(s);return;
    }
    if(guest==0x4e960u || guest==0x4eb8fu) {
        if(!sdk_gate_helper_profile())stop(2,guest,0);
        ff_sdk_shared_write(s);
        if(guest==0x4e960u){sdk_gate_helper_exit(s);recomp_trace_exit("sdk-shared",guest);}
        else sdk_gate_helper_post(s);
        ff_sdk_shared_read(s);return;
    }
    if(guest==0x4eb7bu) {
        ff_sdk_shared_write(s);(void)sdk_thread();uint32_t *w=record_log_words;
        if((sdk_enabled!=28u && (sdk_enabled!=29u && (sdk_enabled!=30u && (sdk_enabled!=31u && (sdk_enabled!=32u && (sdk_enabled!=33u && (sdk_enabled!=34u && (sdk_enabled!=35u && (sdk_enabled!=36u && (sdk_enabled!=37u && (sdk_enabled!=38u && (sdk_enabled!=39u && (sdk_enabled!=40u && (sdk_enabled!=41u && (sdk_enabled!=42u && (sdk_enabled!=43u && (sdk_enabled!=44u && sdk_enabled!=45u))))))))))))))))) || sdk_current_target!=0x4f5e0u || w[0]!=1u || w[1]!=1u || w[36] ||
           g_esp!=w[2]+4u || g_eax!=w[11] || g_ebp!=w[4] || g_seh_ebp!=w[32] ||
           g_ebx!=w[8] || g_esi!=w[9] || g_edi!=w[10] || g_df || g_fs_base!=copy_words[75])stop(9,g_esp,guest);
        w[36]=1u;w[37]=g_esp;w[38]=g_eax;w[39]=g_ebp;w[40]=g_seh_ebp;w[41]=(uint32_t)g_df;
        ff_sdk_shared_read(s);return;
    }
    if(guest==0x1e956fu || guest==0x1e95dbu || guest==0x1e96d3u || guest==0x4eb66u) {
        if(!sdk_copy_profile())stop(2,guest,0);
        ff_sdk_shared_write(s);
        if(guest==0x1e956fu)sdk_copy_exit(s);
        else if(guest==0x1e95dbu)sdk_directory_exit(s);
        else if(guest==0x1e96d3u)sdk_copy_post(s);
        else sdk_directory_post(s);
        if(guest==0x1e956fu || guest==0x1e95dbu)recomp_trace_exit("sdk-shared",guest);
        ff_sdk_shared_read(s);return;
    }

    if(guest==0x4d555u || guest==0x488adu || guest==0x488b9u){
        ff_sdk_shared_write(s);sdk_continuation_observe(guest,s);ff_sdk_shared_read(s);return;
    }

    if(guest==0x4c0dau || guest==0x4d539u){
        ff_sdk_shared_write(s);
        if(guest==0x4c0dau)sdk_release_post(s);else sdk_pointer_post(s);
        ff_sdk_shared_read(s);return;
    }

    if(guest==0x4c048u){
        ff_sdk_shared_write(s);sdk_node_post(s);ff_sdk_shared_read(s);return;
    }

    if(guest==0x4bf85u){
        ff_sdk_shared_write(s);sdk_wait_post(s);ff_sdk_shared_read(s);return;
    }

    if (!((sdk_current_target==0x30943u && (guest==0x30943u || guest==0x3082du || guest==0x311f9u)) || (sdk_current_target==0x3055au && guest==0x3055au) || (sdk_current_target==0xa6fa0u && guest==0xa6fa0u) || (sdk_current_target==0x4f890u && guest==0x4f890u) || (sdk_enabled>=11u && sdk_current_target==0x48880u && guest==0x24d37eu) || (sdk_enabled>=13u && sdk_current_target==0x48880u && guest==0x49990u) || (sdk_enabled>=14u && sdk_current_target==0x48880u && guest==0x1e893cu) || (sdk_enabled>=15u && sdk_current_target==0x48880u && (guest==0x49d85u || guest==0x48895u)) || (sdk_enabled>=16u && sdk_current_target==0x48880u && (guest==0x4d4f0u || guest==0x4dc30u)) || ((sdk_enabled>=17u && sdk_current_target==0x48880u && guest==0x4bf10u) || ((sdk_scope42() && a42[A42_STAGE]==2u && guest==0x4bf10u) || (sdk_allocator_scope() && s43_call()->stage==S43_ALLOC && guest==0x4bf10u))) || (sdk_nested_wait() && (guest==0x30943u || guest==0x3082du || guest==0x311f9u)) || ((sdk_enabled>=19u && sdk_current_target==0x48880u && guest==0x4be40u) || ((sdk_scope42() && a42[A42_STAGE]==2u && guest==0x4be40u) || (sdk_allocator_scope() && s43_call()->stage==S43_ALLOC && guest==0x4be40u))) || (sdk_release_profile() && guest==0x3080du))) stop(2,guest,0);
    ff_sdk_shared_write(s);
    if(guest==0x3080du && sdk_release_profile())sdk_release_exit(s);
    if(guest==0x4be40u)sdk_node_exit(s);
    if(sdk_nested_wait() && (guest==0x30943u || guest==0x3082du || guest==0x311f9u))sdk_wait_exit(guest,s);
    if(guest==0x4bf10u)sdk_call_exit(s);
    if(guest==0x4d4f0u || guest==0x4dc30u)sdk_prefix_exit(guest,s);
    if (guest==0x24d37eu) {
        if (sdk_enabled<13u) {
            if (!getter_active || getter_active_id!=1u || g_esp!=irql_words[0] || g_eax ||
                irql_pair.active || irql_pair.raise_count!=1u || irql_pair.lower_count!=1u)
                stop(9,g_esp,0);
            irql_words[1]=g_esp;
            irql_words[6]=MEM32(0x24c2bcu+4u);irql_words[7]=MEM32(0x24c2bcu+8u);
        } else {
            if (!getter_active || !getter_active_id || g_eax || irql_pair.active ||
                irql_pair.raise_count!=getter_active_id || irql_pair.lower_count!=getter_active_id)
                stop(9,g_esp,0);
            ff_boot_thread *t=sdk_thread();
            if (g_esp!=(getter_active_id==1u?irql_words[0]:second_query_words[0]) ||
                (g_esp&3u) || g_esp<t->stack_low || g_esp>t->stack_top ||
                8u>t->stack_top-g_esp || ff_vm_check(&virtual_memory,g_esp,8u,FF_VM_READ)) stop(9,g_esp,0);
            const uint32_t *retframe=(const uint32_t *)ff_sdk_shared_memory(g_esp,8u,FF_VM_READ,s);
            if(retframe[0]!=(getter_active_id==1u?0x499c7u:0x49a5bu) ||
               retframe[1]!=(getter_active_id==1u?0x24c2bcu:0x24c238u)) stop(9,g_esp,0);
            if (getter_active_id==1u) {
                irql_words[1]=g_esp;
                irql_words[6]=MEM32(0x24c2bcu+4u);irql_words[7]=MEM32(0x24c2bcu+8u);
            } else {
                second_query_words[7]=g_esp;
                sdk_require_zero_xpp(0x24c238u,s);
            }
        }
        ++getter_completed;getter_active=0;getter_active_id=0;
    }
    if (guest==0x49d85u && sdk_enabled>=15u) {
        if (sdk_current_target!=0x48880u || tick_stage!=3u || tick_words[TICK_PRE_RET_VALIDATED]!=1u ||
            parent_setup_words[14]==0u || parent_setup_words[32] || g_esp!=tick_entry_esp+4u)
            stop(9,g_esp,0x49d85u);
        ++parent_setup_words[32];parent_setup_words[33]=g_esp;tick_stage=4u;
        return;
    }
    if (guest==0x48895u && sdk_enabled>=15u) {
        ff_boot_thread *t=sdk_thread();
        ++parent_setup_words[16];
        if (parent_setup_words[0]!=1u || parent_setup_words[13]!=1u || parent_setup_words[15]!=0x48895u ||
            parent_setup_words[16]!=1u || parent_setup_words[17] || parent_setup_words[31]!=1u || parent_setup_words[32]!=1u ||
            g_esp!=parent_setup_words[2]-8u || (g_esp&3u) || g_esp<t->stack_low || g_esp>t->stack_top ||
            16u>t->stack_top-g_esp || ff_vm_check(&virtual_memory,g_esp,16u,FF_VM_READ) ||
            g_ebx!=parent_setup_words[4] || g_esi!=parent_setup_words[8] || g_edi!=parent_setup_words[9] ||
            g_ebp!=parent_setup_words[7] || g_df!=0 || g_ebp!=parent_setup_words[12] || g_seh_ebp!=parent_setup_words[12])
            stop(9,g_esp,0x48895u);
        const uint32_t *frame=(const uint32_t *)ff_sdk_shared_memory(g_esp,16u,FF_VM_READ,s);
        if (frame[0]!=parent_setup_words[31] || frame[1]!=parent_setup_words[9] ||
            frame[2]!=parent_setup_words[1] || frame[3]!=parent_setup_words[31]) stop(9,g_esp,0x48895u);
        parent_setup_words[17]=1u;parent_setup_words[18]=0x48895u;parent_setup_words[19]=g_esp;
        parent_setup_words[20]=g_eax;parent_setup_words[21]=g_ebx;parent_setup_words[22]=g_esi;
        parent_setup_words[23]=g_edi;parent_setup_words[24]=g_ebp;parent_setup_words[25]=(uint32_t)g_df;
        parent_setup_words[26]=0u; /* No general caller EFLAGS observation is claimed here. */
        parent_setup_words[27]=g_seh_ebp;
        parent_setup_words[28]=frame[0];parent_setup_words[29]=frame[1];
        parent_setup_words[30]=frame[2];parent_setup_words[31]=frame[3];
        return;
    }
    if (guest==0x49990u && sdk_enabled>=13u) {
        ff_boot_thread *t=sdk_thread();
        ++title_return_words[1];
        if (title_return_words[0]!=1u || title_return_words[1]!=1u || title_return_words[2] ||
            getter_completed!=2u || second_query_words[7]!=second_query_words[0] || second_log_bridge_words[1]!=1u ||
            (g_esp&3u) || g_esp<t->stack_low || g_esp>t->stack_top ||
            4u>t->stack_top-g_esp || ff_vm_check(&virtual_memory,g_esp,4u,FF_VM_READ) ||
            g_esp!=title_return_words[3] || g_ebx!=title_return_words[4] ||
            g_esi!=title_return_words[5] || g_edi!=title_return_words[6] ||
            g_ebp!=title_return_words[7] || g_df!=0)
            stop(9,g_esp,0x49990u);
        const uint32_t ret=*(uint32_t *)ff_sdk_shared_memory(g_esp,4u,FF_VM_READ,s);
        if(ret!=0x49d80u) stop(9,g_esp,0x49990u);
        title_return_words[9]=g_esp;title_return_words[10]=ret;
        title_return_words[11]=g_ebx;title_return_words[12]=g_esi;
        title_return_words[13]=g_edi;title_return_words[14]=g_ebp;
        title_return_words[15]=(uint32_t)g_df;title_return_words[16]=g_eax;title_return_words[17]=g_eflags;
        recomp_trace_exit("sdk-shared",guest);
        ff_sdk_shared_read(s);
        title_return_words[2]++;
        return;
    }
    if (guest==0x1e893cu && sdk_enabled>=14u && sdk_current_target==0x48880u) {
        ff_boot_thread *t=sdk_thread();
        ++tick_words[TICK_PRE_RET_ATTEMPTS];
        if (tick_words[TICK_ATTEMPTS]!=1u || tick_words[TICK_PRE_RET_ATTEMPTS]!=1u ||
            tick_words[TICK_PRE_RET_VALIDATED] || tick_stage!=2u ||
            tick_slot_reads!=1u || tick_cell_reads!=1u ||
            tick_words[TICK_SLOT_READS]!=1u || tick_words[TICK_CELL_READS]!=1u ||
            tick_words[TICK_POINTER]!=SDK_TICK_CELL || tick_words[TICK_REFRESHES]!=1u || tick_refreshing ||
            tick_extra_words[TICK_PREWRITE_RECHECKS]!=2u || sdk_tick_cell_mapping(1u) ||
            tick_words[TICK_CELL_UPDATED]!=tick_words[TICK_MILLISECONDS] ||
            g_eax!=tick_words[TICK_MILLISECONDS] || g_esp!=tick_entry_esp ||
            (g_esp&3u) || g_esp<t->stack_low || g_esp>t->stack_top ||
            4u>t->stack_top-g_esp || ff_vm_check(&virtual_memory,g_esp,4u,FF_VM_READ) ||
            g_ebx!=tick_words[TICK_ENTRY_EBX] || g_esi!=tick_words[TICK_ENTRY_ESI] ||
            g_edi!=tick_words[TICK_ENTRY_EDI] || g_ebp!=tick_words[TICK_ENTRY_EBP] ||
            g_df!=0 || g_eflags!=tick_entry_eflags || g_seh_ebp!=tick_entry_seh)
            stop(9,g_esp,0x1e893cu);
        tick_stage=3u;
        const uint32_t *retframe=(const uint32_t *)ff_sdk_shared_memory(g_esp,4u,FF_VM_READ,s);
        if (retframe[0]!=0x49d85u || tick_exit_frame_reads!=1u) stop(9,g_esp,0x1e893cu);
        tick_words[TICK_EXIT_EAX]=g_eax;tick_words[TICK_EXIT_ESP]=g_esp;
        tick_words[TICK_EXIT_RET]=retframe[0];tick_words[TICK_EXIT_EBX]=g_ebx;
        tick_words[TICK_EXIT_ESI]=g_esi;tick_words[TICK_EXIT_EDI]=g_edi;
        tick_words[TICK_EXIT_EBP]=g_ebp;tick_words[TICK_EXIT_DF]=(uint32_t)g_df;
        tick_words[TICK_EXIT_EFLAGS]=g_eflags;tick_words[TICK_EXIT_SEH]=g_seh_ebp;
        recomp_trace_exit("sdk-shared",guest);
        ff_sdk_shared_read(s);
        ++tick_words[TICK_PRE_RET_VALIDATED];
        tick_stage=(sdk_enabled>=15u)?3u:0u;
        return;
    }
    recomp_trace_exit("sdk-shared", guest);
    if(guest==0x4d4f0u && sdk_enabled>=22u && sdk_invocation==1u)sdk_tail_exit(s);
    ff_sdk_shared_read(s);
}
void ff_sdk_shared_kernel(uint32_t ordinal, ff_sdk_scalar *s)
{
    ff_sdk_shared_write(s);
    ff_boot_thread *t=sdk_thread();
    if(ordinal==301u){sdk_error_kernel(s);return;}
    if(ordinal==207u && sdk_next_directory_profile() && next_directory_words[0]){sdk_next_directory_query_stop(s);return;}
    if(ordinal==207u && (sdk_enabled==26u || sdk_enabled==27u || (sdk_enabled==28u || (sdk_enabled==29u || (sdk_enabled==30u || (sdk_enabled==31u || (sdk_enabled==32u || (sdk_enabled==33u || (sdk_enabled==34u || (sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u))))))))))))))))))){sdk_directory_query_first(s);return;}
    if (ordinal==129u || ordinal==161u) {sdk_irql_kernel(ordinal,s);return;}
    if(ordinal==202u && sdk_enabled==24u && sdk_directory_profile())sdk_directory_open_stop(s);
    if(ordinal==202u && sdk_directory_root_profile())sdk_directory_open_stop(s);
    uint32_t frame=0,slot=0,site=0;
    if (ordinal==234u && (sdk_current_target==0x30943u || sdk_nested_wait()) && local_mutant_wait_enabled) {
        frame=20u;slot=0x2533a0u;site=0x3084au;
    } else if (ordinal==221u && sdk_release_profile() && local_mutant_release_enabled) {
        frame=12u;slot=0x25339cu;site=0x30813u;
    } else if (ordinal==289u && sdk_directory_profile()) {
        frame=12u;slot=0x25338cu;site=0x1e95edu;
    } else if (ordinal==202u && sdk_directory_root_profile()) {
        frame=28u;slot=0x2533d0u;site=0x1e967du;
    } else if (ordinal==171u && sdk_current_target==0x3055au) {
        frame=8u;slot=0x25336cu;site=0x305aau;
    } else stop(1,ordinal,ordinal);
    if ((g_esp&3u) || g_esp<t->stack_low || g_esp>t->stack_top ||
        frame>t->stack_top-g_esp || ff_vm_check(&virtual_memory,g_esp,frame,FF_VM_READ))
        stop(9,g_esp,ordinal);
    if (ordinal==234u && (MEM32(g_esp)!=0x30850u || MEM32(g_esp+8u)!=1u ||
        MEM32(g_esp+12u) || MEM32(g_esp+16u))) stop(9,g_esp,ordinal);
    if (ordinal==171u && MEM32(g_esp)!=0x305b0u) stop(9,g_esp,ordinal);
    if (ordinal==221u && (MEM32(g_esp)!=0x30819u || MEM32(g_esp+8u)))stop(9,g_esp,ordinal);
    uint32_t token=0,found=0;
    for (uint32_t i=0;i<FF_KERNEL_BINDING_COUNT;i++) if (bindings[i].ordinal==ordinal) {
        if (++found!=1u || bindings[i].slot!=slot || ff_vm_check(&virtual_memory,slot,4u,FF_VM_READ))
            stop(9,slot,ordinal);
        token=FF_KERNEL_BASE+i*4u;
        if (*(uint32_t *)ff_probe_guest_memory(slot,4u,FF_VM_READ,site)!=token) stop(9,slot,ordinal);
    }
    recomp_func_t handler=found==1u?recomp_lookup_kernel(token):0;
    if (!handler) stop(1,token,ordinal);
    /* Reuse the existing selected service model or its existing unsupported stop. */
    if(ordinal==234u && sdk_nested_wait())sdk_wait_kernel_before(s);
    if(ordinal==221u && sdk_release_profile())sdk_release_kernel_before(s);
    if(ordinal==289u && sdk_directory_profile())sdk_directory_ansi_before(s);
    handler();(void)sdk_thread();ff_sdk_shared_read(s);
    if(ordinal==234u && sdk_nested_wait())sdk_wait_kernel_after(s);
    if(ordinal==221u && sdk_release_profile())sdk_release_kernel_after();
    if(ordinal==289u && sdk_directory_profile())sdk_directory_ansi_after(s);
    if(ordinal==202u && sdk_directory_root_profile())sdk_directory_open_after(s);
}
uint8_t *ff_sdk_shared_memory(uint32_t address, uint32_t bytes, uint32_t access, ff_sdk_scalar *s)
{
    ff_sdk_shared_write(s);
    if (!bytes || ff_vm_check(&virtual_memory,address,bytes,access)) stop(9,address,access);
    if(sdk_scope43() || (sdk_scope44() && allocator44.stage==S43_ALLOC))s43_memory(address,bytes,access,s);
    if(sdk_scope42() && (access&FF_VM_WRITE) && a42[A42_POINTER_READY]) sdk_scope42_write_check(address,bytes,access,s);
    if (third_log_memory_stage) {
        if ((sdk_enabled!=15u && sdk_enabled!=16u && (sdk_enabled!=17u && (sdk_enabled!=18u && (sdk_enabled!=19u && (sdk_enabled!=20u && (sdk_enabled!=21u && (sdk_enabled!=22u && (sdk_enabled!=23u && (sdk_enabled!=24u && (sdk_enabled!=25u && (sdk_enabled!=26u && (sdk_enabled!=27u && (sdk_enabled!=28u && (sdk_enabled!=29u && (sdk_enabled!=30u && (sdk_enabled!=31u && (sdk_enabled!=32u && (sdk_enabled!=33u && (sdk_enabled!=34u && (sdk_enabled!=35u && (sdk_enabled!=36u && (sdk_enabled!=37u && (sdk_enabled!=38u && (sdk_enabled!=39u && (sdk_enabled!=40u && (sdk_enabled!=41u && (sdk_enabled!=42u && (sdk_enabled!=43u && (sdk_enabled!=44u && sdk_enabled!=45u))))))))))))))))))))))))))))) || sdk_current_target!=0x48880u || third_log_memory_stage!=1u ||
            bytes!=4u || access!=FF_VM_READ) stop(9,address,access);
        if (third_log_memory_reads==0u && address==0x002cc280u) {
            ++third_log_memory_reads;
            return ff_probe_guest_memory(address,bytes,access,sdk_current_target);
        }
        if (third_log_memory_reads==1u && address==0x002cccf8u) {
            ++third_log_memory_reads;
            return ff_probe_guest_memory(address,bytes,access,sdk_current_target);
        }
        stop(9,address,access);
    }
    if (tick_refreshing==2u) {
        if ((sdk_enabled!=14u && (sdk_enabled!=15u && sdk_enabled!=16u && (sdk_enabled!=17u && (sdk_enabled!=18u && (sdk_enabled!=19u && (sdk_enabled!=20u && (sdk_enabled!=21u && (sdk_enabled!=22u && (sdk_enabled!=23u && (sdk_enabled!=24u && (sdk_enabled!=25u && (sdk_enabled!=26u && (sdk_enabled!=27u && (sdk_enabled!=28u && (sdk_enabled!=29u && (sdk_enabled!=30u && (sdk_enabled!=31u && (sdk_enabled!=32u && (sdk_enabled!=33u && (sdk_enabled!=34u && (sdk_enabled!=35u && (sdk_enabled!=36u && (sdk_enabled!=37u && (sdk_enabled!=38u && (sdk_enabled!=39u && (sdk_enabled!=40u && (sdk_enabled!=41u && (sdk_enabled!=42u && (sdk_enabled!=43u && (sdk_enabled!=44u && sdk_enabled!=45u)))))))))))))))))))))))))))))) || sdk_current_target!=0x48880u || tick_stage!=2u || bytes!=4u || access!=FF_VM_READ)
            stop(9,address,access);
        if (tick_prewrite_reads==0u && address==SDK_TICK_SLOT) {
            tick_extra_words[TICK_PREWRITE_SLOT_VALUE]=*(uint32_t *)ff_probe_guest_memory(address,bytes,access,sdk_current_target);
            ++tick_prewrite_reads;tick_extra_words[TICK_PREWRITE_RECHECKS]=tick_prewrite_reads;
            return (uint8_t *)&guest_ram[address];
        }
        if (tick_prewrite_reads==1u && address==SDK_TICK_CELL) {
            tick_extra_words[TICK_PREWRITE_CELL_VALUE]=*(uint32_t *)ff_probe_guest_memory(address,bytes,access,sdk_current_target);
            ++tick_prewrite_reads;tick_extra_words[TICK_PREWRITE_RECHECKS]=tick_prewrite_reads;
            return (uint8_t *)&guest_ram[address];
        }
        stop(9,address,access);
    }
    if (tick_refreshing==1u) {
        if ((sdk_enabled!=14u && (sdk_enabled!=15u && sdk_enabled!=16u && (sdk_enabled!=17u && (sdk_enabled!=18u && (sdk_enabled!=19u && (sdk_enabled!=20u && (sdk_enabled!=21u && (sdk_enabled!=22u && (sdk_enabled!=23u && (sdk_enabled!=24u && (sdk_enabled!=25u && (sdk_enabled!=26u && (sdk_enabled!=27u && (sdk_enabled!=28u && (sdk_enabled!=29u && (sdk_enabled!=30u && (sdk_enabled!=31u && (sdk_enabled!=32u && (sdk_enabled!=33u && (sdk_enabled!=34u && (sdk_enabled!=35u && (sdk_enabled!=36u && (sdk_enabled!=37u && (sdk_enabled!=38u && (sdk_enabled!=39u && (sdk_enabled!=40u && (sdk_enabled!=41u && (sdk_enabled!=42u && (sdk_enabled!=43u && (sdk_enabled!=44u && sdk_enabled!=45u)))))))))))))))))))))))))))))) || sdk_current_target!=0x48880u || tick_stage!=2u ||
            address!=SDK_TICK_CELL || bytes!=4u || access!=FF_VM_WRITE) stop(9,address,access);
        tick_extra_words[TICK_FINAL_REFRESH_ACTIVE]=tick_refreshing;
        return ff_probe_guest_memory(address,bytes,access,sdk_current_target);
    }
    if (sdk_enabled>=14u && sdk_current_target==0x48880u && tick_stage>=1u && tick_stage<=3u) {
        if (tick_stage==1u && address==tick_entry_esp && bytes==4u && access==FF_VM_READ &&
            tick_entry_frame_reads==0u && tick_entry_recheck_reads==0u) {
            ++tick_entry_frame_reads;
            return ff_probe_guest_memory(address,bytes,access,sdk_current_target);
        }
        if (tick_stage==1u && bytes==4u && access==FF_VM_READ && tick_entry_recheck_reads==0u &&
            address==SDK_TICK_SLOT) {
            tick_extra_words[TICK_ENTRY_SLOT_VALUE]=*(uint32_t *)ff_probe_guest_memory(address,bytes,access,sdk_current_target);
            tick_entry_recheck_reads=1u;tick_extra_words[TICK_ENTRY_RECHECKS]=tick_entry_recheck_reads;
            return (uint8_t *)&guest_ram[address];
        }
        if (tick_stage==1u && bytes==4u && access==FF_VM_READ && tick_entry_recheck_reads==1u &&
            address==SDK_TICK_CELL) {
            tick_extra_words[TICK_ENTRY_CELL_VALUE]=*(uint32_t *)ff_probe_guest_memory(address,bytes,access,sdk_current_target);
            tick_entry_recheck_reads=2u;tick_extra_words[TICK_ENTRY_RECHECKS]=tick_entry_recheck_reads;
            return (uint8_t *)&guest_ram[address];
        }
        if (tick_stage==2u && address==SDK_TICK_SLOT && bytes==4u && access==FF_VM_READ &&
            !tick_slot_reads && !tick_cell_reads) {
            ++tick_slot_reads;tick_words[TICK_SLOT_READS]=tick_slot_reads;
            return ff_probe_guest_memory(address,bytes,access,sdk_current_target);
        }
        if (tick_stage==2u && address==SDK_TICK_CELL && bytes==4u && access==FF_VM_READ &&
            tick_slot_reads==1u && !tick_cell_reads && tick_words[TICK_POINTER]==0u) {
            tick_words[TICK_POINTER]=address;
            tick_refreshing=2u;tick_prewrite_reads=0u;tick_extra_words[TICK_PREWRITE_ACTIVE]=tick_refreshing;
            const uint32_t *slot=(const uint32_t *)ff_sdk_shared_memory(SDK_TICK_SLOT,4u,FF_VM_READ,s);
            const uint32_t *before=(const uint32_t *)ff_sdk_shared_memory(SDK_TICK_CELL,4u,FF_VM_READ,s);
            tick_refreshing=0u;tick_extra_words[TICK_PREWRITE_ACTIVE]=0u;
            if (tick_prewrite_reads!=2u || sdk_tick_cell_mapping(1u) || *slot!=SDK_TICK_CELL || *before!=0u) stop(9,address,*before);
            tick_refreshing=1u;
            uint32_t *cell=(uint32_t *)ff_sdk_shared_memory(SDK_TICK_CELL,4u,FF_VM_WRITE,s);
            *cell=tick_words[TICK_MILLISECONDS];
            tick_refreshing=0u;
            ++tick_words[TICK_REFRESHES];tick_words[TICK_CELL_UPDATED]=tick_words[TICK_MILLISECONDS];
            ++tick_cell_reads;tick_words[TICK_CELL_READS]=tick_cell_reads;
            return ff_probe_guest_memory(address,bytes,access,sdk_current_target);
        }
        if (tick_stage==3u && address==tick_entry_esp && bytes==4u && access==FF_VM_READ && !tick_exit_frame_reads) {
            ++tick_exit_frame_reads;
            return ff_probe_guest_memory(address,bytes,access,sdk_current_target);
        }
        stop(9,address,access);
    }
    /* Charge every scalar access through the existing checked guest-memory path. */
    return ff_probe_guest_memory(address,bytes,access,sdk_current_target);
}
static void sdk_handoff_30943(void)
{
    if (!sdk_enabled) { ff_title_00030943_legacy(); return; }
    if ((g_esp&3u) || ff_vm_check(&virtual_memory,g_esp,4u,FF_VM_READ)) stop(9,g_esp,0);
    if (MEM32(g_esp)!=0x4bdcdu) { ff_title_00030943_legacy(); return; }
    if (sdk_active) stop(9,0x30943u,0);
    sdk_active=1;sdk_current_target=0x30943u;sdk_words[17]=sdk_current_target;
    ff_boot_thread *t=sdk_thread();
    if (g_esp<t->stack_low || g_esp>t->stack_top || 12u>t->stack_top-g_esp ||
        ff_vm_check(&virtual_memory,g_esp,12u,FF_VM_READ) || MEM32(g_esp+8u)!=0xffffffffu)
        stop(9,g_esp,0);
    sdk_words[1]++; sdk_words[3]=0xffffffffu; sdk_words[4]=0x4bdcdu; sdk_words[5]=t->id;
    sdk_words[6]=g_eax; sdk_words[7]=g_esp;
    sdk_words[11]=g_ebp; sdk_words[13]=g_seh_ebp; sdk_words[15]=g_eflags;
    uint32_t saved_seh=g_seh_ebp;
    uint32_t status=ff_sdk_shared_invoke();
    g_seh_ebp=saved_seh;
    sdk_words[3]=status; sdk_words[8]=g_eax; sdk_words[9]=g_esp;
    sdk_words[12]=g_ebp; sdk_words[14]=g_seh_ebp; sdk_words[16]=g_eflags;
    if (status || g_esp != sdk_words[7]+12u || g_eax || g_eflags!=sdk_words[15])
        stop(9,status,0);
    sdk_words[2]++; sdk_active=0;sdk_current_target=0;
}
void ff_title_00030943(void) { sdk_handoff_30943(); }

void ff_sdk_shared_memory_fault(uint32_t address,uint32_t bytes,uint32_t access,const ff_sdk_scalar *s)
{
    ff_sdk_shared_write(s);
    if (!bytes || (access!=FF_VM_READ && access!=FF_VM_WRITE) ||
        !ff_vm_check(&virtual_memory,address,bytes,access)) stop(9,address,access);
    /* Terminal RAM-only rejection; retain the existing per-access work bound
     * without routing a raw unresolved import into device/alias providers. */
    if (pfifo_consumer_enabled && pfifo_consumer_steps &&
        ++pfifo_consumer_vm_checks>=FF_PFIFO_CONSUMER_MAX_VM_CHECKS) {
        pfifo_consumer_pusher.classification=FF_PFIFO_PUSH_BUDGET;
        pfifo_consumer_budget_hit=1;stop(13,address,access);
    }
    vm_fault[0]=address;vm_fault[1]=bytes;vm_fault[2]=0;vm_fault[3]=access;
    vm_fault[4]=sdk_current_target;vm_fault[5]=g_esi;
    stop(10,address,access);
}
void ff_title_0003055A(void)
{
    if (sdk_enabled<2u) {ff_title_0003055A_legacy();return;}
    if (sdk_active) stop(9,0x3055au,0);
    sdk_active=1;sdk_current_target=0x3055au;sdk_words[17]=sdk_current_target;
    ff_boot_thread *t=sdk_thread();
    if ((g_esp&3u) || g_esp<t->stack_low || g_esp>t->stack_top ||
        12u>t->stack_top-g_esp || ff_vm_check(&virtual_memory,g_esp,12u,FF_VM_READ) ||
        MEM32(g_esp)!=0x4e828u || MEM32(g_esp+4u)!=0x2d0750u || MEM32(g_esp+8u)!=0x2d08a8u)
        stop(9,g_esp,0);
    ++sdk_words[1];++sdk_words[18];sdk_words[3]=0xffffffffu;
    sdk_words[4]=0x4e828u;sdk_words[5]=t->id;sdk_words[6]=g_eax;sdk_words[7]=g_esp;
    sdk_words[11]=g_ebp;sdk_words[13]=g_seh_ebp;sdk_words[15]=g_eflags;
    uint32_t saved_seh=g_seh_ebp;
    uint32_t status=ff_sdk_shared_invoke_target(0x3055au);
    g_seh_ebp=saved_seh;
    sdk_words[3]=status;sdk_words[8]=g_eax;sdk_words[9]=g_esp;
    sdk_words[12]=g_ebp;sdk_words[14]=g_seh_ebp;sdk_words[16]=g_eflags;
    if (status || g_esp!=sdk_words[7]+12u || g_ebp!=sdk_words[11] || g_eflags!=sdk_words[15])
        stop(9,status,0);
    ++sdk_words[2];++sdk_words[19];sdk_active=0;sdk_current_target=0;
}

/* Explicit modeled cold start, based on the pinned SDK's InitKernelData(164)=0.
 * This is typed import relocation, never an alias for encoded ordinal800000A4.
 * The one new data cell uses existing RO model backing and no new VM pages. */
static uint32_t sdk_launch_data_profile(uint32_t enable)
{
    if (enable>1u || sdk_active || local_mutant_wait_sealed || probe_trace_count ||
        kernel_call_count || parent_returned) return FF_BOOT_INVALID;
    if (!enable && !launch_data_enabled) return 0;
    if (!vm_ready || !disk_ready || virtual_memory.ram!=guest_ram ||
        FF_KERNEL_BINDING_COUNT>=64u ||
        SDK_LAUNCH_DATA_CELL!=FF_DISK_DATA_ADDRESS+4u ||
        ff_vm_check(&virtual_memory,SDK_LAUNCH_DATA_SLOT,4u,FF_VM_READ|FF_VM_WRITE) ||
        ff_vm_check(&virtual_memory,FF_DISK_DATA_ADDRESS,8u,FF_VM_READ) ||
        !ff_vm_check(&virtual_memory,SDK_LAUNCH_DATA_CELL,4u,FF_VM_WRITE) ||
        MEM32(SDK_LAUNCH_DATA_CELL)!=0u) return FF_BOOT_INVALID;
    const uint32_t expected=launch_data_enabled?SDK_LAUNCH_DATA_CELL:0x800000a4u;
    if (MEM32(SDK_LAUNCH_DATA_SLOT)!=expected ||
        (launch_data_enabled && MEM32(FF_DISK_DATA_ADDRESS)!=launch_data_disk)) return FF_BOOT_INVALID;
    if (enable && !launch_data_enabled) {
        launch_data_original=MEM32(SDK_LAUNCH_DATA_SLOT);
        launch_data_disk=MEM32(FF_DISK_DATA_ADDRESS);
        MEM32(SDK_LAUNCH_DATA_SLOT)=SDK_LAUNCH_DATA_CELL;
        launch_data_enabled=1u;
    } else if (!enable && launch_data_enabled) {
        MEM32(SDK_LAUNCH_DATA_SLOT)=launch_data_original;
        launch_data_enabled=0u;
    }
    return 0;
}

/* Ordinal 156 is a typed pointer-valued data import. Its cell is in the
 * already fixed RW guest-RAM span after the separate RO disk/LaunchData page.
 * Preflight requires a fresh zero cell and refuses nonzero contents; reset does
 * not establish this cell value or semantic ownership. */
static uint32_t sdk_tick_cell_mapping(uint32_t record)
{
    const uint32_t first=SDK_TICK_CELL/FF_VM_PAGE, last=(SDK_TICK_CELL+3u)/FF_VM_PAGE;
    if (SDK_TICK_CELL<SDK_TICK_OWNER_START || SDK_TICK_CELL+4u>SDK_TICK_OWNER_END || first!=last ||
        last>=FF_VM_MAX_PAGES) return FF_BOOT_INVALID;
    const uint16_t owner=virtual_memory.owner[first];
    const uint32_t committed=virtual_memory.committed[first];
    const uint32_t permission=virtual_memory.permission[first];
    const ff_vm_region *r=(owner && owner<=FF_VM_REGIONS)?&virtual_memory.regions[owner-1u]:0;
    if (record) {
        tick_extra_words[TICK_OWNER_ID]=owner;
        tick_extra_words[TICK_PAGE_COMMITTED]=committed;
        tick_extra_words[TICK_PAGE_PERMISSION]=permission;
        tick_extra_words[TICK_REGION_KIND]=r?r->kind:0u;
        tick_extra_words[TICK_REGION_BASE]=r?r->base:0u;
        tick_extra_words[TICK_REGION_BYTES]=r?r->bytes:0u;
        tick_extra_words[TICK_REGION_CELL_END]=r?r->base+r->bytes:0u;
    }
    if (!owner || !committed || permission!=(FF_VM_READ|FF_VM_WRITE) || !r ||
        r->kind!=FF_VM_FIXED || r->base!=SDK_TICK_OWNER_START ||
        r->bytes!=SDK_TICK_OWNER_END-SDK_TICK_OWNER_START ||
        (uint64_t)SDK_TICK_CELL+4u>(uint64_t)r->base+r->bytes) return FF_BOOT_INVALID;
    return FF_BOOT_OK;
}
static uint32_t sdk_tick_validate(void)
{
    if (!vm_ready || virtual_memory.ram!=guest_ram || FF_KERNEL_BINDING_COUNT+4u>64u ||
        SDK_TICK_SLOT==SDK_LAUNCH_DATA_SLOT || SDK_TICK_CELL<SDK_TICK_OWNER_START ||
        SDK_TICK_CELL>=SDK_TICK_OWNER_END || SDK_TICK_CELL==FF_DISK_DATA_ADDRESS ||
        SDK_TICK_CELL==SDK_LAUNCH_DATA_CELL ||
        ff_vm_check(&virtual_memory,SDK_TICK_SLOT,4u,FF_VM_READ|FF_VM_WRITE) ||
        ff_vm_check(&virtual_memory,SDK_TICK_CELL,4u,FF_VM_READ|FF_VM_WRITE) ||
        sdk_tick_cell_mapping(1u) ||
        ff_vm_check(&virtual_memory,FF_DISK_DATA_ADDRESS,4u,FF_VM_READ) ||
        !ff_vm_check(&virtual_memory,FF_DISK_DATA_ADDRESS,4u,FF_VM_WRITE) ||
        ff_vm_check(&virtual_memory,SDK_LAUNCH_DATA_SLOT,4u,FF_VM_READ|FF_VM_WRITE) ||
        ff_vm_check(&virtual_memory,SDK_LAUNCH_DATA_CELL,4u,FF_VM_READ) ||
        !ff_vm_check(&virtual_memory,SDK_LAUNCH_DATA_CELL,4u,FF_VM_WRITE) ||
        MEM32(SDK_LAUNCH_DATA_CELL)!=0u) return FF_BOOT_INVALID;
    const uint32_t expected_tick_slot=tick_enabled?SDK_TICK_CELL:SDK_TICK_RAW;
    const uint32_t expected_tick_cell=tick_enabled?tick_initial_cell:0u;
    const uint32_t expected_launch_slot=launch_data_enabled?SDK_LAUNCH_DATA_CELL:0x800000a4u;
    if (MEM32(SDK_TICK_SLOT)!=expected_tick_slot || MEM32(SDK_TICK_CELL)!=expected_tick_cell ||
        MEM32(SDK_LAUNCH_DATA_SLOT)!=expected_launch_slot ||
        (launch_data_enabled && MEM32(FF_DISK_DATA_ADDRESS)!=launch_data_disk)) return FF_BOOT_INVALID;
    return FF_BOOT_OK;
}
static uint32_t sdk_tick_profile(uint32_t enable)
{
    if (enable>1u || sdk_active || local_mutant_wait_sealed || probe_trace_count ||
        kernel_call_count || parent_returned) return FF_BOOT_INVALID;
    if (enable==tick_enabled) return enable?sdk_tick_validate():FF_BOOT_OK;
    uint32_t status=sdk_tick_validate();if (status) return status;
    if (enable) {
        tick_raw_slot=MEM32(SDK_TICK_SLOT);
        tick_initial_cell=MEM32(SDK_TICK_CELL);
        if (tick_raw_slot!=SDK_TICK_RAW || tick_initial_cell!=0u) return FF_BOOT_INVALID;
        MEM32(SDK_TICK_SLOT)=SDK_TICK_CELL;
        tick_enabled=1u;
        tick_words[TICK_IMPORT_RAW]=tick_raw_slot;
        tick_words[TICK_CELL_INITIAL]=tick_initial_cell;
        tick_words[TICK_CELL_UPDATED]=tick_initial_cell;
        tick_words[TICK_CELL_PERMISSIONS]=tick_extra_words[TICK_PAGE_PERMISSION];
        tick_words[TICK_OWNER_START]=tick_extra_words[TICK_REGION_BASE];
        tick_words[TICK_OWNER_END]=tick_extra_words[TICK_REGION_CELL_END];
        tick_words[TICK_DISK_VALUE]=MEM32(FF_DISK_DATA_ADDRESS);
        tick_words[TICK_LAUNCH_SLOT_VALUE]=MEM32(SDK_LAUNCH_DATA_SLOT);
        tick_words[TICK_LAUNCH_CELL_VALUE]=MEM32(SDK_LAUNCH_DATA_CELL);
    } else {
        /* Profiles can only be changed before execution; restore both the raw
         * import token and its pre-run cell value. */
        uint8_t *cell=(uint8_t *)ff_probe_guest_memory(SDK_TICK_CELL,4u,FF_VM_WRITE,0u);
        *(uint32_t *)cell=tick_initial_cell;
        MEM32(SDK_TICK_SLOT)=tick_raw_slot;
        tick_enabled=0u;
    }
    return FF_BOOT_OK;
}

/* Bounded exact original caller; MOV/RET leave lazy flags and non-scalars alone. */
void ff_title_000A6FA0(void)
{
    if (sdk_enabled<4u) {ff_title_000A6FA0_legacy();return;}
    if ((g_esp&3u) || ff_vm_check(&virtual_memory,g_esp,4u,FF_VM_READ)) stop(9,g_esp,0);
    if (MEM32(g_esp)!=0x41dfcu) {ff_title_000A6FA0_legacy();return;}
    if (sdk_active) stop(9,0xa6fa0u,0);
    sdk_active=1;sdk_current_target=0xa6fa0u;sdk_words[17]=sdk_current_target;
    ff_boot_thread *t=sdk_thread();
    if (g_esp<t->stack_low || g_esp>t->stack_top || 4u>t->stack_top-g_esp ||
        ff_vm_check(&virtual_memory,g_esp,4u,FF_VM_READ)) stop(9,g_esp,0);
    ++sdk_words[1];++leaf_attempts;sdk_words[3]=0xffffffffu;
    sdk_words[4]=0x41dfcu;sdk_words[5]=t->id;sdk_words[6]=g_eax;sdk_words[7]=g_esp;
    sdk_words[11]=g_ebp;sdk_words[13]=g_seh_ebp;sdk_words[15]=g_eflags;
    uint32_t saved_seh=g_seh_ebp;
    uint32_t status=ff_sdk_shared_invoke_target(0xa6fa0u);
    g_seh_ebp=saved_seh;
    sdk_words[3]=status;sdk_words[8]=g_eax;sdk_words[9]=g_esp;
    sdk_words[12]=g_ebp;sdk_words[14]=g_seh_ebp;sdk_words[16]=g_eflags;
    if (status || g_eax!=1u || g_esp!=sdk_words[7]+4u ||
        g_ebp!=sdk_words[11] || g_eflags!=sdk_words[15]) stop(9,status,0);
    ++sdk_words[2];++leaf_completed;sdk_active=0;sdk_current_target=0;
}

/* Exact observed two-argument caller. XOR flags are overwritten by caller ADD;
 * the C lazy flags image and all non-scalar state retain their existing policy. */
void ff_title_0004F890(void)
{
    if (sdk_enabled<5u) {ff_title_0004F890_legacy();return;}
    if (sdk_active) stop(9,0x4f890u,0);
    sdk_active=1;sdk_current_target=0x4f890u;sdk_words[17]=sdk_current_target;
    ff_boot_thread *t=sdk_thread();
    if ((g_esp&3u) || g_esp<t->stack_low || g_esp>t->stack_top ||
        12u>t->stack_top-g_esp || ff_vm_check(&virtual_memory,g_esp,12u,FF_VM_READ) ||
        MEM32(g_esp)!=0x41e15u || MEM32(g_esp+4u)!=0x2bd620u || MEM32(g_esp+8u)!=0x1eu)
        stop(9,g_esp,0);
    ++sdk_words[1];++init_attempts;sdk_words[3]=0xffffffffu;
    sdk_words[4]=0x41e15u;sdk_words[5]=t->id;sdk_words[6]=g_eax;sdk_words[7]=g_esp;
    sdk_words[11]=g_ebp;sdk_words[13]=g_seh_ebp;sdk_words[15]=g_eflags;
    uint32_t saved_seh=g_seh_ebp;
    uint32_t status=ff_sdk_shared_invoke_target(0x4f890u);
    g_seh_ebp=saved_seh;
    sdk_words[3]=status;sdk_words[8]=g_eax;sdk_words[9]=g_esp;
    sdk_words[12]=g_ebp;sdk_words[14]=g_seh_ebp;sdk_words[16]=g_eflags;
    if (status || g_eax!=0x2bd620u || g_ecx!=0x1eu || g_esp!=sdk_words[7]+4u ||
        g_ebp!=sdk_words[11] || g_eflags!=sdk_words[15]) stop(9,status,0);
    ++sdk_words[2];++init_completed;sdk_active=0;sdk_current_target=0;
}

/* Exact reached caller, terminal first-dependency profile. No tail result or
 * subsequent REP/callee contract is accepted by an unexpected host return. */
void ff_title_00048880(void)
{
    if (sdk_enabled<6u) {ff_title_00048880_legacy();return;}
    if (sdk_active) stop(9,0x48880u,0);
    sdk_active=1;sdk_current_target=0x48880u;sdk_words[17]=sdk_current_target;
    ff_boot_thread *t=sdk_thread();
    if ((g_esp&3u) || g_esp<t->stack_low || g_esp>t->stack_top ||
        8u>t->stack_top-g_esp || ff_vm_check(&virtual_memory,g_esp,8u,FF_VM_READ) ||
        MEM32(g_esp)!=0x4f7ffu || MEM32(g_esp+4u)!=1u) stop(9,g_esp,0);
    ++sdk_words[1];++setup_attempts;sdk_words[3]=0xffffffffu;
    sdk_words[4]=0x4f7ffu;sdk_words[5]=t->id;sdk_words[6]=g_eax;sdk_words[7]=g_esp;
    sdk_words[11]=g_ebp;sdk_words[13]=g_seh_ebp;sdk_words[15]=g_eflags;
    uint32_t status=ff_sdk_shared_invoke_target(0x48880u);
    if(sdk_enabled>=22u){
        if(status || sdk_invocation!=1u || tail_words[2]!=1u || tail_words[3] ||
           g_esp!=sdk_words[7]+4u || g_eax!=1u || g_ebx!=parent_setup_words[4] ||
           g_esi!=parent_setup_words[8] || g_edi!=parent_setup_words[9] || g_ebp!=sdk_words[11] ||
           g_df || g_seh_ebp!=sdk_words[13] || g_fs_base!=invocation_evidence[0].continuation[39] ||
           g_eflags!=sdk_words[15])stop(9,g_esp,0x4f7ffu);
        ff_sdk_scalar scalar;ff_sdk_shared_read(&scalar);
        const uint32_t *f=sdk_continuation_frame(4u,0x4f7ffu,&scalar);
        if(f[0]!=tail_words[6])stop(9,g_esp,0x4f7ffu);
        tail_words[3]=1u;tail_words[11]=g_esp;tail_words[12]=g_eax;tail_words[13]=status;
        sdk_words[3]=status;sdk_words[8]=g_eax;sdk_words[9]=g_esp;
        sdk_words[12]=g_ebp;sdk_words[14]=g_seh_ebp;sdk_words[16]=g_eflags;
        ++sdk_words[2];sdk_active=0;sdk_current_target=0;return;
    }
    ++setup_unexpected_returns;sdk_words[3]=status;
    stop(9,0x48880u,status);
}

/* Reached zero-explicit-argument original root. Its first dependency remains a
 * terminal guard, so no return or downstream caller result is manufactured. */
void ff_title_0004F5E0(void)
{
    if(sdk_enabled<23u){ff_title_0004F5E0_legacy();return;}
    if((sdk_enabled!=23u && (sdk_enabled!=24u && (sdk_enabled!=25u && (sdk_enabled!=26u && (sdk_enabled!=27u && (sdk_enabled!=28u && (sdk_enabled!=29u && (sdk_enabled!=30u && (sdk_enabled!=31u && (sdk_enabled!=32u && (sdk_enabled!=33u && (sdk_enabled!=34u && (sdk_enabled!=35u && (sdk_enabled!=36u && (sdk_enabled!=37u && (sdk_enabled!=38u && (sdk_enabled!=39u && (sdk_enabled!=40u && (sdk_enabled!=41u && (sdk_enabled!=42u && (sdk_enabled!=43u && (sdk_enabled!=44u && sdk_enabled!=45u)))))))))))))))))))))) || sdk_active || startup_words[49] || tail_words[3]!=1u ||
       g_esp!=tail_words[11] || g_eax!=1u || g_df)stop(9,g_esp,0x4f5e0u);
    sdk_active=1u;sdk_current_target=0x4f5e0u;sdk_words[17]=sdk_current_target;
    ff_boot_thread *t=sdk_thread();ff_sdk_scalar scalar;ff_sdk_shared_read(&scalar);
    const uint32_t *f=sdk_continuation_frame(4u,0x4f5e0u,&scalar);
    if(f[0]!=0x4f807u)stop(9,g_esp,0x4f5e0u);
    startup_words[49]=1u;startup_words[50]=g_esp;++sdk_words[1];
    sdk_words[3]=UINT32_MAX;sdk_words[4]=f[0];sdk_words[5]=t->id;sdk_words[6]=g_eax;sdk_words[7]=g_esp;
    sdk_words[11]=g_ebp;sdk_words[13]=g_seh_ebp;sdk_words[15]=g_eflags;
    uint32_t status=ff_sdk_shared_invoke_target(0x4f5e0u);
    ++startup_words[1];stop(9,0x4f5e0u,status);
}

void ff_sdk_shared_stos_event(uint32_t complete)
{
    if(sdk_enabled>=23u && sdk_current_target==0x4f5e0u){sdk_startup_stos(complete);return;}
    (void)sdk_thread();
    if (sdk_enabled<7u || sdk_current_target!=0x48880u || complete>1u) stop(9,0x49d50u,complete);
    if (sdk_enabled>=15u && parent_setup_words[17]==1u) {
        if (!complete) {
            ff_boot_thread *t=sdk_thread();
            if (parent_fill_words[0] || parent_fill_words[1] || g_eax || g_ecx!=0x310u ||
                g_edi!=0x2cb570u || g_df || g_esp!=parent_setup_words[14] || g_esp!=parent_setup_words[19]-4u ||
                (g_esp&3u) || g_esp<t->stack_low || g_esp>t->stack_top ||
                20u>t->stack_top-g_esp || ff_vm_check(&virtual_memory,g_esp,20u,FF_VM_READ))
                stop(9,g_esp,0x488a6u);
            ff_sdk_scalar live={g_eax,g_ecx,g_edx,g_ebx,g_esp,g_ebp,g_esi,g_edi,g_fs_base,(uint32_t)g_df};
            const uint32_t *frame=(const uint32_t *)ff_sdk_shared_memory(g_esp,20u,FF_VM_READ,&live);
            if (frame[0]!=0x259470u || frame[1]!=parent_setup_words[31] ||
                frame[2]!=parent_setup_words[9] || frame[3]!=parent_setup_words[1] || frame[4]!=parent_setup_words[31])
                stop(9,g_esp,0x488a6u);
            parent_fill_words[0]=1u;parent_fill_words[2]=g_eax;parent_fill_words[3]=g_ecx;
            parent_fill_words[4]=g_edi;parent_fill_words[5]=(uint32_t)g_df;parent_fill_words[6]=g_esp;
            parent_fill_words[7]=frame[3];parent_fill_words[8]=frame[0];
            parent_fill_words[14]=g_ebx;parent_fill_words[15]=g_esi;parent_fill_words[16]=g_ebp;
            parent_fill_words[17]=parent_setup_words[17];parent_fill_words[19]=0xc40u;
        } else {
            if (parent_fill_words[0]!=1u || parent_fill_words[1] || g_eax || g_ecx ||
                g_edi!=0x2cc1b0u || g_df || g_esp!=parent_fill_words[6] ||
                g_ebx!=parent_fill_words[14] || g_esi!=parent_fill_words[15] || g_ebp!=parent_fill_words[16])
                stop(9,g_esp,0x488a6u);
            parent_fill_words[1]=1u;parent_fill_words[9]=g_edi;parent_fill_words[10]=g_ecx;
            parent_fill_words[11]=g_eax;parent_fill_words[12]=g_esp;parent_fill_words[13]=(uint32_t)g_df;
        }
        return;
    }
    if (!complete) {
        if (stos_attempts!=stos_completed) stop(9,0x49d50u,0);
        ++stos_attempts;
    } else {
        if (stos_attempts!=stos_completed+1u || g_eax || g_ecx || g_edi!=0x2cc3bcu || g_df)
            stop(9,0x49d50u,1);
        ++stos_completed;stos_eax=g_eax;stos_ecx=g_ecx;stos_edi=g_edi;
    }
}

/* Only the actual empty standard-port observation is admitted initially.
 * This stage supplies no button/axis events and no asynchronous polling. */
uint32_t ff_probe_sdk_host_input(uint32_t source,uint32_t ports,uint32_t mask)
{
    if (sdk_active || host_words[0] || local_mutant_wait_sealed || probe_trace_count ||
        kernel_call_count || parent_returned || source!=1u || ports!=FF_CONTROLLER_MAX_PORTS)
        return FF_BOOT_INVALID;
    if (mask!=0u) return FF_BOOT_UNSUPPORTED;
    const uint8_t analog[FF_CONTROLLER_ANALOG_COUNT]={0};
    ff_controller_device_init(&host_controllers);
    for (uint32_t i=0;i<FF_CONTROLLER_MAX_PORTS;++i)
        if (ff_controller_device_set_port(&host_controllers,i,0u,0u,analog,0,0,0,0))
            return FF_BOOT_INVALID;
    host_words[0]=1u;host_words[1]=mask;
    return 0;
}

/* Adapt the matching SDK's XInitDevices host policy for an observed empty
 * browser snapshot. This is HOST SERVICE execution, not the original USB body.
 * 3925 source layout: Current/Changed/Previous at0/4/8 in each XPP record. */
void ff_sdk_shared_xinit(ff_sdk_scalar *scalar)
{
    ff_sdk_shared_write(scalar);
    ff_boot_thread *t=sdk_thread();
    if ((sdk_enabled!=9u && sdk_enabled!=10u && sdk_enabled!=11u && sdk_enabled!=12u && sdk_enabled!=13u && sdk_enabled!=14u && (sdk_enabled!=15u && sdk_enabled!=16u && (sdk_enabled!=17u && (sdk_enabled!=18u && (sdk_enabled!=19u && (sdk_enabled!=20u && (sdk_enabled!=21u && (sdk_enabled!=22u && (sdk_enabled!=23u && (sdk_enabled!=24u && (sdk_enabled!=25u && (sdk_enabled!=26u && (sdk_enabled!=27u && (sdk_enabled!=28u && (sdk_enabled!=29u && (sdk_enabled!=30u && (sdk_enabled!=31u && (sdk_enabled!=32u && (sdk_enabled!=33u && (sdk_enabled!=34u && (sdk_enabled!=35u && (sdk_enabled!=36u && (sdk_enabled!=37u && (sdk_enabled!=38u && (sdk_enabled!=39u && (sdk_enabled!=40u && (sdk_enabled!=41u && (sdk_enabled!=42u && (sdk_enabled!=43u && (sdk_enabled!=44u && sdk_enabled!=45u)))))))))))))))))))))))))))))) || sdk_current_target!=0x48880u || !host_words[0] || host_words[1] ||
        host_words[2] || host_words[3]) stop(9,0x24ca33u,0);
    if ((g_esp&3u) || g_esp<t->stack_low || g_esp>t->stack_top ||
        32u>t->stack_top-g_esp || ff_vm_check(&virtual_memory,g_esp,32u,FF_VM_READ) ||
        MEM32(g_esp)!=0x49d7bu || MEM32(g_esp+4u)!=2u || MEM32(g_esp+8u)!=g_esp+16u ||
        MEM32(g_esp+16u)!=0x24c2bcu || MEM32(g_esp+20u)!=4u ||
        MEM32(g_esp+24u)!=0x24c238u || MEM32(g_esp+28u)!=4u ||
        ff_vm_check(&virtual_memory,0x24c2bcu,12u,FF_VM_READ|FF_VM_WRITE) ||
        ff_vm_check(&virtual_memory,0x24c238u,12u,FF_VM_READ|FF_VM_WRITE)) stop(9,g_esp,0);
    uint32_t connected=0u;
    for (uint32_t i=0;i<FF_CONTROLLER_MAX_PORTS;++i)
        if (host_controllers.ports[i].state.connected) connected|=1u<<i;
    if (connected!=host_words[1]) stop(9,connected,0);
    /* Leave room for all six individually charged stores before mutation. */
    if (pfifo_consumer_enabled && pfifo_consumer_steps &&
        pfifo_consumer_vm_checks>=FF_PFIFO_CONSUMER_MAX_VM_CHECKS-6u) {
        pfifo_consumer_pusher.classification=FF_PFIFO_PUSH_BUDGET;
        pfifo_consumer_budget_hit=1;stop(13,0x24ca33u,FF_VM_WRITE);
    }
    ++host_words[2];host_words[4]=g_esp;host_words[6]=g_eax;
    host_words[8]=MEM32(g_esp+4u);host_words[9]=MEM32(g_esp+8u);
    const uint32_t types[2]={0x24c2bcu,0x24c238u};
    for (uint32_t i=0;i<2u;++i) for (uint32_t field=0;field<3u;++field)
        *(uint32_t *)ff_sdk_shared_memory(types[i]+4u*field,4u,FF_VM_WRITE,scalar)=0u;
    /* Native UpdatePorts changes masks only when connected!=clearedCurrent.
     * The admitted observed mask is zero, so that branch performs no write. */
    for (uint32_t i=0;i<2u;++i) for (uint32_t field=0;field<3u;++field)
        host_words[10u+i*3u+field]=MEM32(types[i]+4u*field);
    g_eax=0u;g_esp+=12u;
    host_words[5]=g_esp;host_words[7]=g_eax;++host_words[3];
    ff_sdk_shared_read(scalar);
}

/* One reached caller. These are observer buffers, never guest initialization. */
static const uint8_t literal45[43]={98,73,110,105,116,67,97,99,104,101,58,32,90,32,68,114,105,118,101,32,102,105,108,101,32,99,97,99,104,101,32,105,115,32,69,78,65,66,76,69,68,10,0};
static uint8_t object45[257],file45_before[65536],file45_after[65536];
static ff_fatx_directory_entry file45_entry;
static ff_guest_cpu cpu45_before;
static void sdk_run_c_log_body(ff_sdk_scalar*,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t*,uint32_t);
static int s45_overlap(uint32_t a,uint32_t n,uint32_t b,uint32_t m) {
 return (uint64_t)a+n>b && (uint64_t)b+m>a;
}
static void s45_file_state(uint32_t start) {
 for(uint32_t i=0;i<16u;i++)for(uint32_t k=0;k<7u;k++)logger45[start+7u*i+k]=ff_probe_fatx_file_word(i,k);
}
/* Private Goal A ledger for the two original SEH helpers.
 * Integrate only into a fresh source45-03 snapshot. This fragment observes
 * existing guest state; it does not write guest memory or alter helper code.
 */
typedef struct {
    uint32_t active, prolog_record, p, locals, descriptor, caller_ra;
    uint32_t old_seh, old_fshead, ebx, esi, edi, saved_reg_top, pre_ret;
} s45_ledger_frame;
typedef struct {
    uint32_t active, va, record, frame_index;
} s45_ledger_call;
static s45_ledger_frame s45_ledger_frames[12];
static s45_ledger_call s45_ledger_calls[12];

enum {
    S45L_MAGIC=103, S45L_ACTIVE, S45L_RECORDS, S45L_FRAME_DEPTH,
    S45L_CALL_DEPTH, S45L_EXPECT_COUNT, S45L_EXPECT_FIRST,
    S45L_EXPECT_LAST, S45L_EXPECT_ESP_COUNT, S45L_EXPECT_ESP_FIRST,
    S45L_EXPECT_ESP_LAST, S45L_PROLOG_ENTRIES, S45L_EPILOG_ENTRIES,
    S45L_PRE_RET_EVENTS, S45L_POST_RET_EVENTS, S45L_MAX_FRAME_DEPTH,
    S45L_INIT_COUNT, S45L_INIT_FIRST, S45L_INIT_LAST,
    S45L_INIT_ESP_COUNT, S45L_INIT_ESP_FIRST, S45L_INIT_ESP_LAST,
    S45L_FINISHED, S45L_COLLECTOR_CHECKS, S45L_PROOF_BITS
};
enum {
    S45L_VA=0, S45L_ENTRY_ESP, S45L_PRE_RET_ESP, S45L_POST_RET_ESP,
    S45L_ENTRY_EBX, S45L_ENTRY_ESI, S45L_ENTRY_EDI,
    S45L_POST_EBX, S45L_POST_ESI, S45L_POST_EDI,
    S45L_ENTRY_SEH, S45L_POST_SEH, S45L_ENTRY_FS_HEAD, S45L_POST_FS_HEAD,
    S45L_RETURN_ADDRESS, S45L_LOCALS_OR_FRAME, S45L_DIAG_EXPECTED,
    S45L_ESP_DIAG_EXPECTED, S45L_PROOF, S45L_PHASE, S45L_EVENT_WORDS=20
};
enum { S45L_PHASE_ENTRY=1u, S45L_PHASE_PRE_RET=2u, S45L_PHASE_POST_RET=3u };
enum { S45L_PROOF_ENTRY_READ=1u, S45L_PROOF_FRAME=2u,
       S45L_PROOF_POST=4u, S45L_PROOF_COLLECTOR=8u };
enum { S45L_HELPER_PROLOG=0x001e04ccu, S45L_HELPER_EPILOG=0x001e0505u };
#define S45L_FAILURE(va) stop(9u,(va),0x4b230u)

/* The logger bank has two approved ledger ranges around the frozen file table.
 * Logical words [0,192) map to logger45[320,512); [192,480) map to [736,1024).
 * A logical record may cross that boundary; no access touches [512,736).
 */
static uint32_t s45_ledger_phys(uint32_t logical)
{
    if(logical>=480u)S45L_FAILURE(logical);
    return logical<192u?320u+logical:736u+(logical-192u);
}
static uint32_t s45_ledger_get(uint32_t record,uint32_t field)
{
    if(record>=24u||field>=S45L_EVENT_WORDS)S45L_FAILURE(record);
    return logger45[s45_ledger_phys(record*S45L_EVENT_WORDS+field)];
}
static void s45_ledger_put(uint32_t record,uint32_t field,uint32_t value)
{
    if(record>=24u||field>=S45L_EVENT_WORDS)S45L_FAILURE(record);
    logger45[s45_ledger_phys(record*S45L_EVENT_WORDS+field)]=value;
}
static uint32_t s45_ledger_new_record(uint32_t va)
{
    uint32_t record=logger45[S45L_RECORDS];
    if(record>=24u)S45L_FAILURE(va);
    ++logger45[S45L_RECORDS];
    s45_ledger_put(record,S45L_VA,va);
    s45_ledger_put(record,S45L_PHASE,S45L_PHASE_ENTRY);
    return record;
}
static uint32_t s45_ledger_read32(uint32_t address,ff_sdk_scalar *s,uint32_t tag)
{
    ff_boot_thread *t=sdk_thread();
    if(!s||!t||address<t->stack_low||address>t->stack_top||
       4u>t->stack_top-address||ff_vm_check(&virtual_memory,address,4u,FF_VM_READ))
        S45L_FAILURE(tag);
    const uint8_t *p=(const uint8_t*)ff_sdk_shared_memory(address,4u,FF_VM_READ,s);
    return s43_u32(p);
}
static uint32_t s45_ledger_fshead(ff_boot_thread *t,ff_sdk_scalar *s,uint32_t tag)
{
    if(!t||!s||g_fs_base!=t->fs||t->fs>virtual_memory.bytes||
       4u>virtual_memory.bytes-t->fs||ff_vm_check(&virtual_memory,t->fs,4u,FF_VM_READ))
        S45L_FAILURE(tag);
    return s43_u32((const uint8_t*)ff_sdk_shared_memory(t->fs,4u,FF_VM_READ,s));
}
static void s45_ledger_context(ff_boot_thread *t,ff_sdk_scalar *s,uint32_t tag)
{
    if(!t||!s||sdk_enabled!=45u||!sdk_active||sdk_current_target!=0x4f5e0u||
       !logger45[64]||t->id!=logger45[70]||t->fs!=logger45[71]||
       g_fs_base!=t->fs||s->fs_base!=t->fs||g_df||s->df||
       dpc_frame.active||irql_pair.active)S45L_FAILURE(tag);
}
static void s45_ledger_collect(uint32_t out[6])
{
    /* Snapshot every field before the caller compares any of them. */
    for(uint32_t i=0;i<6u;i++)out[i]=ff_probe_abi_violation_word(i);
}
static void s45_ledger_check_collect(const uint32_t actual[6],uint32_t tag)
{
    const uint32_t expected[6]={logger45[S45L_EXPECT_COUNT],logger45[S45L_EXPECT_FIRST],
        logger45[S45L_EXPECT_LAST],logger45[S45L_EXPECT_ESP_COUNT],
        logger45[S45L_EXPECT_ESP_FIRST],logger45[S45L_EXPECT_ESP_LAST]};
    ++logger45[S45L_COLLECTOR_CHECKS];
    for(uint32_t i=0;i<6u;i++)if(actual[i]!=expected[i])S45L_FAILURE(tag+i);
}
static void s45_ledger_capture_expected(void)
{
    uint32_t actual[6];s45_ledger_collect(actual);
    s45_ledger_check_collect(actual,0x4b230u);
}

static void s45_ledger_init(void)
{
    if(sdk_enabled!=45u||!sdk_active||sdk_current_target!=0x4f5e0u||
       !logger45[64]||logger45[S45L_ACTIVE]||logger45[S45L_FINISHED])S45L_FAILURE(0x4b230u);
    ff_boot_thread *t=sdk_thread();ff_sdk_scalar s;ff_sdk_shared_read(&s);
    s45_ledger_context(t,&s,0x4b230u);
    memset(s45_ledger_frames,0,sizeof s45_ledger_frames);
    memset(s45_ledger_calls,0,sizeof s45_ledger_calls);
    for(uint32_t i=0;i<480u;i++)logger45[s45_ledger_phys(i)]=0u;
    for(uint32_t i=S45L_MAGIC;i<128u;i++)logger45[i]=0u;
    logger45[S45L_MAGIC]=0x4c454403u;
    logger45[S45L_ACTIVE]=1u;
    logger45[S45L_INIT_COUNT]=logger45[88u];
    logger45[S45L_INIT_FIRST]=logger45[89u];
    logger45[S45L_INIT_LAST]=logger45[90u];
    logger45[S45L_INIT_ESP_COUNT]=logger45[91u];
    logger45[S45L_INIT_ESP_FIRST]=logger45[92u];
    logger45[S45L_INIT_ESP_LAST]=logger45[93u];
    logger45[S45L_EXPECT_COUNT]=logger45[S45L_INIT_COUNT];
    logger45[S45L_EXPECT_FIRST]=logger45[S45L_INIT_FIRST];
    logger45[S45L_EXPECT_LAST]=logger45[S45L_INIT_LAST];
    logger45[S45L_EXPECT_ESP_COUNT]=logger45[S45L_INIT_ESP_COUNT];
    logger45[S45L_EXPECT_ESP_FIRST]=logger45[S45L_INIT_ESP_FIRST];
    logger45[S45L_EXPECT_ESP_LAST]=logger45[S45L_INIT_ESP_LAST];
    s45_ledger_capture_expected();
}

static void s45_ledger_trace(uint32_t kind,uint32_t address)
{
    if(address!=S45L_HELPER_PROLOG&&address!=S45L_HELPER_EPILOG)return;
    if(logger45[S45L_MAGIC]!=0x4c454403u||!logger45[S45L_ACTIVE]||
       logger45[S45L_FINISHED]||!logger45[64]||sdk_enabled!=45u||
       !sdk_active||sdk_current_target!=0x4f5e0u||kind<1u||kind>2u)
        S45L_FAILURE(address);
    s45_ledger_capture_expected();
    ff_boot_thread *t=sdk_thread();ff_sdk_scalar s;ff_sdk_shared_read(&s);
    s45_ledger_context(t,&s,address);
    if(!t||s.esp!=g_esp||(g_esp&3u)||
       g_esp<t->stack_low||g_esp>t->stack_top-4u)S45L_FAILURE(address);

    if(kind==1u) {
        uint32_t frame_index=UINT32_MAX;
        uint32_t locals=0u,descriptor=0u,caller_ra=0u,old_seh=0u,old_fshead=0u;
        uint32_t saved_top=0u,pre_ret=0u;
        if(address==S45L_HELPER_PROLOG) {
            if(g_esp>UINT32_MAX-12u||g_esp>t->stack_top-12u)S45L_FAILURE(g_esp);
            caller_ra=s45_ledger_read32(g_esp,&s,address);
            descriptor=s45_ledger_read32(g_esp+4u,&s,address);
            locals=s45_ledger_read32(g_esp+8u,&s,address);
            if(g_esp<t->stack_low+24u||locals>g_esp-t->stack_low-24u)
                S45L_FAILURE(g_esp);
            saved_top=g_esp-locals-20u;pre_ret=saved_top-4u;
            old_seh=g_seh_ebp;old_fshead=s45_ledger_fshead(t,&s,address);
            if(saved_top<t->stack_low||pre_ret<t->stack_low||
               (uint64_t)g_esp+12u>t->stack_top)S45L_FAILURE(g_esp);
            if(logger45[S45L_FRAME_DEPTH]>=12u)S45L_FAILURE(address);
            frame_index=logger45[S45L_FRAME_DEPTH];
        } else {
            uint32_t depth=logger45[S45L_FRAME_DEPTH];
            if(!depth)S45L_FAILURE(address);
            frame_index=depth-1u;
            s45_ledger_frame *f=&s45_ledger_frames[frame_index];
            if(!f->active||g_esp!=f->pre_ret||g_seh_ebp!=f->p+8u||
               s45_ledger_fshead(t,&s,address)!=f->p-8u)
                S45L_FAILURE(address);
            caller_ra=s45_ledger_read32(g_esp,&s,address);
            locals=f->locals;descriptor=f->descriptor;old_seh=f->old_seh;
            old_fshead=f->old_fshead;saved_top=f->saved_reg_top;pre_ret=f->pre_ret;
            if(s45_ledger_read32(saved_top,&s,address)!=f->edi||
               s45_ledger_read32(saved_top+4u,&s,address)!=f->esi||
               s45_ledger_read32(saved_top+8u,&s,address)!=f->ebx||
               s45_ledger_read32(f->p+8u,&s,address)!=f->old_seh||
               s45_ledger_read32(f->p-8u,&s,address)!=f->old_fshead)
                S45L_FAILURE(address);
        }
        uint32_t rec=s45_ledger_new_record(address);
        if(logger45[S45L_CALL_DEPTH]>=12u)S45L_FAILURE(address);
        s45_ledger_call *c=&s45_ledger_calls[logger45[S45L_CALL_DEPTH]++];
        c->active=1u;c->va=address;c->record=rec;c->frame_index=frame_index;
        s45_ledger_put(rec,S45L_ENTRY_ESP,g_esp);
        s45_ledger_put(rec,S45L_ENTRY_EBX,g_ebx);
        s45_ledger_put(rec,S45L_ENTRY_ESI,g_esi);
        s45_ledger_put(rec,S45L_ENTRY_EDI,g_edi);
        s45_ledger_put(rec,S45L_ENTRY_SEH,g_seh_ebp);
        s45_ledger_put(rec,S45L_ENTRY_FS_HEAD,s45_ledger_fshead(t,&s,address));
        s45_ledger_put(rec,S45L_RETURN_ADDRESS,caller_ra);
        s45_ledger_put(rec,S45L_LOCALS_OR_FRAME,locals);
        s45_ledger_put(rec,S45L_PROOF,S45L_PROOF_ENTRY_READ);
        if(address==S45L_HELPER_PROLOG) {
            s45_ledger_frame *f=&s45_ledger_frames[frame_index];
            memset(f,0,sizeof *f);f->active=1u;f->prolog_record=rec;f->p=g_esp;
            f->locals=locals;f->descriptor=descriptor;f->caller_ra=caller_ra;
            f->old_seh=old_seh;f->old_fshead=old_fshead;f->ebx=g_ebx;
            f->esi=g_esi;f->edi=g_edi;f->saved_reg_top=saved_top;f->pre_ret=pre_ret;
            ++logger45[S45L_FRAME_DEPTH];
            ++logger45[S45L_PROLOG_ENTRIES];
            if(logger45[S45L_FRAME_DEPTH]>logger45[S45L_MAX_FRAME_DEPTH])
                logger45[S45L_MAX_FRAME_DEPTH]=logger45[S45L_FRAME_DEPTH];
        } else {
            s45_ledger_frame *f=&s45_ledger_frames[frame_index];
            if(g_esp!=f->saved_reg_top-4u||g_seh_ebp!=f->p+8u)
                S45L_FAILURE(address);
            ++logger45[S45L_EPILOG_ENTRIES];
        }
        logger45[S45L_PROOF_BITS]|=S45L_PROOF_ENTRY_READ;
        return;
    }

    if(!logger45[S45L_CALL_DEPTH])S45L_FAILURE(address);
    s45_ledger_call *c=&s45_ledger_calls[logger45[S45L_CALL_DEPTH]-1u];
    if(!c->active||c->va!=address||
       !s45_ledger_frames[c->frame_index].active||
       s45_ledger_get(c->record,S45L_PHASE)!=S45L_PHASE_ENTRY)
        S45L_FAILURE(address);
    uint32_t rec=c->record,frame_index=c->frame_index;
    s45_ledger_frame *f=&s45_ledger_frames[frame_index];
    if(address==S45L_HELPER_PROLOG) {
        const uint32_t p=s45_ledger_get(rec,S45L_ENTRY_ESP),n=f->locals;
        if(p<n+24u||g_esp!=p-n-24u||
           s45_ledger_read32(g_esp,&s,address)!=f->caller_ra||
           s45_ledger_read32(f->saved_reg_top,&s,address)!=f->edi||
           s45_ledger_read32(f->saved_reg_top+4u,&s,address)!=f->esi||
           s45_ledger_read32(f->saved_reg_top+8u,&s,address)!=f->ebx||
           s45_ledger_read32(p-16u,&s,address)!=f->saved_reg_top||
           s45_ledger_read32(p-8u,&s,address)!=f->old_fshead||
           s45_ledger_read32(p,&s,address)!=f->descriptor||
           s45_ledger_read32(p+4u,&s,address)!=0xffffffffu||
           s45_ledger_read32(p+8u,&s,address)!=f->old_seh||
           s45_ledger_fshead(t,&s,address)!=p-8u||
           g_seh_ebp!=f->old_seh)
            S45L_FAILURE(address);
    } else {
        if(g_esp!=f->p+8u||g_ebx!=f->ebx||g_esi!=f->esi||g_edi!=f->edi||
           s45_ledger_read32(f->p+8u,&s,address)!=
               s45_ledger_get(rec,S45L_RETURN_ADDRESS)||
           g_seh_ebp!=f->p+8u||s45_ledger_fshead(t,&s,address)!=f->old_fshead)
            S45L_FAILURE(address);
        /* The helper's pushed return address now occupies the known-lifetime
         * slot B. The old saved EBP word is intentionally not required here. */
    }
    s45_ledger_put(rec,S45L_PRE_RET_ESP,g_esp);
    s45_ledger_put(rec,S45L_PROOF,s45_ledger_get(rec,S45L_PROOF)|S45L_PROOF_FRAME);
    s45_ledger_put(rec,S45L_PHASE,S45L_PHASE_PRE_RET);
    ++logger45[S45L_PRE_RET_EVENTS];
}

static void s45_ledger_esp(const char *name,const char *tag)
{
    if(sdk_enabled!=45u||!logger45[64])return;
    if(!tag||(strcmp(tag,"after call 0x001E04CC")&&
              strcmp(tag,"after call 0x001E0505")))return;
    if(logger45[S45L_MAGIC]!=0x4c454403u||!logger45[S45L_ACTIVE]||
       logger45[S45L_FINISHED]||!name||!name[0]||!logger45[S45L_CALL_DEPTH])
        S45L_FAILURE(0x4b230u);
    s45_ledger_call *c=&s45_ledger_calls[logger45[S45L_CALL_DEPTH]-1u];
    const uint32_t va=!strcmp(tag,"after call 0x001E04CC")?S45L_HELPER_PROLOG:S45L_HELPER_EPILOG;
    if(!c->active||c->va!=va||
       s45_ledger_get(c->record,S45L_PHASE)!=S45L_PHASE_PRE_RET)
        S45L_FAILURE(va);
    uint32_t rec=c->record,frame_index=c->frame_index;
    s45_ledger_frame *f=&s45_ledger_frames[frame_index];
    ff_boot_thread *t=sdk_thread();ff_sdk_scalar s;ff_sdk_shared_read(&s);
    const uint32_t post_esp=g_esp,post_ebx=g_ebx,post_esi=g_esi,post_edi=g_edi;
    const uint32_t post_seh=g_seh_ebp,post_fs=s45_ledger_fshead(t,&s,va);
    uint32_t actual[6];s45_ledger_collect(actual);
    s45_ledger_context(t,&s,va);
    if(s.esp!=post_esp)S45L_FAILURE(va);
    if(va==S45L_HELPER_PROLOG) {
        const uint32_t p=f->p,n=f->locals;
        if(p<n+20u||post_esp!=p-n-20u||post_ebx!=f->ebx||
           post_esi!=f->esi||post_edi!=f->edi||post_seh!=p+8u||
           post_fs!=p-8u)S45L_FAILURE(va);
    } else {
        const uint32_t b=f->p+8u;
        if(post_esp!=b+4u||post_seh!=f->old_seh||post_fs!=f->old_fshead||
           post_ebx!=f->ebx||post_esi!=f->esi||post_edi!=f->edi||
           s45_ledger_read32(b,&s,va)!=s45_ledger_get(rec,S45L_RETURN_ADDRESS))
            S45L_FAILURE(va);
    }
    const uint32_t entry_esp=s45_ledger_get(rec,S45L_ENTRY_ESP);
    const int diagnostic=(post_ebx!=s45_ledger_get(rec,S45L_ENTRY_EBX)||
        post_esi!=s45_ledger_get(rec,S45L_ENTRY_ESI)||
        post_edi!=s45_ledger_get(rec,S45L_ENTRY_EDI)||
        (uint64_t)post_esp<(uint64_t)entry_esp+4u);
    const int esp_counter=(diagnostic&&post_esp!=entry_esp);
    s45_ledger_put(rec,S45L_POST_RET_ESP,post_esp);
    s45_ledger_put(rec,S45L_POST_EBX,post_ebx);
    s45_ledger_put(rec,S45L_POST_ESI,post_esi);
    s45_ledger_put(rec,S45L_POST_EDI,post_edi);
    s45_ledger_put(rec,S45L_POST_SEH,post_seh);
    s45_ledger_put(rec,S45L_POST_FS_HEAD,post_fs);
    s45_ledger_put(rec,S45L_DIAG_EXPECTED,(uint32_t)diagnostic);
    s45_ledger_put(rec,S45L_ESP_DIAG_EXPECTED,(uint32_t)esp_counter);
    s45_ledger_put(rec,S45L_PROOF,s45_ledger_get(rec,S45L_PROOF)|S45L_PROOF_POST);
    s45_ledger_put(rec,S45L_PHASE,S45L_PHASE_POST_RET);

    /* Mirror recomp_abi_violation_log exactly, deriving each expected word
     * from the observed invocation. Its ESP counter tests != esp0, where esp0
     * is this helper's entry ESP, and differs from the ABI predicate's < P+4.
     */
    if(diagnostic) {
        if(!logger45[S45L_EXPECT_FIRST])logger45[S45L_EXPECT_FIRST]=va;
        logger45[S45L_EXPECT_LAST]=va;++logger45[S45L_EXPECT_COUNT];
        if(esp_counter) {
            if(!logger45[S45L_EXPECT_ESP_FIRST])logger45[S45L_EXPECT_ESP_FIRST]=va;
            logger45[S45L_EXPECT_ESP_LAST]=va;++logger45[S45L_EXPECT_ESP_COUNT];
        }
    }
    s45_ledger_check_collect(actual,va);
    s45_ledger_put(rec,S45L_PROOF,s45_ledger_get(rec,S45L_PROOF)|S45L_PROOF_COLLECTOR);
    ++logger45[S45L_POST_RET_EVENTS];
    c->active=0u;--logger45[S45L_CALL_DEPTH];
    if(va==S45L_HELPER_EPILOG) {
        if(frame_index+1u!=logger45[S45L_FRAME_DEPTH])S45L_FAILURE(va);
        f->active=0u;--logger45[S45L_FRAME_DEPTH];
    }
}

static void s45_ledger_finish(void)
{
    if(logger45[S45L_MAGIC]!=0x4c454403u||!logger45[S45L_ACTIVE]||
       logger45[S45L_FINISHED]||sdk_enabled!=45u||!sdk_active||
       sdk_current_target!=0x4f5e0u||!logger45[64]||logger45[S45L_CALL_DEPTH]||
       logger45[S45L_FRAME_DEPTH]||
       logger45[S45L_PROLOG_ENTRIES]!=logger45[S45L_EPILOG_ENTRIES]||
       logger45[S45L_RECORDS]>24u)S45L_FAILURE(0x4b230u);
    ff_boot_thread *t=sdk_thread();ff_sdk_scalar s;ff_sdk_shared_read(&s);
    s45_ledger_context(t,&s,0x4b230u);
    for(uint32_t i=0;i<logger45[S45L_RECORDS];i++)
        if(s45_ledger_get(i,S45L_PHASE)!=S45L_PHASE_POST_RET||
           (s45_ledger_get(i,S45L_PROOF)&15u)!=15u)S45L_FAILURE(i);
    const uint32_t final[6]={logger45[94u],logger45[95u],logger45[96u],
        logger45[97u],logger45[98u],logger45[99u]};
    s45_ledger_check_collect(final,0x4b230u);
    logger45[S45L_PROOF_BITS]|=S45L_PROOF_FRAME|S45L_PROOF_POST|S45L_PROOF_COLLECTOR;
    logger45[S45L_FINISHED]=1u;logger45[S45L_ACTIVE]=0u;
}
#undef S45L_FAILURE

static void s45_trace(uint32_t kind,uint32_t address) {
 uint32_t *w=logger45;if(sdk_enabled!=45u||!w[64])return;
 ff_boot_thread *t=sdk_thread();
 if(g_fs_base!=t->fs||g_df||!sdk_active||sdk_current_target!=0x4f5e0u)stop(9,g_esp,address);
 s45_ledger_trace(kind,address);
 if(address!=0x4b230u && address!=0x1e041fu && address!=0x31227u &&
    address!=0x1e01adu && address!=0x1e0c90u && address!=0x1dfe85u)return;
 ff_sdk_scalar s;ff_sdk_shared_read(&s);
 const uint32_t *f=(const uint32_t*)s43_read(g_esp,12u,&s);
 if(g_esp<t->stack_low || g_esp>t->stack_top-12u || w[100]>=12u)stop(9,g_esp,address);
 uint32_t *event=w+128u+16u*w[100]++;
 event[0]=kind;event[1]=address;event[2]=g_esp;event[3]=g_eax;event[4]=g_ecx;event[5]=g_edx;
 event[6]=g_ebx;event[7]=g_ebp;event[8]=g_esi;event[9]=g_edi;event[10]=g_fs_base;
 event[11]=(uint32_t)g_df;event[12]=g_seh_ebp;event[13]=f[0];event[14]=f[1];event[15]=f[2];
 if(address==0x4b230u) {
  if(g_esp!=w[2]||f[0]!=0x4f6c2u||f[1]!=0x25c89cu||
     g_ebx!=w[8]||g_esi!=w[9]||g_edi!=w[10]||g_fs_base!=w[71]||g_eflags!=w[7])stop(9,g_esp,address);
  if(kind==1u){if(w[66]||w[67])stop(9,address,kind);w[66]=1u;}
  else {if(kind!=2u||w[66]!=1u||w[67])stop(9,address,kind);w[67]=1u;w[72]=g_esp;w[73]=g_eax;}
 }
 if(kind==1u && address==0x1e041fu &&
    (g_esp!=w[2]-32u||f[0]!=0x4b2b8u||f[1]!=0x2cc4f8u||f[2]!=0x25c89cu||
     *(const uint32_t*)s43_read(g_esp+12u,4u,&s)!=w[2]+8u))stop(9,g_esp,address);
 /* The ordinary C formatter still owns every va_list-derived access. */
 if(kind==2u && address==0x1e041fu) {
  if(g_esp!=w[2]-32u||g_eax!=42u)stop(9,g_esp,address);w[74]=1u;
 }
}
void ff_sdk_shared_log_bridge45(ff_sdk_scalar *s) {
 if(!s||sdk_enabled!=45u||!sdk_active||sdk_current_target!=0x4f5e0u||logger45[0]||logger45[65]||
    bridge44[5]!=1u||bridge44[0]||!allocator44.complete||s->esp!=0x03c10e5cu||s->df)stop(9,s?s->esp:0u,0x4b230u);
 ff_sdk_shared_write(s);ff_boot_thread *t=sdk_thread();uint32_t *w=logger45;
 if(s->esp<t->stack_low+0x1000u||s->esp>t->stack_top-12u||
    ff_vm_check(&virtual_memory,s->esp-0x1000u,0x100cu,FF_VM_READ|FF_VM_WRITE)||
    s->fs_base!=t->fs||g_seh_ebp!=s->ebp||g_seh_ebp!=bridge44[9]||
    dpc_frame.active||irql_pair.active)stop(9,s->esp,0x4b230u);
 sdk_shared_remaining_budget(16384u,0x4b230u);
 const uint32_t *f=(const uint32_t*)s43_read(s->esp,12u,s);
 if(f[0]!=0x4f6c2u||f[1]!=0x25c89cu)stop(9,s->esp,0x4b230u);
 w[21]=f[2]; /* ambient, never a formal argument */
 const uint8_t *format=s43_read(0x25c89cu,43u,s);
 if(memcmp(format,literal45,43u))stop(9,0x25c89cu,0x4b230u);
 const uint32_t object=s43_u32(s43_read(0x28525cu,4u,s));
 const uint32_t gate=s43_u32(s43_read(0x2cccf8u,4u,s));
 if(object>FF_GUEST_BYTES-257u||object!=record_log_words[28]||gate)stop(2,0x4b230u,0u);
 memcpy(object45,s43_read(object,257u,s),257u);
 /* Revalidate the retained, current synthetic log provider; do not force it. */
 if(object45[0]!='z'||object45[1]!=':'||object45[2]!='\\'||object45[15]||
    memcmp(object45+3u,copy_source+64u,12u)||object45[256]!=6u||record_log_words[29]!=6u)
  stop(2,0x4b230u,0u);
 const uint32_t starts[5]={0x28525cu,object,0x2cccf8u,0x25c89cu,0x2cc4f8u};
 const uint32_t lengths[5]={4u,257u,4u,43u,43u};
 for(uint32_t i=0;i<5u;i++) {
  if(s45_overlap(starts[i],lengths[i],t->stack_low,t->stack_top-t->stack_low)||
     s45_overlap(starts[i],lengths[i],t->fs,4u))stop(9,starts[i],0x4b230u);
  for(uint32_t j=0;j<i;j++)if(s45_overlap(starts[i],lengths[i],starts[j],lengths[j]))stop(9,starts[i],0x4b230u);
 }
 (void)ff_sdk_shared_memory(0x2cc4f8u,43u,FF_VM_READ|FF_VM_WRITE,s);
 w[26]=42u;w[27]=0u;w[28]=object;w[29]=object45[256];w[30]=object45[0];w[31]=gate;
 w[32]=g_seh_ebp;w[70]=t->id;w[71]=s->fs_base;w[75]=t->stack_low;w[76]=t->stack_top;
 w[77]=s43_u32(s43_read(t->fs,4u,s));w[78]=kernel_call_count;w[79]=g_eflags;
 if(!fatx_ready||fatx_volume.disk!=&local_disk||fatx_volume.space!=&object_namespace||
    fatx_volume.mount_count!=1u||fatx_volume.active||fatx_volume.file_count)stop(2,0x4b230u,0u);
 w[80]=fatx_volume.mounts[0].id;w[81]=fatx_volume.opened;w[82]=fatx_volume.closed;
 if(ff_fatx_lookup(&fatx_volume,w[80],object45+3u,12u,0x40u,&file45_entry)||
    file45_entry.size>65536u-43u||file45_entry.size+43u>fatx_volume.mounts[0].cluster_bytes)
  stop(2,0x4b230u,0u);
 uint32_t bytes=0;
 if(ff_fatx_read_entry(&fatx_volume,w[80],&file45_entry,0u,file45_before,file45_entry.size,&bytes)||bytes!=file45_entry.size)
  stop(9,bytes,0x4b230u);
 w[83]=file45_entry.size;w[84]=file45_entry.first_cluster;s45_file_state(512u);
 ff_capture_cpu(&cpu45_before);
 for(uint32_t i=0;i<6u;i++)w[88u+i]=ff_probe_abi_violation_word(i);
 w[64]=1u;
 s45_ledger_init();
 sdk_run_c_log_body(s,0x4f6c2u,0x25c89cu,w[21],0u,w,0u);
 if(!w[66]||!w[67]||!w[74]||w[1]!=1u||s->esp!=w[2]+4u||s->eax!=0u||g_seh_ebp!=w[32]||
    s->fs_base!=w[71]||s43_u32(s43_read(t->fs,4u,s))!=w[77]||
    memcmp(s43_read(w[28],257u,s),object45,257u)||s43_u32(s43_read(0x28525cu,4u,s))!=w[28]||
    s43_u32(s43_read(0x2cccf8u,4u,s))||memcmp(s43_read(0x2cc4f8u,43u,s),literal45,43u))stop(9,s->esp,0x4b230u);
 ff_guest_cpu after;ff_capture_cpu(&after);s44_fp_preserved(&cpu45_before,&after);w[85]=1u;
 for(uint32_t i=0;i<6u;i++)w[94u+i]=ff_probe_abi_violation_word(i);
 s45_ledger_finish();
 ff_fatx_directory_entry entry;
 if(ff_fatx_lookup(&fatx_volume,w[80],object45+3u,12u,0x40u,&entry)||entry.first_cluster!=file45_entry.first_cluster||
    entry.size<file45_entry.size||entry.size>65536u||fatx_volume.active||fatx_volume.file_count||
    fatx_volume.opened!=w[81]+1u||fatx_volume.closed!=w[82]+1u)stop(9,s->esp,0x4b230u);
 bytes=0;
 if(ff_fatx_read_entry(&fatx_volume,w[80],&entry,0u,file45_after,entry.size,&bytes)||bytes!=entry.size||
    memcmp(file45_before,file45_after,file45_entry.size))stop(9,bytes,0x4b230u);
 /* Existing text-mode logger emits CR before LF. Verify complete bytes, not a hash. */
 uint32_t n=file45_entry.size;
 for(uint32_t i=0;i<42u;i++) {
  if(literal45[i]=='\n'){if(n>=entry.size||file45_after[n++]!='\r')stop(9,n,0x4b230u);}
  if(n>=entry.size||file45_after[n++]!=literal45[i])stop(9,n,0x4b230u);
 }
 if(n!=entry.size)stop(9,n,0x4b230u);
 w[86]=entry.size;w[87]=entry.size-file45_entry.size;s45_file_state(624u);
 if(memcmp(w+512u,w+624u,112u*sizeof(uint32_t)))stop(9,s->esp,0x4b230u);
 w[68]=1u;w[65]=1u;w[64]=0u;
}
static void s45_post(ff_sdk_scalar *s) {
 uint32_t *w=logger45;(void)sdk_thread();
 if(!w[65]||!w[68]||w[69]||w[64]||s->esp!=w[2]+4u||s->eax!=w[11]||
    s->ebp!=w[4]||s->ebx!=w[8]||s->esi!=w[9]||s->edi!=w[10]||
    s->fs_base!=w[71]||s->df||g_seh_ebp!=w[32])stop(9,s->esp,0x4f6c2u);
 w[69]=1u;w[101]=s->esp;w[102]=s->eax;
}

/* Reuse the retained C body without copying it. Each caller has its own exact
 * admission and telemetry bank; both C dependency stubs remain terminal. */
static void sdk_run_c_log_body(ff_sdk_scalar *s,uint32_t ret,uint32_t arg0,uint32_t arg1,
                               uint32_t log_gate,uint32_t *words,uint32_t attempt_already_recorded)
{
    ff_boot_thread *t=sdk_thread();
    const uint32_t record=(sdk_enabled==45u && logger45[64]==1u && words==logger45 && ret==0x4f6c2u && arg0==0x25c89cu) || ((sdk_enabled==28u || (sdk_enabled==29u || (sdk_enabled==30u || (sdk_enabled==31u || (sdk_enabled==32u || (sdk_enabled==33u || (sdk_enabled==34u || (sdk_enabled==35u || (sdk_enabled==36u || (sdk_enabled==37u || (sdk_enabled==38u || (sdk_enabled==39u || (sdk_enabled==40u || (sdk_enabled==41u || (sdk_enabled==42u || (sdk_enabled==43u || (sdk_enabled==44u || sdk_enabled==45u))))))))))))))))) && sdk_current_target==0x4f5e0u && words==record_log_words &&
        ret==0x4eb7bu && arg0==0x25c470u && arg1==0x03c10e58u);
    if (!s || !words || !sdk_active || (!record && sdk_current_target!=0x48880u) ||
        !host_words[0] || host_words[3]!=1u || device_stos[0]!=1u || device_stos[1]!=1u ||
        (!record && arg1!=0u) || log_gate!=0u ||
        s->df!=0u || (s->esp&3u) || s->esp<t->stack_low || s->esp>t->stack_top ||
        12u>t->stack_top-s->esp ||
        (!record && g_ebp!=g_seh_ebp) || s->ebp!=g_ebp || g_esp!=s->esp)
        stop(9,s?s->esp:0u,0x4b230u);
    if (!attempt_already_recorded) words[0]++;
    words[2]=s->esp;words[4]=s->ebp;
    words[5]=g_ebp;words[6]=g_seh_ebp;
    words[7]=g_eflags;words[8]=s->ebx;
    words[9]=s->esi;words[10]=s->edi;
    words[20]=arg0;words[21]=arg1;
    words[22]=ret;words[23]=log_gate;words[24]=s->df;
    const uint32_t saved_ebp=g_ebp,saved_seh=g_seh_ebp,saved_flags=g_eflags;
    ff_sdk_shared_write(s);
    /* write() must not synthesize a different inherited FPO frame. */
    if (g_ebp!=saved_ebp || g_seh_ebp!=saved_seh || (!record && g_seh_ebp!=g_ebp))
        stop(9,g_ebp,0x4b230u);
    ff_title_0004B230();
    /* The reached C path calls the framed C provider 0x1E041F, which publishes
       its own EBP to both globals and leaves that frame published on return.
       Record that generated result, then restore the enclosing SDK boundary's
       saved C-side frame globals, matching the existing SDK handoff convention. */
    const uint32_t body_ebp=g_ebp,body_seh=g_seh_ebp;
    words[3]=g_esp;words[11]=g_eax;
    words[12]=g_ebx;words[13]=g_esp;
    words[14]=g_ebp;words[15]=g_esi;
    words[16]=g_edi;words[17]=body_ebp;
    words[18]=body_seh;words[19]=g_eflags;
    words[25]=(uint32_t)g_df;
    if (g_esp!=words[2]+4u || g_ebx!=words[8] ||
        g_esi!=words[9] || g_edi!=words[10] ||
        g_eflags!=saved_flags || g_df!=0)
        stop(9,g_esp,0x4b230u);
    g_ebp=saved_ebp;g_seh_ebp=saved_seh;
    ff_sdk_shared_read(s);
    if(s->esp!=words[2]+4u||s->ebp!=words[4]||s->df!=0u)
        stop(9,s->esp,0x4b230u);
    words[1]++;
}
/* Extra plain-RAM reads delimit admission; the original formatter, debug
 * provider and later control read still execute in their original order. */
void ff_sdk_shared_log_bridge_record(ff_sdk_scalar *s,uint32_t ret,uint32_t literal,
                                     uint32_t ambient,uint32_t gate)
{
    ff_sdk_shared_write(s);ff_boot_thread *t=sdk_thread();uint32_t *w=record_log_words;
    if((sdk_enabled!=28u && (sdk_enabled!=29u && (sdk_enabled!=30u && (sdk_enabled!=31u && (sdk_enabled!=32u && (sdk_enabled!=33u && (sdk_enabled!=34u && (sdk_enabled!=35u && (sdk_enabled!=36u && (sdk_enabled!=37u && (sdk_enabled!=38u && (sdk_enabled!=39u && (sdk_enabled!=40u && (sdk_enabled!=41u && (sdk_enabled!=42u && (sdk_enabled!=43u && (sdk_enabled!=44u && sdk_enabled!=45u))))))))))))))))) || !sdk_active || sdk_current_target!=0x4f5e0u || w[0] || w[1] ||
       copy_words[4]!=1u || ret!=0x4eb7bu || literal!=0x25c470u || ambient!=0x03c10e58u || gate ||
       g_esp!=0x03c10c5cu || g_eax!=0x70038u || g_ebp!=0x70038u || g_df ||
       g_ebx!=copy_words[70] || g_esi!=copy_words[71] || g_edi!=copy_words[72] ||
       g_fs_base!=copy_words[75] || g_seh_ebp!=copy_words[76] || g_seh_ebp!=ambient ||
       g_esp<t->stack_low+0x800u || g_esp>t->stack_top-12u ||
       ff_vm_check(&virtual_memory,g_esp-0x800u,0x80cu,FF_VM_READ|FF_VM_WRITE))stop(9,g_esp,0x4b230u);
    sdk_shared_remaining_budget(16384u,0x4b230u);
    const uint32_t *f=(const uint32_t *)ff_sdk_shared_memory(g_esp,12u,FF_VM_READ,s);
    if(f[0]!=ret || f[1]!=literal || f[2]!=ambient)stop(9,g_esp,0x4b230u);
    const uint32_t actual_gate=*(uint32_t *)ff_sdk_shared_memory(0x2cccf8u,4u,FF_VM_READ,s);
    const uint32_t object=*(uint32_t *)ff_sdk_shared_memory(0x28525cu,4u,FF_VM_READ,s);
    if(actual_gate || object!=0x285158u)stop(9,object,0x4b230u);
    const uint8_t *data=ff_sdk_shared_memory(object,257u,FF_VM_READ,s);
    if(data[0]!=122u || data[1]!=':' || data[2]!='\\' || data[15] || data[256]!=6u ||
       memcmp(data+3u,copy_source+64u,12u))stop(9,object,0x4b230u);
    const uint8_t *format=ff_sdk_shared_memory(literal,128u,FF_VM_READ,s);
    uint32_t length=0u;
    while(length<128u && format[length]) {
        /* No conversions admitted; original formatter owns vararg reads. */
        if(format[length]=='%' || format[length]>=128u)stop(9,literal+length,0x4b230u);
        ++length;
    }
    if(length!=52u)stop(9,literal,0x4b230u);
    (void)ff_sdk_shared_memory(0x2cc4f8u,length+1u,FF_VM_READ|FF_VM_WRITE,s);
    w[26]=length;w[27]=0u;w[28]=object;w[29]=data[256];w[30]=data[0];w[31]=actual_gate;
    w[32]=g_seh_ebp;w[34]=0x800u;w[35]=16384u;w[42]=15u;w[43]=1u;
    sdk_run_c_log_body(s,ret,literal,ambient,gate,w,0u);
    if(g_seh_ebp!=w[32] || g_ebp!=0x70038u || s->ebp!=0x70038u ||
       g_fs_base!=copy_words[75] || g_esp!=0x03c10c60u)stop(9,g_esp,0x4b230u);
    w[33]=g_seh_ebp;
}

void ff_sdk_shared_log_bridge(ff_sdk_scalar *s,uint32_t ret,uint32_t arg0,uint32_t arg1,uint32_t log_gate)
{
    if ((sdk_enabled<12u || sdk_enabled>45u) || log_bridge_words[0] || log_bridge_words[1] ||
        ret!=0x499d4u || arg0!=0x25b55cu)
        stop(9,s?s->esp:0u,0x4b230u);
    sdk_run_c_log_body(s,ret,arg0,arg1,log_gate,log_bridge_words,0u);
}
void ff_sdk_shared_log_bridge_second(ff_sdk_scalar *s,uint32_t ret,uint32_t arg0,uint32_t arg1,uint32_t log_gate)
{
    if (sdk_enabled<13u || second_log_bridge_words[0] || second_log_bridge_words[1] ||
        log_bridge_words[1]!=1u || getter_completed!=2u || getter_active ||
        ret!=0x49a68u || arg0!=0x25b4f8u)
        stop(9,s?s->esp:0u,0x4b230u);
    sdk_run_c_log_body(s,ret,arg0,arg1,log_gate,second_log_bridge_words,0u);
}
void ff_sdk_shared_log_bridge_third(ff_sdk_scalar *s,uint32_t ret,uint32_t arg0,uint32_t arg1)
{
    if (!s || (sdk_enabled!=15u && sdk_enabled!=16u && (sdk_enabled!=17u && (sdk_enabled!=18u && (sdk_enabled!=19u && (sdk_enabled!=20u && (sdk_enabled!=21u && (sdk_enabled!=22u && (sdk_enabled!=23u && (sdk_enabled!=24u && (sdk_enabled!=25u && (sdk_enabled!=26u && (sdk_enabled!=27u && (sdk_enabled!=28u && (sdk_enabled!=29u && (sdk_enabled!=30u && (sdk_enabled!=31u && (sdk_enabled!=32u && (sdk_enabled!=33u && (sdk_enabled!=34u && (sdk_enabled!=35u && (sdk_enabled!=36u && (sdk_enabled!=37u && (sdk_enabled!=38u && (sdk_enabled!=39u && (sdk_enabled!=40u && (sdk_enabled!=41u && (sdk_enabled!=42u && (sdk_enabled!=43u && (sdk_enabled!=44u && sdk_enabled!=45u))))))))))))))))))))))))))))) || !sdk_active || sdk_current_target!=0x48880u ||
        third_log_bridge_words[0] || third_log_bridge_words[1] ||
        log_bridge_words[1]!=1u || second_log_bridge_words[1]!=1u ||
        getter_completed!=2u || getter_active || tick_words[TICK_PRE_RET_VALIDATED]!=1u ||
        title_return_words[2]!=1u || parent_setup_words[0]!=1u || parent_setup_words[13]!=1u || parent_setup_words[32]!=1u ||
        device_stos[0]!=1u || device_stos[1]!=1u || ret!=0x49d9bu || arg0!=0x25b628u || arg1!=0u)
        stop(9,s?s->esp:0u,0x4b230u);
    third_log_bridge_words[0]=1u;
    third_log_memory_reads=0u;third_log_memory_stage=1u;
    const uint32_t counter=*(uint32_t *)ff_sdk_shared_memory(0x002cc280u,4u,FF_VM_READ,s);
    const uint32_t log_gate=*(uint32_t *)ff_sdk_shared_memory(0x002cccf8u,4u,FF_VM_READ,s);
    third_log_memory_stage=0u;
    third_log_bridge_words[26]=counter;third_log_bridge_words[27]=third_log_memory_reads;
    third_log_bridge_words[28]=third_log_memory_reads-1u;
    if (third_log_memory_reads!=2u || counter!=arg1 || counter!=0u || log_gate!=0u)
        stop(9,counter?0x002cc280u:0x002cccf8u,counter?counter:log_gate);
    sdk_run_c_log_body(s,ret,arg0,arg1,log_gate,third_log_bridge_words,1u);
}

/* Second reached zero-fill shape. Counters stay separate from the earlier
 * 115-DWORD fill so prior diagnostics retain their original meaning. */
void ff_sdk_shared_device_stos_event(uint32_t complete)
{
    ff_boot_thread *t=sdk_thread();
    if ((sdk_enabled!=10u && sdk_enabled!=11u && sdk_enabled!=12u && sdk_enabled!=13u && sdk_enabled!=14u && (sdk_enabled!=15u && sdk_enabled!=16u && (sdk_enabled!=17u && (sdk_enabled!=18u && (sdk_enabled!=19u && (sdk_enabled!=20u && (sdk_enabled!=21u && (sdk_enabled!=22u && (sdk_enabled!=23u && (sdk_enabled!=24u && (sdk_enabled!=25u && (sdk_enabled!=26u && (sdk_enabled!=27u && (sdk_enabled!=28u && (sdk_enabled!=29u && (sdk_enabled!=30u && (sdk_enabled!=31u && (sdk_enabled!=32u && (sdk_enabled!=33u && (sdk_enabled!=34u && (sdk_enabled!=35u && (sdk_enabled!=36u && (sdk_enabled!=37u && (sdk_enabled!=38u && (sdk_enabled!=39u && (sdk_enabled!=40u && (sdk_enabled!=41u && (sdk_enabled!=42u && (sdk_enabled!=43u && (sdk_enabled!=44u && sdk_enabled!=45u)))))))))))))))))))))))))))))) || sdk_current_target!=0x48880u || complete>1u ||
        (g_esp&3u) || g_esp<t->stack_low || g_esp>t->stack_top ||
        16u>t->stack_top-g_esp || ff_vm_check(&virtual_memory,g_esp,16u,FF_VM_READ) ||
        MEM32(g_esp+12u)!=0x49d80u || g_eax || g_df)
        stop(9,0x4999fu,complete);
    if (!complete) {
        if (device_stos[0] || device_stos[1] || g_ecx!=0x24u || g_edi!=0x2cc1f0u)
            stop(9,0x4999fu,0);
        ++device_stos[0];
    } else {
        if (device_stos[0]!=1u || device_stos[1] || g_ecx || g_edi!=0x2cc280u)
            stop(9,0x4999fu,1);
        ++device_stos[1];device_stos[2]=g_eax;device_stos[3]=g_ecx;device_stos[4]=g_edi;
    }
}

/* Opt-in function imports retain the legacy table and token indices exactly.
 * Two function tokens and two typed data imports remain under the 64 cap. */
static uint32_t sdk_irql_validate(void)
{
    const uint32_t slots[2]={0x2534d4u,0x25349cu},raw[2]={0x80000081u,0x800000a1u};
    if (!vm_ready || FF_KERNEL_BINDING_COUNT!=59u || FF_KERNEL_BINDING_COUNT+4u>64u) return FF_BOOT_INVALID;
    for (uint32_t i=0;i<2u;++i)
        if (ff_vm_check(&virtual_memory,slots[i],4u,FF_VM_READ|FF_VM_WRITE) ||
            MEM32(slots[i])!=(irql_imports?FF_KERNEL_BASE+(59u+i)*4u:raw[i])) return FF_BOOT_INVALID;
    return 0;
}
static uint32_t sdk_irql_profile(uint32_t enable)
{
    const uint32_t slots[2]={0x2534d4u,0x25349cu};
    uint32_t valid=sdk_irql_validate();if (valid) return valid;
    if (enable==irql_imports) return 0;
    for (uint32_t i=0;i<2u;++i) {
        if (enable) irql_raw[i]=MEM32(slots[i]);
        MEM32(slots[i])=enable?FF_KERNEL_BASE+(59u+i)*4u:irql_raw[i];
    }
    irql_imports=enable;return 0;
}

/* Same authoritative PCR byte as boot/DPC state. Only the reached getter's
 * 0 -> 2 -> 0 pair is modeled; fastcall ECX is not a stack argument. */
static void sdk_irql_kernel(uint32_t ordinal,ff_sdk_scalar *scalar)
{
    ff_boot_thread *t=sdk_thread();
    const uint32_t index=ordinal==129u?0u:1u;
    const uint32_t slot=index?0x25349cu:0x2534d4u,ret=index?0x24d39au:0x24d385u;
    const uint32_t expected_caller=getter_active_id==1u?0x499c7u:0x49a5bu;
    const uint32_t expected_type=getter_active_id==1u?0x24c2bcu:0x24c238u;
    if (sdk_enabled<11u || sdk_enabled>45u || sdk_current_target!=0x48880u || !irql_imports ||
        !getter_active || (getter_active_id!=1u && getter_active_id!=2u) ||
        (getter_active_id==2u && (sdk_enabled<13u || log_bridge_words[1]!=1u || getter_completed!=1u)) ||
        !host_words[3] || (g_esp&3u) || g_esp<t->stack_low || g_esp>t->stack_top ||
        16u>t->stack_top-g_esp || ff_vm_check(&virtual_memory,g_esp,16u,FF_VM_READ) ||
        ff_vm_check(&virtual_memory,slot,4u,FF_VM_READ)) stop(9,g_esp,ordinal);
    const uint32_t *frame=(const uint32_t *)((sdk_enabled>=13u)?
        ff_sdk_shared_memory(g_esp,16u,FF_VM_READ,scalar):(guest_ram+g_esp));
    if (frame[0]!=ret || frame[2]!=expected_caller || frame[3]!=expected_type ||
        *(uint32_t *)ff_sdk_shared_memory(slot,4u,FF_VM_READ,scalar)!=FF_KERNEL_BASE+(59u+index)*4u) stop(9,g_esp,ordinal);
    uint32_t *row=kernel_record(ordinal,0u);
    (void)ff_sdk_shared_memory(t->fs+0x24u,1u,FF_VM_READ,scalar);
    (void)ff_sdk_shared_memory(t->fs+0x24u,1u,FF_VM_WRITE,scalar);
    uint8_t old_level=0xffu;
    uint32_t status;
    if (!index) {
        if (irql_pair.active || irql_pair.raise_count!=getter_active_id-1u ||
            irql_pair.lower_count!=getter_active_id-1u) stop(9,g_esp,ordinal);
        if (getter_active_id==2u) second_query_words[3]=g_esp;
        status=ff_irql_pair_raise_to_dpc(&irql_pair,&boot,&virtual_memory,&old_level);
        if (status) stop(9,t->fs+0x24u,status);
        if (old_level!=0u || irql_pair.raise_count!=getter_active_id || irql_pair.lower_count!=getter_active_id-1u)
            stop(9,t->fs+0x24u,ordinal);
        g_eax=old_level;
        if (getter_active_id==1u) {irql_words[2]=old_level;irql_words[3]=guest_ram[t->fs+0x24u];}
        else second_query_words[4]=old_level;
    } else {
        /* The generated fastcall caller writes the old level into CL only.
         * Keep full ECX in context/telemetry, but pass the source-width byte. */
        const uint32_t lower_level=(uint8_t)g_ecx;
        if (!irql_pair.active || irql_pair.raise_count!=getter_active_id ||
            irql_pair.lower_count!=getter_active_id-1u || lower_level!=0u) stop(9,g_esp,ordinal);
        if (getter_active_id==2u) {second_query_words[5]=g_esp;second_query_words[6]=g_ecx;}
        else irql_words[4]=g_ecx;
        status=ff_irql_pair_lower_saved(&irql_pair,&boot,&virtual_memory,lower_level);
        if (status) stop(9,t->fs+0x24u,status);
        if (irql_pair.active || irql_pair.raise_count!=getter_active_id || irql_pair.lower_count!=getter_active_id)
            stop(9,t->fs+0x24u,ordinal);
        g_eax=0u;
        if (getter_active_id==1u) irql_words[5]=guest_ram[t->fs+0x24u];
    }
    row[1]=g_eax;g_esp+=4u;
    ff_sdk_shared_read(scalar);
}
