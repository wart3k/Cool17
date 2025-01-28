//
// Created by WART3K on 28.01.25.
//

#ifndef SHA256_H
#define SHA256_H

#include <stdint.h>
#include <stddef.h>

#define SHA256_HASH_SIZE    32
#define SHA256_BLOCK_SIZE   64

#ifdef __cplusplus
extern "C" {
#endif

    // SHA-256 State structure for init, update, and final
    typedef struct {
        uint32_t state[8];
        uint64_t bitlen;
        uint8_t data[64];
        size_t datalen;
    } sha256_ctx;

    void sha256(const char *data, size_t len, uint8_t hash[SHA256_HASH_SIZE]);

    void sha256_init(sha256_ctx *ctx);
    void sha256_update(sha256_ctx *ctx, const uint8_t *data, size_t len);
    void sha256_final(sha256_ctx *ctx, uint8_t hash[SHA256_HASH_SIZE]);

#ifdef __cplusplus
}
#endif

#endif //SHA256_H
