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

/**
 * @brief Default log level configuration based on build type
 *
 * Automatic log level selection depending on build defines:
 *
 * BUILD_DEPENDING_LEVELS enabled:
 *   - RELEASE builds:    LOG_LEVEL_INFO (3) - Production optimized, less verbose
 *   - DEBUG/TEST builds: LOG_LEVEL_TRACE (5) - Full debugging, all messages
 *
 * BUILD_DEPENDING_LEVELS disabled:
 *   - Manual control:    LOG_LEVEL_TRACE (5) - Safe default, runtime configurable
 *
 * Log levels: 0=ERROR, 1=CRITICAL, 2=WARNING, 3=INFO, 4=DEBUG, 5=TRACE
 * All levels <= DEFAULT_LOG_LEVEL will be output.
 */
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
 * @brief Include function names in log output
 *
 * Disable this to save Flash memory in embedded systems
 * Each function name costs ~5-30 bytes of Flash memory
 */
#define LOG_INCLUDE_FUNCTION_NAMES

/**
 * @brief Function name macro for log messages
 *
 * This macro resolves to either __func__ (current function name) or an empty string,
 * depending on the LOG_INCLUDE_FUNCTION_NAMES configuration setting.
 *
 * When LOG_INCLUDE_FUNCTION_NAMES is defined:
 *   - LOG_FUNC_NAME expands to __func__ (actual function name)
 *   - Memory usage: ~5-30 bytes Flash per function that uses logging
 *
 * When LOG_INCLUDE_FUNCTION_NAMES is NOT defined:
 *   - LOG_FUNC_NAME expands to "" (empty string)
 *   - Memory usage: minimal (only 1 byte for null terminator)
 *
 * @note This is used internally by the CLOG macros
 * @see LOG_INCLUDE_FUNCTION_NAMES in logger_config.h
 */
#ifdef LOG_INCLUDE_FUNCTION_NAMES
#define LOG_FUNC_NAME   __func__
#else
#define LOG_FUNC_NAME   ""
#endif // LOG_INCLUDE_FUNCTION_NAMES

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
