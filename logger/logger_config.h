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
 * @brief default log level
 *
 * Set the default level. All lower log levels will be logged out
 */
#define DEFAULT_LOG_LEVEL   INFO

/**
 * @brief Log Level Buffer
 *
 * Set the maximum number of the print buffer
 */
#define LOG_PRINT_BUFFER_LEN    UINT16_MAX

/**
 * @brief Log with printf
 *
 * use printf for the output log
 */
#define LOG_WITH_PRINTF

#ifdef __cplusplus
}
#endif

#endif //LOGGER_CONFIG_H
