//
// Created by WART3K on 19.10.24.
//

#include "logger.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if defined(LOG_WITH_PRINTF)
#include <stdio.h>
#endif //defined(LOG_WITH_PRINTF)

#define LOG_LEVEL_COUNT         6U

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
    .level = DEFAULT_LOG_LEVEL
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
        return COOL_NULL_POINTER_ERROR;
    
    char buffer[LOG_PRINT_BUFFER_LEN] = { 0U };

    const size_t preamble_len = strlen(log_config->level_str);
    const size_t func_len = strlen(func);
    const size_t msg_len = strlen(msg);

    if (preamble_len + func_len + msg_len  >= sizeof(buffer))
        return COOL_OVERFLOW;

    // +2 for ": " and +1 for null terminator
    const size_t complete_msg_len = preamble_len + func_len + 2 + msg_len + 1;

    if (complete_msg_len >= sizeof(buffer))
        return COOL_OVERFLOW;

#if defined(LOG_WITH_PRINTF)
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
#endif //defined(LOG_WITH_PRINTF)

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

cool_status_t log_array(const char * restrict func,
                        const log_level_list_t level,
                        const log_format_t format,
                        const void *array,
                        const size_t array_size)
{
    if (func == NULL || format > S64_BIN || array == NULL || array_size == 0)
    {
        return COOL_WRONG_INPUT_PARAMETER;
    }

    if((logger_conf.level) < level)
    {
        return COOL_OK;
    }

    size_t element_size = 0;

    switch (format) {
        case U8_DEC:
        case S8_DEC:
        case U8_HEX:
        case S8_BIN:
            element_size = sizeof(uint8_t);
            break;

        case U16_DEC:
        case S16_DEC:
        case U16_HEX:
        case S16_BIN:
            element_size = sizeof(uint16_t);
            break;

        case U32_DEC:
        case S32_DEC:
        case U32_HEX:
        case S32_BIN:
            element_size = sizeof(uint32_t);
            break;

        case U64_DEC:
        case S64_DEC:
        case U64_HEX:
        case S64_BIN:
            element_size = sizeof(uint64_t);
            break;

        default:
            return COOL_FORMAT_ERROR;
    }

    if (array_size % element_size != 0) {
        CLOGE("Array size (%zu) is not valid for element size (%zu).", array_size, element_size);
        return COOL_FORMAT_ERROR;
    }

    size_t num_elements = array_size / element_size;

    char buffer[LOG_PRINT_BUFFER_LEN] = { 0 };
    size_t offset = 0;

    for (size_t i = 0; i < num_elements; i++) {
        int written = 0;

        switch (format) {
            case U8_DEC:
                written = snprintf(buffer + offset, sizeof(buffer) - offset, "%u", ((uint8_t *)array)[i]);
            break;

            case S8_DEC:
                written = snprintf(buffer + offset, sizeof(buffer) - offset, "%d", ((int8_t *)array)[i]);
            break;

            case U8_HEX:
                written = snprintf(buffer + offset, sizeof(buffer) - offset, "0x%02x", ((uint8_t *)array)[i]);
            break;

            case S8_BIN: {
                uint8_t value = ((uint8_t *)array)[i];
                written = snprintf(buffer + offset, sizeof(buffer) - offset, "0b");
                if (written >= 0 && (size_t)written < sizeof(buffer) - offset) {
                    offset += written;
                    for (int b = 7; b >= 0; b--) {
                        written = snprintf(buffer + offset, sizeof(buffer) - offset, "%d", (value >> b) & 1);
                        if (written < 0 || (size_t)written >= sizeof(buffer) - offset) {
                            CLOGE("Buffer overflow occurred.");
                            return COOL_FORMAT_ERROR;
                        }
                        offset += written;
                    }
                }
                break;
            }

            case U16_DEC:
                written = snprintf(buffer + offset, sizeof(buffer) - offset, "%u", ((uint16_t *)array)[i]);
            break;

            case S16_DEC:
                written = snprintf(buffer + offset, sizeof(buffer) - offset, "%d", ((int16_t *)array)[i]);
            break;

            case U16_HEX:
                written = snprintf(buffer + offset, sizeof(buffer) - offset, "0x%04x", ((uint16_t *)array)[i]);
            break;

            case U32_DEC:
                written = snprintf(buffer + offset, sizeof(buffer) - offset, "%u", ((uint32_t *)array)[i]);
            break;

            case S32_DEC:
                written = snprintf(buffer + offset, sizeof(buffer) - offset, "%d", ((int32_t *)array)[i]);
            break;

            case U32_HEX:
                written = snprintf(buffer + offset, sizeof(buffer) - offset, "0x%08x", ((uint32_t *)array)[i]);
            break;

            case U64_DEC:
                written = snprintf(buffer + offset, sizeof(buffer) - offset, "%llu", ((uint64_t *)array)[i]);
            break;

            case S64_DEC:
                written = snprintf(buffer + offset, sizeof(buffer) - offset, "%lld", ((int64_t *)array)[i]);
            break;

            case U64_HEX:
                written = snprintf(buffer + offset, sizeof(buffer) - offset, "0x%016llx", ((uint64_t *)array)[i]);
            break;

            default:
                return COOL_FORMAT_ERROR;
        }

        if (written < 0 || (size_t)written >= sizeof(buffer) - offset) {
            CLOGE("Buffer overflow occurred.");
            return COOL_FORMAT_ERROR;
        }

        offset += written;

        if (i + 1 < num_elements) {
            if (offset < sizeof(buffer) - 1) {
                buffer[offset++] = ' ';
            } else {
                CLOGE("Buffer overflow occurred.");
                return COOL_FORMAT_ERROR;
            }
        }
    }

    if (offset < sizeof(buffer) - 1) {
        buffer[offset++] = '\n';
    } else {
        CLOGE("Buffer overflow occurred while adding newline.");
        return COOL_FORMAT_ERROR;
    }


    switch (level) {
        case ERROR:
            return log_error(func, "%s", buffer);
        case CRITICAL:
            return log_critical(func, "%s", buffer);
        case WARNING:
            return log_warning(func, "%s", buffer);
        case INFO:
            return log_info(func, "%s", buffer);
        case DEBUG:
            return log_debug(func, "%s", buffer);
        case TRACE:
            return log_trace(func, "%s", buffer);
        default:
            return COOL_FORMAT_ERROR;
    }
}


