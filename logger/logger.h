//
// Created by WART3K on 19.10.24.
//

#ifndef COOL17_LOGGER_H
#define COOL17_LOGGER_H

#include <stdarg.h>

#include "../status.h"

/**
 * @brief Enum for log levels.
 *
 * Defines the different log levels available for logging.
 */
typedef enum {
    ERROR = 0,     /**< Error level */
    CRITICAL,      /**< Critical level */
    WARNING,       /**< Warning level */
    INFO,          /**< Information level */
    DEBUG,         /**< Debug level */
    TRACE          /**< Trace level */
}log_level_list_t;

/**
 * @brief Initializes the logger with a specific log level.
 *
 * @param log_level The initial log level.
 * @return COOL_OK on success, otherwise an error status.
 */
cool_status_t logger_init(log_level_list_t log_level);

/**
 * @brief Sets the log level for the logger.
 *
 * @param log_level The log level to set.
 * @return COOL_OK on success, otherwise an error status.
 */
cool_status_t logger_set_level(log_level_list_t log_level);

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
 * @return COOL_OK on success, otherwise an error status.
 */
cool_status_t log_error(const char * restrict func, const char * restrict msg, ...);

/**
 * @brief Logs a critical message with optional arguments.
 *
 * @param func The function name where the log is triggered.
 * @param msg The message to log.
 * @param ... Additional arguments for the message format.
 * @return COOL_OK on success, otherwise an error status.
 */
cool_status_t log_critical(const char * restrict func, const char * restrict msg, ...);

/**
 * @brief Logs a warning message with optional arguments.
 *
 * @param func The function name where the log is triggered.
 * @param msg The message to log.
 * @param ... Additional arguments for the message format.
 * @return COOL_OK on success, otherwise an error status.
 */
cool_status_t log_warning(const char * restrict func, const char * restrict msg, ...);

/**
 * @brief Logs an informational message with optional arguments.
 *
 * @param func The function name where the log is triggered.
 * @param msg The message to log.
 * @param ... Additional arguments for the message format.
 * @return COOL_OK on success, otherwise an error status.
 */
cool_status_t log_info(const char * restrict func, const char * restrict msg, ...);

/**
 * @brief Logs a debug message with optional arguments.
 *
 * @param func The function name where the log is triggered.
 * @param msg The message to log.
 * @param ... Additional arguments for the message format.
 * @return COOL_OK on success, otherwise an error status.
 */
cool_status_t log_debug(const char * restrict func, const char * restrict msg, ...);

/**
 * @brief Logs a trace message with optional arguments.
 *
 * @param func The function name where the log is triggered.
 * @param msg The message to log.
 * @param ... Additional arguments for the message format.
 * @return COOL_OK on success, otherwise an error status.
 */
cool_status_t log_trace(const char * restrict func, const char * restrict msg, ...);

/**
 * @brief Macro for logging an error message.
 *
 * This macro simplifies calling the `log_error` function.
 */
#define CLOGE(msg, ...)     log_error(__func__, msg, ##__VA_ARGS__)

/**
 * @brief Macro for logging a critical message.
 *
 * This macro simplifies calling the `log_critical` function.
 */
#define CLOGC(msg, ...)     log_critical(__func__, msg, ##__VA_ARGS__)

/**
 * @brief Macro for logging a warning message.
 *
 * This macro simplifies calling the `log_warning` function.
 */
#define CLOGW(msg, ...)     log_warning(__func__, msg, ##__VA_ARGS__)

/**
 * @brief Macro for logging an informational message.
 *
 * This macro simplifies calling the `log_info` function.
 */
#define CLOGI(msg, ...)     log_info(__func__, msg, ##__VA_ARGS__)

/**
 * @brief Macro for logging a debug message.
 *
 * This macro simplifies calling the `log_debug` function.
 */
#define CLOGD(msg, ...)     log_debug(__func__, msg, ##__VA_ARGS__)

/**
 * @brief Macro for logging a trace message.
 *
 * This macro simplifies calling the `log_trace` function.
 */
#define CLOGT(msg, ...)     log_trace(__func__, msg, ##__VA_ARGS__)

#endif //COOL17_LOGGER_H
