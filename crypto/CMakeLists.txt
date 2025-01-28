set(COOL_CRYPTO_SOURCES
    "sha256.c"
)

set(COOL_CRYPTO_HEADERS
        "sha256.h"
)

add_library(${COOL_CRYPTO_LIB} STATIC
        ${COOL_CRYPTO_SOURCES}
        ${COOL_CRYPTO_HEADERS}
)

target_include_directories(${COOL_CRYPTO_LIB} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}")