#ifndef FF_SHA256_H
#define FF_SHA256_H

#include <stdint.h>

/* Incremental SHA-256 state.  The caller owns each context; the helper uses no
 * heap or global state.  Update may be called repeatedly with bounded chunks.
 */
typedef struct ff_sha256 {
    uint32_t state[8];
    uint64_t total_bytes;
    uint8_t block[64];
    uint32_t block_bytes;
} ff_sha256;

void ff_sha256_init(ff_sha256 *context);
void ff_sha256_update(ff_sha256 *context, const uint8_t *bytes, uint32_t count);

/* Finalizes a copy, leaving context unchanged.  Repeated final calls are
 * stable, and a later update continues the original unfinalized stream.
 */
void ff_sha256_final(const ff_sha256 *context, uint8_t output[32]);

#endif
