//
// Created by WART3K on 20.10.24.
//

#ifndef C_LIB_TESTER_HAL_LOGGER_H
#define C_LIB_TESTER_HAL_LOGGER_H

#include <stdint.h>

#include "../../status.h"

typedef struct {
    uint32_t hour;
    uint32_t minute;
    uint32_t second;
}logger_time_t;

cool_status_t hal_logger_init(void);

cool_status_t get_hal_logger_time(logger_time_t* logger_time);

#endif //C_LIB_TESTER_HAL_LOGGER_H
