//
// Created by WART3K on 28.01.25.
//

#include "sha256.h"

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

// SHA-256 Constants (first 32 bits of the fractional parts of the square roots of prime numbers 2–19)
static const uint32_t K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

// Initial Hash Values (first 32 bits of the square roots of prime numbers 2–19)
static const uint32_t H_INIT[8] = {
    0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
    0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

// Rotate and Shift Operations
#define ROTR(x, n) ((x >> n) | (x << (32 - n)))
#define SHR(x, n)  (x >> n)

// SHA-256 Specific Functions
#define CH(x, y, z)  ((x & y) ^ (~x & z))
#define MAJ(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define SIGMA0(x)    (ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define SIGMA1(x)    (ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25))
#define sigma0(x)    (ROTR(x, 7) ^ ROTR(x, 18) ^ SHR(x, 3))
#define sigma1(x)    (ROTR(x, 17) ^ ROTR(x, 19) ^ SHR(x, 10))

// SHA-256 Padding and Processing
void sha256_transform(uint32_t state[8], const uint8_t block[64]) {
    uint32_t W[64];

    // 1. Load block into W (first 16 words)
    for (uint8_t i = 0; i < 16; i++) {
        W[i] = (block[i * 4] << 24) | (block[i * 4 + 1] << 16) |
               (block[i * 4 + 2] << 8) | block[i * 4 + 3];
    }
    // 2. Calculate remaining words
    for (uint8_t i = 16; i < 64; i++) {
        W[i] = sigma1(W[i - 2]) + W[i - 7] + sigma0(W[i - 15]) + W[i - 16];
    }

    // 3. Load initial hash values into a-h
    uint32_t a = state[0];
    uint32_t b = state[1];
    uint32_t c = state[2];
    uint32_t d = state[3];
    uint32_t e = state[4];
    uint32_t f = state[5];
    uint32_t g = state[6];
    uint32_t h = state[7];

    // 4. Main loop
    for (uint8_t i = 0; i < 64; i++) {
        uint32_t T1 = h + SIGMA1(e) + CH(e, f, g) + K[i] + W[i];
        uint32_t T2 = SIGMA0(a) + MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + T1;
        d = c;
        c = b;
        b = a;
        a = T1 + T2;
    }

    // 5. Add result to hash value
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
    state[5] += f;
    state[6] += g;
    state[7] += h;
}

// SHA-256 Padding and Finalization
void sha256(const uint8_t *data, size_t len, uint8_t hash[32]) {
    uint32_t state[8];
    uint8_t block[64];
    uint64_t bitlen = len * 8;

    // Initialize state
    memcpy(state, H_INIT, sizeof(H_INIT));

    // Process full blocks
    while (len >= 64) {
        memcpy(block, data, 64);
        sha256_transform(state, block);
        data += 64;
        len -= 64;
    }

    // Handle remaining bytes with padding
    memset(block, 0, sizeof(block));
    memcpy(block, data, len);
    block[len] = 0x80;
    if (len >= 56) {
        sha256_transform(state, block);
        memset(block, 0, 64);
    }

    // Append length
    for (uint8_t i = 0; i < 8; i++) {
        block[63 - i] = bitlen & 0xFF;
        bitlen >>= 8;
    }
    sha256_transform(state, block);

    // Write final hash value to result
    for (uint8_t i = 0; i < 8; i++) {
        hash[i * 4]     = (state[i] >> 24) & 0xFF;
        hash[i * 4 + 1] = (state[i] >> 16) & 0xFF;
        hash[i * 4 + 2] = (state[i] >> 8) & 0xFF;
        hash[i * 4 + 3] = state[i] & 0xFF;
    }
}

#ifdef __cplusplus
}
#endif

