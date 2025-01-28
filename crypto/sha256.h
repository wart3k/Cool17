//
// Created by WART3K on 28.01.25.
//

#ifndef SHA256_H
#define SHA256_H

#include <stdint.h>
#include <stddef.h>

#define SHA256_HASH_SIZE    32

#ifdef __cplusplus
extern "C" {
#endif

    void sha256(const uint8_t *data, size_t len, uint8_t hash[SHA256_HASH_SIZE]);

#ifdef __cplusplus
}
#endif

#endif //SHA256_H
