//
// Created by WART3K on 19.10.24.
//

#include "logger.h"

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define LOG_LEVEL_COUNT         6U
#define LOG_PRINT_BUFFER_LEN    UINT16_MAX

typedef struct {
    log_level_list_t level;
}logger_config_t;

typedef struct {
    log_level_list_t level;
    const char * const level_str;
} log_level_and_string_t;

static const char LOG_ERROR_CHAR[]      = "[ERROR   ]: ";
static const char LOG_WARNING_CHAR[]    = "[WARNING ]: ";
static const char LOG_CRITICAL_CHAR[]   = "[CRITICAL]: ";
static const char LOG_INFO_CHAR[]       = "[INFO    ]: ";
static const char LOG_DEBUG_CHAR[]      = "[DEBUG   ]: ";
static const char LOG_TRACE_CHAR[]      = "[TRACE   ]: ";

static logger_config_t logger_conf = {
    .level = INFO
};

static const log_level_and_string_t log_level_and_string[LOG_LEVEL_COUNT] = {
    { .level = ERROR, .level_str = LOG_ERROR_CHAR },
    { .level = CRITICAL, .level_str = LOG_CRITICAL_CHAR },
    { .level = WARNING, .level_str = LOG_WARNING_CHAR },
    { .level = INFO, .level_str = LOG_INFO_CHAR },
    { .level = DEBUG, .level_str = LOG_DEBUG_CHAR },
    { .level = TRACE, .level_str = LOG_TRACE_CHAR }
};

static cool_status_t print_log_msg(const log_level_and_string_t *log_config,
                                   const char * restrict func,
                                   const char * restrict msg,
                                   const va_list args)
{
    if (log_config == NULL || logger_conf.level < log_config->level)
    {
        return COOL_NULL_POINTER_ERROR;
    }

    char buffer[LOG_PRINT_BUFFER_LEN] = { 0U };

    const size_t preamble_len = strlen(log_config->level_str);
    const size_t func_len = strlen(func);
    const size_t msg_len = strlen(msg);

    const size_t complete_msg_len = preamble_len + func_len + 2 + msg_len + 1; // +2 for ": " and +1 for null terminator

    if (complete_msg_len > LOG_PRINT_BUFFER_LEN)
    {
        return COOL_OVERFLOW;
    }

    const int snprintf_result = snprintf(buffer, sizeof(buffer), "%s%s: %s", log_config->level_str, func, msg);
    if (snprintf_result < 0 || (size_t)snprintf_result >= sizeof(buffer))
    {
        return COOL_FORMAT_ERROR;
    }

    const int vprintf_result = vprintf(buffer, args);
    if (vprintf_result < 0 || (size_t)vprintf_result != strlen(buffer))
    {
        return COOL_PRINT_FAILED;
    }

    return COOL_OK;
}

static cool_status_t log_generic(const log_level_list_t level,
                                 const char * restrict func,
                                 const char * restrict msg,
                                 const va_list args)
{
    const cool_status_t result = print_log_msg(&log_level_and_string[level], func, msg, args);
    return result;
}

cool_status_t logger_init(const log_level_list_t log_level)
{
    if (log_level > TRACE)
    {
        return COOL_LOGGER_LEVEL_ERROR;
    }
    logger_conf.level = log_level;
    return COOL_OK;
}

cool_status_t logger_set_level(const log_level_list_t log_level)
{
    if(log_level > TRACE)
    {
        return COOL_WRONG_INPUT_PARAMETER;
    }

    logger_conf.level = log_level;
    return COOL_OK;
}

log_level_list_t logger_get_level(void)
{
    return logger_conf.level;
}

cool_status_t log_error(const char * restrict func, const char * restrict msg, ...)
{
    if(func == NULL || msg == NULL)
    {
        return COOL_WRONG_INPUT_PARAMETER;
    }

    if((logger_conf.level) < ERROR)
    {
        return COOL_OK;
    }

    va_list args;
    va_start(args, msg);
    const cool_status_t result = log_generic(ERROR, func, msg, args);
    va_end(args);
    return result;
}

cool_status_t log_critical(const char * restrict func, const char * restrict msg, ...)
{
    if(func == NULL || msg == NULL)
    {
        return COOL_WRONG_INPUT_PARAMETER;
    }

    if((logger_conf.level) < CRITICAL)
    {
        return COOL_OK;
    }

    va_list args;
    va_start(args, msg);
    const cool_status_t result = log_generic(CRITICAL, func, msg, args);
    va_end(args);
    return result;
}

cool_status_t log_warning(const char * restrict func, const char * restrict msg, ...)
{
    if(func == NULL || msg == NULL)
    {
        return COOL_WRONG_INPUT_PARAMETER;
    }

    if((logger_conf.level) < WARNING)
    {
        return COOL_OK;
    }

    va_list args;
    va_start(args, msg);
    const cool_status_t result = log_generic(WARNING, func, msg, args);
    va_end(args);
    return result;
}

cool_status_t log_info(const char * restrict func, const char * restrict msg, ...)
{
    if(func == NULL || msg == NULL)
    {
        return COOL_WRONG_INPUT_PARAMETER;
    }

    if((logger_conf.level) < INFO)
    {
        return COOL_OK;
    }

    va_list args;
    va_start(args, msg);
    const cool_status_t result = log_generic(INFO, func, msg, args);
    va_end(args);
    return result;
}

cool_status_t log_debug(const char * restrict func, const char * restrict msg, ...)
{
    if(func == NULL || msg == NULL)
    {
        return COOL_WRONG_INPUT_PARAMETER;
    }

    if((logger_conf.level) < DEBUG)
    {
        return COOL_OK;
    }

    va_list args;
    va_start(args, msg);
    const cool_status_t result = log_generic(DEBUG, func, msg, args);
    va_end(args);
    return result;
}

cool_status_t log_trace(const char * restrict func, const char * restrict msg, ...)
{
    if(func == NULL || msg == NULL)
    {
        return COOL_WRONG_INPUT_PARAMETER;
    }

    if((logger_conf.level) < TRACE)
    {
        return COOL_OK;
    }

    va_list args;
    va_start(args, msg);
    const cool_status_t result = log_generic(TRACE, func, msg, args);
    va_end(args);
    return result;
}
