# Logger-Quellen und Header
set(COOL_LOGGER_SOURCES
        "logger.c"
        "hal_logger/hal_logger.c"
        "hal_logger/hal_logger_desktop.c"
)

set(COOL_LOGGER_HEADERS
        "logger.h"
        "hal_logger/hal_logger.h"
        "hal_logger/hal_logger_desktop.h"
)

# Logger-Bibliothek erstellen
add_library(${COOL_LOGGER_LIB} STATIC
        ${COOL_LOGGER_SOURCES}
        ${COOL_LOGGER_HEADERS}
)

# Include-Verzeichnisse für die Bibliothek festlegen
target_include_directories(${COOL_LOGGER_LIB} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/hal_logger"
)
