//
// Created by WART3K on 02.02.25.
//

#ifndef AES128_H
#define AES128_H

#include <stdint.h>

#define kNumKeyWords  4
#define kNumColumns  4
#define kNumRounds  10
#define kExpandedKeyWords  (kNumColumns * (kNumRounds + 1))

void key_expansion(const uint8_t key[16], uint32_t w[kExpandedKeyWords]);
void aes_encrypt_block(uint8_t in[16], uint8_t out[16], const uint32_t round_keys[kExpandedKeyWords]);

#endif //AES128_H
