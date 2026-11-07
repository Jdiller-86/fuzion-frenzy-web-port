#include "../runtime/core/kernel_capture.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void canonical_stream(void)
{
    ff_kernel_capture capture;
    assert(ff_kernel_capture_init(&capture,0)==FF_CAPTURE_OK);
    uint32_t first[]={0x12345678u};
    assert(ff_kernel_capture_begin(&capture,277,first,1)==FF_CAPTURE_OK);
    first[0]=0; /* Original arguments must already be hashed. */
    ff_kernel_capture_finish(&capture,0);
    const uint32_t second[]={0x70008,0x100,0x200,56,34};
    assert(ff_kernel_capture_begin(&capture,211,second,5)==FF_CAPTURE_OK);
    ff_kernel_capture_finish(&capture,0xc00000bbu);
    uint8_t digest[32],again[32];
    char hex[65];
    ff_sha256_final(&capture.digest,digest);
    for(unsigned i=0;i<32;++i)snprintf(hex+i*2u,3,"%02x",digest[i]);
    assert(!strcmp(hex,"10c0e69682533ea477605c91213a605036c6afd027e321f9173720efc116f3e0"));
    ff_kernel_capture_finish(&capture,0);
    ff_sha256_final(&capture.digest,again);
    assert(!memcmp(digest,again,sizeof digest) && capture.total==2 && capture.completed==1);
}
static void fixed_windows(void)
{
    ff_kernel_capture full,window,saved;
    assert(!ff_kernel_capture_init(&full,0) && !ff_kernel_capture_init(&window,512));
    for(uint32_t i=0;i<512;++i) {
        assert(ff_kernel_capture_begin(&full,277,&i,1)==FF_CAPTURE_OK);
        assert(ff_kernel_capture_begin(&window,277,&i,1)==FF_CAPTURE_OK);
        const uint32_t status=i==4u?0xc000000du:0;
        ff_kernel_capture_finish(&full,status);
        ff_kernel_capture_finish(&window,status);
    }
    assert(window.captured_start==0 && window.total==512 && window.completed==511);
    saved=full;
    assert(ff_kernel_capture_begin(&full,277,NULL,0)==FF_CAPTURE_FULL && !memcmp(&saved,&full,sizeof full));
    uint8_t prefix[32];
    ff_sha256_final(&full.digest,prefix);
    assert(ff_kernel_capture_begin(&window,294,NULL,0)==FF_CAPTURE_ROTATE);
    assert(window.captured_start==512 && window.prefix_completed==511 &&
           !memcmp(prefix,window.prefix_digest,sizeof prefix));
    saved=window;
    assert(ff_kernel_capture_begin(&window,277,NULL,0)==FF_CAPTURE_INVALID &&
           !memcmp(&saved,&window,sizeof window));
    ff_kernel_capture_finish(&window,0);
    for(uint32_t i=513;i<1024;++i) {
        assert(ff_kernel_capture_begin(&window,277,&i,1)==FF_CAPTURE_OK);
        ff_kernel_capture_finish(&window,0);
    }
    saved=window;
    assert(window.total==1024 && window.completed==1023 &&
           ff_kernel_capture_begin(&window,277,NULL,0)==FF_CAPTURE_FULL &&
           !memcmp(&saved,&window,sizeof window));
    assert(ff_kernel_capture_init(&window,513)==FF_CAPTURE_INVALID &&
           !memcmp(&saved,&window,sizeof window));
}
int main(void)
{
    canonical_stream();
    fixed_windows();
    puts("canonical SHA stream, idempotent completion, exact windows and hard ceilings passed");
    return 0;
}
