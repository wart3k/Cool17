//
// Created by WART3K on 04.02.25.
//

#ifndef LOGGER_CONFIG_H
#define LOGGER_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief Default log level depending on Build Defines.
 *
 * Set the default level. All lower log levels will be logged out.
 */
#define BUILD_DEPENDING_LEVELS

#if defined (BUILD_DEPENDING_LEVELS)

#if defined(RELEASE)
#define DEFAULT_LOG_LEVEL   LOG_LEVEL_INFO
#elif defined(DEBUG) || defined(TEST) // #if defined(RELEASE)
#define DEFAULT_LOG_LEVEL   LOG_LEVEL_TRACE
#endif // DEBUG || TEST

#else // defined (BUILD_DEPENDING_LEVELS)

#define DEFAULT_LOG_LEVEL   LOG_LEVEL_TRACE

#endif // defined (BUILD_DEPENDING_LEVELS)
/**
 * @brief Log Level Buffer.
 *
 * Set the maximum number of the print buffer.
 */
#define LOG_PRINT_BUFFER_LEN    UINT16_MAX

/**
 * @brief Log output method selection.
 *
 * Choose one of the following output methods:
 * - LOG_WITH_PRINTF: Use standard printf functions (higher memory usage)
 * - LOG_WITH_COOL_PRINT: Use custom lightweight print implementation (lower memory usage)
 */
// Uncomment ONE of the following lines:
// #define LOG_WITH_PRINTF
#define LOG_WITH_COOL_PRINT


#ifdef __cplusplus
}
#endif

#endif //LOGGER_CONFIG_H
