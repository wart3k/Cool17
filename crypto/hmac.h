//
// Created by WART3K on 28.01.25.
//

#ifndef HMAC_H
#define HMAC_H

#include <stdint.h>
#include <stddef.h>

#include "sha256.h"

void hmac_sha256(const uint8_t *key, size_t key_len, const uint8_t *data, size_t data_len, uint8_t hash[SHA256_HASH_SIZE]);

#endif //HMAC_H
