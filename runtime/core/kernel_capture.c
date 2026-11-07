#include "kernel_capture.h"
#include <string.h>

static void hash_word(ff_sha256 *digest,uint32_t value)
{
    const uint8_t bytes[4]={(uint8_t)value,(uint8_t)(value>>8),(uint8_t)(value>>16),(uint8_t)(value>>24)};
    ff_sha256_update(digest,bytes,sizeof bytes);
}
uint32_t ff_kernel_capture_init(ff_kernel_capture *capture,uint32_t start)
{
    if(!capture || (start!=0 && start!=FF_CAPTURE_MAX_START))return FF_CAPTURE_INVALID;
    memset(capture,0,sizeof *capture);
    capture->requested_start=start;
    ff_sha256_init(&capture->digest);
    static const uint8_t domain[]="ff-kernel-capture-v1";
    ff_sha256_update(&capture->digest,domain,sizeof domain);
    return FF_CAPTURE_OK;
}
uint32_t ff_kernel_capture_begin(ff_kernel_capture *capture,uint32_t ordinal,const uint32_t *args,uint32_t argc)
{
    if(!capture || capture->pending || argc>10u || (argc && !args))return FF_CAPTURE_INVALID;
    if(capture->total>=capture->requested_start+FF_CAPTURE_ROWS)return FF_CAPTURE_FULL;
    uint32_t result=FF_CAPTURE_OK;
    if(capture->total==capture->requested_start && capture->requested_start && !capture->captured_start) {
        ff_sha256_final(&capture->digest,capture->prefix_digest);
        capture->prefix_completed=capture->completed;
        capture->captured_start=capture->requested_start;
        result=FF_CAPTURE_ROTATE;
    }
    hash_word(&capture->digest,ordinal);
    hash_word(&capture->digest,argc);
    for(uint32_t i=0;i<argc;++i)hash_word(&capture->digest,args[i]);
    ++capture->total;
    capture->pending=1;
    return result;
}
void ff_kernel_capture_finish(ff_kernel_capture *capture,uint32_t status)
{
    if(!capture || !capture->pending)return;
    hash_word(&capture->digest,status);
    if(!status)++capture->completed;
    capture->pending=0;
}
