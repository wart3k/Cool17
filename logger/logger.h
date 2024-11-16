//
// Created by WART3K on 19.10.24.
//

#ifndef COOL17_LOGGER_H
#define COOL17_LOGGER_H

#include <stdarg.h>

#include "../status.h"

typedef enum {
    ERROR = 0,
    CRITICAL,
    WARNING,
    INFO,
    DEBUG,
    TRACE
}log_level_list_t;

cool_status_t logger_init(log_level_list_t log_level);

cool_status_t logger_set_level(log_level_list_t log_level);
log_level_list_t logger_get_level(void);

cool_status_t log_error(const char * restrict func, const char * restrict msg, ...);
cool_status_t log_critical(const char * restrict func, const char * restrict msg, ...);
cool_status_t log_warning(const char * restrict func, const char * restrict msg, ...);
cool_status_t log_info(const char * restrict func, const char * restrict msg, ...);
cool_status_t log_debug(const char * restrict func, const char * restrict msg, ...);
cool_status_t log_trace(const char * restrict func, const char * restrict msg, ...);


#define CLOGE(msg, ...)     log_error(__func__, msg, ##__VA_ARGS__)
#define CLOGC(msg, ...)     log_critical(__func__, msg, ##__VA_ARGS__)
#define CLOGW(msg, ...)     log_warning(__func__, msg, ##__VA_ARGS__)
#define CLOGI(msg, ...)     log_info(__func__, msg, ##__VA_ARGS__)
#define CLOGD(msg, ...)     log_debug(__func__, msg, ##__VA_ARGS__)
#define CLOGT(msg, ...)     log_trace(__func__, msg, ##__VA_ARGS__)

#endif //COOL17_LOGGER_H
