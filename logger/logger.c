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
#define FORMAT_UNSIGNED_8_BIT           "%u"
#define FORMAT_SIGNED_8_BIT             "%d"
#define FORMAT_HEX_8_BIT                "0x%02x"
#define FORMAT_UNSIGNED_16_BIT          "%u"
#define FORMAT_SIGNED_16_BIT            "%d"
#define FORMAT_HEX_16_BIT               "0x%04x"
#define FORMAT_UNSIGNED_16_BIT          "%u"
#define FORMAT_UNSIGNED_DEC_32_BIT      "%lu"
#define FORMAT_SIGNED_DEC_32_BIT        "%ld"
#define FORMAT_HEX_32_BIT               "0x%08lx"
#define FORMAT_UNSIGNED_DEC_64_BIT      "%" PRIu64
#define FORMAT_SIGNED_DEC_64_BIT        "%" PRId64
#define FORMAT_HEX_64_BIT               "0x%016" PRIx64
#define FORMAT_FLOAT                    "%f"
#define FORMAT_DOUBLE                   "%fd"
#endif

typedef struct {
    size_t element_size;
    bool is_binary;
    const char* format_str;
} format_info_t;

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

static const format_info_t format_info[] = {
    [U8_DEC]  = { sizeof(uint8_t),  false, FORMAT_UNSIGNED_8_BIT },
    [S8_DEC]  = { sizeof(int8_t),   false, FORMAT_SIGNED_8_BIT   },
    [U8_HEX]  = { sizeof(uint8_t),  false, FORMAT_HEX_8_BIT      },
    [S8_BIN]  = { sizeof(uint8_t),  true,  "0b"                  },
    [U16_DEC] = { sizeof(uint16_t), false, FORMAT_UNSIGNED_16_BIT},
    [S16_DEC] = { sizeof(int16_t),  false, FORMAT_SIGNED_16_BIT  },
    [U16_HEX] = { sizeof(uint16_t), false, FORMAT_HEX_16_BIT     },
    [S16_BIN] = { sizeof(uint16_t), true,  "0b"                  },
    [U32_DEC] = { sizeof(uint32_t), false, FORMAT_UNSIGNED_DEC_32_BIT },
    [S32_DEC] = { sizeof(int32_t),  false, FORMAT_SIGNED_DEC_32_BIT   },
    [U32_HEX] = { sizeof(uint32_t), false, FORMAT_HEX_32_BIT     },
    [S32_BIN] = { sizeof(uint32_t), true,  "0b"                  },
    [U64_DEC] = { sizeof(uint64_t), false, FORMAT_UNSIGNED_DEC_64_BIT },
    [S64_DEC] = { sizeof(int64_t),  false, FORMAT_SIGNED_DEC_64_BIT   },
    [U64_HEX] = { sizeof(uint64_t), false, FORMAT_HEX_64_BIT     },
    [S64_BIN] = { sizeof(uint64_t), true,  "0b"                  },
    [FLOAT]   = { sizeof(float),    false, FORMAT_FLOAT          },
    [DOUBLE]  = { sizeof(double),   false, FORMAT_DOUBLE         }
};

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

static logger_status_t print_log_msg(const log_level_and_string_t *log_config,
                                   const char * restrict func,
                                   const char * restrict msg,
                                   const va_list args)
{
    if (log_config == NULL || logger_conf.level < log_config->level)
    {
        return LOGGER_NULL_POINTER_ERROR;
    }

    char buffer[LOG_PRINT_BUFFER_LEN] = { 0U };

    const size_t preamble_len = strlen(log_config->level_str);
    const size_t func_len = strlen(func);
    const size_t msg_len = strlen(msg);

    if (preamble_len + func_len + msg_len  >= sizeof(buffer))
    {
        return LOGGER_OVERFLOW;
    }

    // +2 for ": " and +1 for null terminator
    const size_t complete_msg_len = preamble_len + func_len + 2 + msg_len + 1;

    if (complete_msg_len >= sizeof(buffer))
    {
        return LOGGER_OVERFLOW;
    }

#if defined(LOG_WITH_PRINTF)
    const int snprintf_result = snprintf(buffer, sizeof(buffer), "%s%s: %s", log_config->level_str, func, msg);
    if (snprintf_result < 0 || (size_t)snprintf_result >= sizeof(buffer))
    {
        return LOGGER_FORMAT_ERROR;
    }

    const int vprintf_result = vprintf(buffer, args);
    if (vprintf_result < 0 || (size_t)vprintf_result != strlen(buffer))
    {
        return LOGGER_PRINT_FAILED;
    }
#endif //defined(LOG_WITH_PRINTF)

    return LOGGER_STATUS_OK;
}

static logger_status_t log_generic(const log_level_list_t level,
                                 const char * restrict func,
                                 const char * restrict msg,
                                 const va_list args)
{
    const logger_status_t result = print_log_msg(&log_level_and_string[level], func, msg, args);
    return result;
}

logger_status_t logger_set_level(const log_level_list_t log_level)
{
    if(log_level > LOG_LEVEL_TRACE)
    {
        return LOGGER_WRONG_INPUT_PARAMETER;
    }

    logger_conf.level = log_level;
    return LOGGER_STATUS_OK;
}

log_level_list_t logger_get_level(void)
{
    return logger_conf.level;
}

logger_status_t log_error(const char * restrict func, const char * restrict msg, ...)
{
    if(func == NULL || msg == NULL)
    {
        return LOGGER_WRONG_INPUT_PARAMETER;
    }

    va_list args;
    va_start(args, msg);
    const logger_status_t result = log_generic(LOG_LEVEL_ERROR, func, msg, args);
    va_end(args);
    return result;
}

logger_status_t log_critical(const char * restrict func, const char * restrict msg, ...)
{
    if(func == NULL || msg == NULL)
    {
        return LOGGER_WRONG_INPUT_PARAMETER;
    }

    if(logger_conf.level < LOG_LEVEL_CRITICAL)
    {
        return LOGGER_STATUS_OK;
    }

    va_list args;
    va_start(args, msg);
    const logger_status_t result = log_generic(LOG_LEVEL_CRITICAL, func, msg, args);
    va_end(args);
    return result;
}

logger_status_t log_warning(const char * restrict func, const char * restrict msg, ...)
{
    if(func == NULL || msg == NULL)
    {
        return LOGGER_WRONG_INPUT_PARAMETER;
    }

    if(logger_conf.level < LOG_LEVEL_WARNING)
    {
        return LOGGER_STATUS_OK;
    }

    va_list args;
    va_start(args, msg);
    const logger_status_t result = log_generic(LOG_LEVEL_WARNING, func, msg, args);
    va_end(args);
    return result;
}

logger_status_t log_info(const char * restrict func, const char * restrict msg, ...)
{
    if(func == NULL || msg == NULL)
    {
        return LOGGER_WRONG_INPUT_PARAMETER;
    }

    if(logger_conf.level < LOG_LEVEL_INFO)
    {
        return LOGGER_STATUS_OK;
    }

    va_list args;
    va_start(args, msg);
    const logger_status_t result = log_generic(LOG_LEVEL_INFO, func, msg, args);
    va_end(args);
    return result;
}

logger_status_t log_debug(const char * restrict func, const char * restrict msg, ...)
{
    if(func == NULL || msg == NULL)
    {
        return LOGGER_WRONG_INPUT_PARAMETER;
    }

    if(logger_conf.level < LOG_LEVEL_DEBUG)
    {
        return LOGGER_STATUS_OK;
    }

    va_list args;
    va_start(args, msg);
    const logger_status_t result = log_generic(LOG_LEVEL_DEBUG, func, msg, args);
    va_end(args);
    return result;
}

logger_status_t log_trace(const char * restrict func, const char * restrict msg, ...)
{
    if(func == NULL || msg == NULL)
    {
        return LOGGER_WRONG_INPUT_PARAMETER;
    }

    if(logger_conf.level < LOG_LEVEL_TRACE)
    {
        return LOGGER_STATUS_OK;
    }

    va_list args;
    va_start(args, msg);
    const logger_status_t result = log_generic(LOG_LEVEL_TRACE, func, msg, args);
    va_end(args);
    return result;
}

static int format_binary(char *buffer, size_t buffer_size, const uint64_t value, const size_t num_bits) {
    if (buffer == NULL || num_bits == 0 || (num_bits & (num_bits - 1)) != 0) {
        return LOGGER_WRONG_INPUT_PARAMETER;
    }

    int offset = snprintf(buffer, buffer_size, "0b");

    if (offset < 0 || (size_t)offset >= buffer_size) {
        return LOGGER_OVERFLOW;
    }

    for (int b = (int)num_bits - 1; b >= 0; b--) {
        int written = snprintf(buffer + offset, buffer_size - (size_t)offset, "%d", (int)((value >> b) & 1));
        if (written < 0 || (size_t)written >= buffer_size - (size_t)offset) {
            return LOGGER_OVERFLOW;
        }
        offset += written;
    }

    return offset;
}

static int format_element(char *buffer, size_t buffer_size, const void *element, const log_format_t format) {
    if (format_info[format].is_binary) {
        uint64_t value = 0;
        memcpy(&value, element, format_info[format].element_size);
        return format_binary(buffer, buffer_size, value, format_info[format].element_size * 8);
    }

    return snprintf(buffer, buffer_size, format_info[format].format_str, *(const uint64_t *)element);
}

logger_status_t log_array(const char *restrict func, const log_level_list_t level, const log_format_t format,
                          const void *array, const size_t array_size) {

    if (!func || format >= sizeof(format_info)/sizeof(format_info[0]) || !array || array_size == 0) {
        return LOGGER_WRONG_INPUT_PARAMETER;
    }

    if (logger_conf.level < level) {
        return LOGGER_STATUS_OK;
    }

    const format_info_t *info = &format_info[format];
    if (!info->element_size || array_size % info->element_size != 0) {
        (void)CLOGE("Array size (%zu) is not valid for element size (%zu).", array_size, info->element_size);
        return LOGGER_FORMAT_ERROR;
    }

    const size_t num_elements = array_size / info->element_size;
    char buffer[LOG_PRINT_BUFFER_LEN] = { 0 };
    int offset = 0;

    for (size_t i = 0; i < num_elements; i++) {
        const int written = format_element(buffer + offset, sizeof(buffer) - (size_t)offset, (uint8_t *)array + i * info->element_size, format);
        if (written < 0 || (size_t)written >= sizeof(buffer) - (size_t)offset) {
            (void)CLOGE("Buffer overflow occurred.");
            return LOGGER_FORMAT_ERROR;
        }
        offset += written;

        if (i + 1 < num_elements) {
            if ((size_t)offset < sizeof(buffer) - 1) {
                buffer[offset++] = ' ';
            } else {
                return LOGGER_FORMAT_ERROR;
            }
        }
    }

    if ((size_t)offset < sizeof(buffer) - 1) {
        buffer[offset] = '\n';
    } else {
        (void)CLOGE("Buffer overflow occurred while adding newline.");
        return LOGGER_FORMAT_ERROR;
    }

    switch (level) {
        case LOG_LEVEL_ERROR:    return log_error(func, buffer);
        case LOG_LEVEL_CRITICAL: return log_critical(func, buffer);
        case LOG_LEVEL_WARNING:  return log_warning(func, buffer);
        case LOG_LEVEL_INFO:     return log_info(func, buffer);
        case LOG_LEVEL_DEBUG:    return log_debug(func, buffer);
        case LOG_LEVEL_TRACE:    return log_trace(func, buffer);
        default: return LOGGER_FORMAT_ERROR;
    }
}

