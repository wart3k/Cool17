set(COOL_LOGGER_SOURCES
        "logger.c"
)

set(COOL_LOGGER_HEADERS
        "logger.h"
        "logger_config.h"
        "logger_config_check.h"
)

add_library(${COOL_LOGGER_LIB} STATIC
        ${COOL_LOGGER_SOURCES}
        ${COOL_LOGGER_HEADERS}
)

target_include_directories(${COOL_LOGGER_LIB} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
)
