set(COOL_CRYPTO_SOURCES
        "aes128.c"
        "sha256.c"
        "hmac.c"
)

set(COOL_CRYPTO_HEADERS
        "aes128.h"
        "sha256.h"
        "hmac.h"
)

add_library(${COOL_CRYPTO_LIB} STATIC
        ${COOL_CRYPTO_SOURCES}
        ${COOL_CRYPTO_HEADERS}
)

target_include_directories(${COOL_CRYPTO_LIB} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}")