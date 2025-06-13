//
// Created by WART3K on 19.10.24.
//

#ifndef COOL17_LOGGER_H
#define COOL17_LOGGER_H

#include <stddef.h>
#include <stdarg.h>

#include "logger_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Logger status and error codes
 *
 * Return codes used by all logger functions to indicate success or failure.
 * These codes help with debugging and error handling in the logging system.
 */
typedef enum {
    LOGGER_STATUS_OK = 0,               /**< Operation completed successfully */
    LOGGER_NULL_POINTER_ERROR,          /**< NULL pointer passed as parameter */
    LOGGER_OVERFLOW,                    /**< Buffer overflow occurred during formatting */
    LOGGER_FORMAT_ERROR,                /**< Error in format string or data formatting */
    LOGGER_PRINT_FAILED,                /**< Output operation failed */
    LOGGER_WRONG_INPUT_PARAMETER        /**< Invalid input parameter provided */
} logger_status_t;

/**
 * @brief Enum for log levels.
 *
 * Defines the different log levels available for logging.
 */
typedef enum {
    LOG_LEVEL_ERROR = 0,      /**< Error level */
    LOG_LEVEL_CRITICAL,       /**< Critical level */
    LOG_LEVEL_WARNING,        /**< Warning level */
    LOG_LEVEL_INFO,           /**< Information level */
    LOG_LEVEL_DEBUG,          /**< Debug level */
    LOG_LEVEL_TRACE           /**< Trace level */
}log_level_list_t;

/**
 * @brief enum for format types
 *
 * to specify the output format for CLOG_ARRAY
 */
typedef enum {
    U8_DEC,   /**< Unsigned 8-bit decimal */
    S8_DEC,   /**<  Signed 8-bit decimal */
    U8_HEX,   /**<  Unsigned 8-bit hex */
    S8_BIN,   /**<  Signed 8-bit binary */
    U16_DEC,  /**<  Unsigned 16-bit decimal */
    S16_DEC,  /**<  Signed 16-bit decimal */
    U16_HEX,  /**<  Unsigned 16-bit hex */
    S16_BIN,  /**<  Signed 16-bit binary */
    U32_DEC,  /**<  Unsigned 32-bit decimal */
    S32_DEC,  /**<  Signed 32-bit decimal */
    U32_HEX,  /**<  Unsigned 32-bit hex */
    S32_BIN,  /**<  Signed 32-bit binary */
    U64_DEC,  /**<  Unsigned 64-bit decimal */
    S64_DEC,  /**<  Signed 64-bit decimal */
    U64_HEX,  /**<  Unsigned 64-bit hex */
    S64_BIN,  /**<  Signed 64-bit binary */
    FLOAT,    /**<  Float */
    DOUBLE    /**<  Double */
} log_format_t;

/**
 * @brief Sets the log level for the logger.
 *
 * @param log_level The log level to set.
 * @return LOGGER_STATUS_OK on success, otherwise an error status.
 */
logger_status_t logger_set_level(log_level_list_t log_level);

/**
 * @brief Retrieves the current log level.
 *
 * @return The current log level.
 */
log_level_list_t logger_get_level(void);

/**
 * @brief Logs an error message with optional arguments.
 *
 * @param func The function name where the log is triggered.
 * @param msg The message to log.
 * @param ... Additional arguments for the message format.
 * @return LOGGER_STATUS_OK on success, otherwise an error status.
 */
logger_status_t log_error(const char * restrict func, const char * restrict msg, ...);

/**
 * @brief Logs a critical message with optional arguments.
 *
 * @param func The function name where the log is triggered.
 * @param msg The message to log.
 * @param ... Additional arguments for the message format.
 * @return LOGGER_STATUS_OK on success, otherwise an error status.
 */
logger_status_t log_critical(const char * restrict func, const char * restrict msg, ...);

/**
 * @brief Logs a warning message with optional arguments.
 *
 * @param func The function name where the log is triggered.
 * @param msg The message to log.
 * @param ... Additional arguments for the message format.
 * @return LOGGER_STATUS_OK on success, otherwise an error status.
 */
logger_status_t log_warning(const char * restrict func, const char * restrict msg, ...);

/**
 * @brief Logs an informational message with optional arguments.
 *
 * @param func The function name where the log is triggered.
 * @param msg The message to log.
 * @param ... Additional arguments for the message format.
 * @return LOGGER_STATUS_OK on success, otherwise an error status.
 */
logger_status_t log_info(const char * restrict func, const char * restrict msg, ...);

/**
 * @brief Logs a debug message with optional arguments.
 *
 * @param func The function name where the log is triggered.
 * @param msg The message to log.
 * @param ... Additional arguments for the message format.
 * @return LOGGER_STATUS_OK on success, otherwise an error status.
 */
logger_status_t log_debug(const char * restrict func, const char * restrict msg, ...);

/**
 * @brief Logs a trace message with optional arguments.
 *
 * @param func The function name where the log is triggered.
 * @param msg The message to log.
 * @param ... Additional arguments for the message format.
 * @return LOGGER_STATUS_OK on success, otherwise an error status.
 */
logger_status_t log_trace(const char * restrict func, const char * restrict msg, ...);

/**
 * @brief Logs an array with a specified format and log level.
 *
 * This function logs the contents of an array with the given format and log level.
 *
 * @param func the function name where the log is triggered.
 * @param level The log level for the message (e.g., TRACE, DEBUG, etc.).
 * @param format The format specifier for each array element (e.g., "%d" for integers).
 * @param array Pointer to the array to log.
 * @param array_size The size of the array.
 * @return LOGGER_STATUS_OK on success, otherwise an error status.
 */
logger_status_t log_array(const char * restrict func,
                        log_level_list_t level,
                        log_format_t format,
                        const void *array,
                        size_t array_size);

/**
 * @brief Macro for logging an error message.
 *
 * This macro simplifies calling the `log_error` function.
 */
#define CLOGE(msg, ...)     log_error(LOG_FUNC_NAME, msg, ##__VA_ARGS__)

/**
 * @brief Macro for logging a critical message.
 *
 * This macro simplifies calling the `log_critical` function.
 */
#define CLOGC(msg, ...)     log_critical(LOG_FUNC_NAME, msg, ##__VA_ARGS__)

/**
 * @brief Macro for logging a warning message.
 *
 * This macro simplifies calling the `log_warning` function.
 */
#define CLOGW(msg, ...)     log_warning(LOG_FUNC_NAME, msg, ##__VA_ARGS__)

/**
 * @brief Macro for logging an informational message.
 *
 * This macro simplifies calling the `log_info` function.
 */
#define CLOGI(msg, ...)     log_info(LOG_FUNC_NAME, msg, ##__VA_ARGS__)

/**
 * @brief Macro for logging a debug message.
 *
 * This macro simplifies calling the `log_debug` function.
 */
#define CLOGD(msg, ...)     log_debug(LOG_FUNC_NAME, msg, ##__VA_ARGS__)

/**
 * @brief Macro for logging a trace message.
 *
 * This macro simplifies calling the `log_trace` function.
 */
#define CLOGT(msg, ...)     log_trace(LOG_FUNC_NAME, msg, ##__VA_ARGS__)

/**
 * @brief Logs an array with a specified format and log level.
 *
 * This macro logs an array using the log_array function with the given format.
 *
 * @param level The log level for the message (e.g., TRACE, DEBUG, etc.).
 * @param format The format specifier for each array element (e.g., "%d" for integers).
 * @param array Pointer to the array to log.
 */
#define CLOG_ARRAY(level, format, array, array_size) \
    log_array(LOG_FUNC_NAME, level, format, array, array_size)

#ifdef __cplusplus
}
#endif

#endif //COOL17_LOGGER_H
