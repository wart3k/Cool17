//
// Created by WART3K on 19.10.24.
//

#include "logger.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>


#if defined(LOG_WITH_PRINTF)
#include <stdio.h>
#endif //defined(LOG_WITH_PRINTF)

#define LOG_LEVEL_COUNT         6U

#if defined(__APPLE__) || defined(_WIN32) || defined(__unix__)
#define FORMAT_UNSIGNED_8_BIT           "%u"
#define FORMAT_SIGNED_8_BIT             "%d"
#define FORMAT_HEX_8_BIT                "0x%02x"
#define FORMAT_UNSIGNED_16_BIT          "%u"
#define FORMAT_SIGNED_16_BIT            "%d"
#define FORMAT_HEX_16_BIT               "0x%04x"
#define FORMAT_UNSIGNED_DEC_32_BIT      "%u"
#define FORMAT_SIGNED_DEC_32_BIT        "%d"
#define FORMAT_HEX_32_BIT               "0x%08x"
#define FORMAT_UNSIGNED_DEC_64_BIT      "%" PRIu64
#define FORMAT_SIGNED_DEC_64_BIT        "%" PRId64
#define FORMAT_HEX_64_BIT               "0x%016" PRIx64
#define FORMAT_FLOAT                    "%f"
#define FORMAT_DOUBLE                   "%fd"
#else
#define FORMAT_UNSIGNED_16_BIT          "%u"
#define FORMAT_SIGNED_16_BIT            "%d"
#define FORMAT_HEX_16_BIT               "0x%04x"
#define FORMAT_UNSIGNED_16_BIT          "%u"
#define FORMAT_UNSIGNED_32_DEC          "%lu"
#define FORMAT_SIGNED_32_DEC            "%ld"
#define FORMAT_HEX_32_BIT               "0x%08lx"
#define FORMAT_UNSIGNED_DEC_64_BIT      "%" PRIu64
#define FORMAT_SIGNED_DEC_64_BIT        "%" PRId64
#define FORMAT_FLOAT                    "%f"
#define FORMAT_DOUBLE                   "%fd"
#endif

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
    { .level = LOG_LEVEL_ERROR, .level_str = LOG_ERROR_CHAR },
    { .level = LOG_LEVEL_CRITICAL, .level_str = LOG_CRITICAL_CHAR },
    { .level = LOG_LEVEL_WARNING, .level_str = LOG_WARNING_CHAR },
    { .level = LOG_LEVEL_INFO, .level_str = LOG_INFO_CHAR },
    { .level = LOG_LEVEL_DEBUG, .level_str = LOG_DEBUG_CHAR },
    { .level = LOG_LEVEL_TRACE, .level_str = LOG_TRACE_CHAR }
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

    if (preamble_len + func_len + msg_len  >= sizeof(buffer))
    {
        return COOL_OVERFLOW;
    }

    // +2 for ": " and +1 for null terminator
    const size_t complete_msg_len = preamble_len + func_len + 2 + msg_len + 1;

    if (complete_msg_len >= sizeof(buffer))
    {
        return COOL_OVERFLOW;
    }

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
    if(log_level > LOG_LEVEL_TRACE)
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

    va_list args;
    va_start(args, msg);
    const cool_status_t result = log_generic(LOG_LEVEL_ERROR, func, msg, args);
    va_end(args);
    return result;
}

cool_status_t log_critical(const char * restrict func, const char * restrict msg, ...)
{
    if(func == NULL || msg == NULL)
    {
        return COOL_WRONG_INPUT_PARAMETER;
    }

    if(logger_conf.level < LOG_LEVEL_CRITICAL)
    {
        return COOL_OK;
    }

    va_list args;
    va_start(args, msg);
    const cool_status_t result = log_generic(LOG_LEVEL_CRITICAL, func, msg, args);
    va_end(args);
    return result;
}

cool_status_t log_warning(const char * restrict func, const char * restrict msg, ...)
{
    if(func == NULL || msg == NULL)
    {
        return COOL_WRONG_INPUT_PARAMETER;
    }

    if(logger_conf.level < LOG_LEVEL_WARNING)
    {
        return COOL_OK;
    }

    va_list args;
    va_start(args, msg);
    const cool_status_t result = log_generic(LOG_LEVEL_WARNING, func, msg, args);
    va_end(args);
    return result;
}

cool_status_t log_info(const char * restrict func, const char * restrict msg, ...)
{
    if(func == NULL || msg == NULL)
    {
        return COOL_WRONG_INPUT_PARAMETER;
    }

    if(logger_conf.level < LOG_LEVEL_INFO)
    {
        return COOL_OK;
    }

    va_list args;
    va_start(args, msg);
    const cool_status_t result = log_generic(LOG_LEVEL_INFO, func, msg, args);
    va_end(args);
    return result;
}

cool_status_t log_debug(const char * restrict func, const char * restrict msg, ...)
{
    if(func == NULL || msg == NULL)
    {
        return COOL_WRONG_INPUT_PARAMETER;
    }

    if(logger_conf.level < LOG_LEVEL_DEBUG)
    {
        return COOL_OK;
    }

    va_list args;
    va_start(args, msg);
    const cool_status_t result = log_generic(LOG_LEVEL_DEBUG, func, msg, args);
    va_end(args);
    return result;
}

cool_status_t log_trace(const char * restrict func, const char * restrict msg, ...)
{
    if(func == NULL || msg == NULL)
    {
        return COOL_WRONG_INPUT_PARAMETER;
    }

    if(logger_conf.level < LOG_LEVEL_TRACE)
    {
        return COOL_OK;
    }

    va_list args;
    va_start(args, msg);
    const cool_status_t result = log_generic(LOG_LEVEL_TRACE, func, msg, args);
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

    if(logger_conf.level < level)
    {
        return COOL_OK;
    }

    size_t element_size = 0;
    bool is_binary_format = false;

    switch (format)
    {
        case S8_BIN:
        case S16_BIN:
        case S32_BIN:
        case S64_BIN:
            is_binary_format = true;
            break;

        case U8_HEX:
        case U16_HEX:
        case U32_HEX:
        case U64_HEX:
        case U8_DEC:
        case U16_DEC:
        case U32_DEC:
        case U64_DEC:
        case S8_DEC:
        case S16_DEC:
        case S32_DEC:
        case S64_DEC:
        case FLOAT:
        case DOUBLE:
            is_binary_format = false;
            break;

        default:
            return COOL_WRONG_INPUT_PARAMETER;
    }

    switch (format)
    {
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

        case FLOAT:
            element_size = sizeof(float);
            break;

        case DOUBLE:
            element_size = sizeof(double);
            break;

        default:
            return COOL_FORMAT_ERROR;
    }

    if (array_size % element_size != 0)
    {
        (void)CLOGE("Array size (%zu) is not valid for element size (%zu).", array_size, element_size);
        return COOL_FORMAT_ERROR;
    }

    const size_t num_elements = array_size / element_size;

    char buffer[LOG_PRINT_BUFFER_LEN] = { 0 };
    int offset = 0;

    for (size_t i = 0; i < num_elements; i++)
    {
        int written = 0;

        switch (format)
        {
            case U8_DEC:
                written = snprintf(buffer + offset, sizeof(buffer) - (size_t)offset, FORMAT_UNSIGNED_8_BIT, ((uint8_t *)array)[i]);
            break;

            case S8_DEC:
                written = snprintf(buffer + offset, sizeof(buffer) - (size_t)offset, FORMAT_SIGNED_8_BIT, ((int8_t *)array)[i]);
            break;

            case U8_HEX:
                written = snprintf(buffer + offset, sizeof(buffer) - (size_t)offset, FORMAT_HEX_8_BIT, ((uint8_t *)array)[i]);
            break;

            case S8_BIN:
            {
                const uint8_t value = ((uint8_t *)array)[i];
                written = snprintf(buffer + offset, sizeof(buffer) - (size_t)offset, "0b");
                if (written >= 0 && (size_t)written < sizeof(buffer) - (size_t)offset)
                {
                    offset += written;

                    for (int8_t b = 7; b >= 0; b--)
                    {
                        written = snprintf(buffer + offset, sizeof(buffer) - (size_t)offset, FORMAT_SIGNED_8_BIT, (value >> b) & 1);

                        if (written < 0 || (size_t)written >= sizeof(buffer) - (size_t)offset)
                        {
                            (void)CLOGE("Buffer overflow occurred.");
                            return COOL_FORMAT_ERROR;
                        }

                        offset += written;
                    }
                }
            }
            break;

            case U16_DEC:
                written = snprintf(buffer + offset, sizeof(buffer) - (size_t)offset, FORMAT_UNSIGNED_16_BIT, ((uint16_t *)array)[i]);
            break;

            case S16_DEC:
                written = snprintf(buffer + offset, sizeof(buffer) - (size_t)offset, FORMAT_SIGNED_16_BIT, ((int16_t *)array)[i]);
            break;

            case U16_HEX:
                written = snprintf(buffer + offset, sizeof(buffer) - (size_t)offset, FORMAT_HEX_16_BIT, ((uint16_t *)array)[i]);
            break;

            case S16_BIN:
            {
                const uint16_t value = ((uint16_t *)array)[i];
                written = snprintf(buffer + offset, sizeof(buffer) - (size_t)offset, "0b");
                if (written >= 0 && (size_t)written < sizeof(buffer) - (size_t)offset)
                {
                    offset += written;

                    for (int8_t b = 15; b >= 0; b--)
                    {
                        written = snprintf(buffer + offset, sizeof(buffer) - (size_t)offset, FORMAT_SIGNED_16_BIT, (value >> b) & 1);

                        if (written < 0 || (size_t)written >= sizeof(buffer) - (size_t)offset)
                        {
                            (void)CLOGE("Buffer overflow occurred.");
                            return COOL_FORMAT_ERROR;
                        }

                        offset += written;
                    }
                }
            }
            break;

            case U32_DEC:
                written = snprintf(buffer + offset, sizeof(buffer) - (size_t)offset, FORMAT_UNSIGNED_DEC_32_BIT, ((uint32_t *)array)[i]);
            break;

            case S32_DEC:
                written = snprintf(buffer + offset, sizeof(buffer) - (size_t)offset, FORMAT_SIGNED_DEC_32_BIT, ((int32_t *)array)[i]);
            break;

            case U32_HEX:
                written = snprintf(buffer + offset, sizeof(buffer) - (size_t)offset, FORMAT_HEX_32_BIT, ((uint32_t *)array)[i]);
            break;

            case S32_BIN:
            {
                const uint32_t value = ((uint32_t *)array)[i];
                written = snprintf(buffer + offset, sizeof(buffer) - (size_t)offset, "0b");
                if (written >= 0 && (size_t)written < sizeof(buffer) - (size_t)offset)
                {
                    offset += written;
                    for (int8_t b = 31; b >= 0; b--)
                    {
                        written = snprintf(buffer + offset, sizeof(buffer) - (size_t)offset, FORMAT_SIGNED_DEC_32_BIT, (value >> b) & 1);

                        if (written < 0 || (size_t)written >= sizeof(buffer) - (size_t)offset)
                        {
                            (void)CLOGE("Buffer overflow occurred.");
                            return COOL_FORMAT_ERROR;
                        }

                        offset += written;
                    }
                }
            }
            break;

            case U64_DEC:
                written = snprintf(buffer + offset, sizeof(buffer) - (size_t)offset, FORMAT_UNSIGNED_DEC_64_BIT, ((uint64_t *)array)[i]);
            break;

            case S64_DEC:
                written = snprintf(buffer + offset, sizeof(buffer) - (size_t)offset, FORMAT_SIGNED_DEC_64_BIT, ((int64_t *)array)[i]);
            break;

            case U64_HEX:
                written = snprintf(buffer + offset, sizeof(buffer) - (size_t)offset, FORMAT_HEX_64_BIT, ((uint64_t *)array)[i]);
            break;

            case S64_BIN:
            {
                const uint64_t value = ((uint64_t *)array)[i];
                written = snprintf(buffer + offset, sizeof(buffer) - (size_t)offset, "0b");
                if (written >= 0 && (size_t)written < sizeof(buffer) - (size_t)offset)
                {
                    offset += written;

                    for (int8_t b = 63; b >= 0; b--)
                    {
                        written = snprintf(buffer + offset, sizeof(buffer) - (size_t)offset, FORMAT_SIGNED_DEC_64_BIT, (value >> b) & 1);

                        if (written < 0 || (size_t)written >= sizeof(buffer) - (size_t)offset)
                        {
                            CLOGE("Buffer overflow occurred.");
                            return COOL_FORMAT_ERROR;
                        }

                        offset += written;
                    }
                }
            }
            break;

            case FLOAT:
                written = snprintf(buffer + offset, sizeof(buffer) - (size_t)offset, FORMAT_FLOAT, ((float *)array)[i]);
                break;

            case DOUBLE:
                written = snprintf(buffer + offset, sizeof(buffer) - (size_t)offset, FORMAT_DOUBLE, ((double *)array)[i]);
                break;

            default:
                return COOL_FORMAT_ERROR;
        }

        if (written < 0 || (size_t)written >= sizeof(buffer) - (size_t)offset)
        {
            (void)CLOGE("Buffer overflow occurred.");
            return COOL_FORMAT_ERROR;
        }

        if (!is_binary_format)
        {
            offset += written;
        }

        if (i + 1 < num_elements)
        {
            if ((size_t)offset < sizeof(buffer) - 1)
            {
                buffer[offset++] = ' ';
            }

            else
            {
                (void)CLOGE("Buffer overflow occurred.");
                return COOL_FORMAT_ERROR;
            }
        }
    }

    if ((size_t)offset < sizeof(buffer) - 1)
    {
        buffer[(size_t)offset] = '\n';
    }

    else
    {
        (void)CLOGE("Buffer overflow occurred while adding newline.");
        return COOL_FORMAT_ERROR;
    }

    switch (level)
    {
        case LOG_LEVEL_ERROR:
            return log_error(func, buffer);
        case LOG_LEVEL_CRITICAL:
            return log_critical(func, buffer);
        case LOG_LEVEL_WARNING:
            return log_warning(func, buffer);
        case LOG_LEVEL_INFO:
            return log_info(func, buffer);
        case LOG_LEVEL_DEBUG:
            return log_debug(func, buffer);
        case LOG_LEVEL_TRACE:
            return log_trace(func, buffer);
        default:
            return COOL_FORMAT_ERROR;
    }
}
