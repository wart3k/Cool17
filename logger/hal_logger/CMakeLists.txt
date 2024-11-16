set(HAL_LOGGER_SOURCES
        "hal_logger.c"
        "hal_logger_desktop.c"
)

set(HAL_LOGGER_HEADERS
        "hal_logger.h"
        "hal_logger_desktop.h"
)

add_library(${COOL_HAL_LOGGER_LIB} STATIC
        ${HAL_LOGGER_SOURCES}
        ${HAL_LOGGER_HEADERS}
)

target_include_directories(${COOL_HAL_LOGGER_LIB} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
)