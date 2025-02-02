cmake_minimum_required(VERSION 3.24)

project(
        cool17
        VERSION 0.0.1
        LANGUAGES C CXX
)

set(CMAKE_C_STANDARD            17)
set(CMAKE_C_STANDARD_REQUIRED   ON)
set(CMAKE_C_EXTENSIONS          OFF)

set(COOL_LOGGER_LIB cool_logger_lib)
set(COOL_CRYPTO_LIB cool_crypto_lib)

include_directories("${CMAKE_SOURCE_DIR}")

add_subdirectory(logger)
add_subdirectory(crypto)