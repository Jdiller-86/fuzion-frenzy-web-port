/* Source-owned cross-library ABI check; contains no original game content. */
#include <stdio.h>
#include <string.h>
#include <zlib.h>

int main(void)
{
    const unsigned char vector[] = "123456789";
    unsigned char compressed[64], restored[64];
    uLongf compressed_size = sizeof(compressed), restored_size = sizeof(restored);
    if (strcmp(zlibVersion(), "1.3.1") != 0) return 1;
    if (crc32(0, vector, 9) != 0xcbf43926UL) return 2;
    if (compress2(compressed, &compressed_size, vector, 9, 6) != Z_OK) return 3;
    if (uncompress(restored, &restored_size, compressed, compressed_size) != Z_OK) return 4;
    if (restored_size != 9 || memcmp(restored, vector, 9) != 0) return 5;
    puts("Wasm zlib 1.3.1: CRC golden vector and compression roundtrip passed");
    return 0;
}
