//
// Created by Thomas Woltering on 28.01.25.
//

#include "hmac.h"

#include <string.h>

#include "sha256.h"

void hmac_sha256(const uint8_t *key, size_t key_len, const uint8_t *message, size_t message_len, uint8_t *output) {
    uint8_t key_ipad[SHA256_BLOCK_SIZE];
    uint8_t key_opad[SHA256_BLOCK_SIZE];
    uint8_t inner_hash[SHA256_HASH_SIZE];

    if (key_len > SHA256_BLOCK_SIZE) {
        sha256(key, key_len, key_ipad); // Hash the key if it's longer than block size
        key_len = SHA256_HASH_SIZE;
    } else {
        memset(key_ipad, 0, SHA256_BLOCK_SIZE);
        memcpy(key_ipad, key, key_len);
    }

    // Outer padding
    for (size_t i = 0; i < SHA256_BLOCK_SIZE; i++) {
        key_opad[i] = key_ipad[i] ^ 0x5c;
    }

    // Inner padding
    for (size_t i = 0; i < SHA256_BLOCK_SIZE; i++) {
        key_ipad[i] ^= 0x36;
    }

    // Inner hash: sha256(key_ipad || message)
    uint8_t inner_input[SHA256_BLOCK_SIZE + message_len];
    memcpy(inner_input, key_ipad, SHA256_BLOCK_SIZE);
    memcpy(inner_input + SHA256_BLOCK_SIZE, message, message_len);
    sha256(inner_input, sizeof(inner_input), inner_hash);

    // Outer hash: sha256(key_opad || inner_hash)
    uint8_t outer_input[SHA256_BLOCK_SIZE + SHA256_HASH_SIZE];
    memcpy(outer_input, key_opad, SHA256_BLOCK_SIZE);
    memcpy(outer_input + SHA256_BLOCK_SIZE, inner_hash, SHA256_HASH_SIZE);
    sha256(outer_input, sizeof(outer_input), output);
}

