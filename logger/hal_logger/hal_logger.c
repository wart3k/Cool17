//
// Created by WART3K on 20.10.24.
//

#include "hal_logger.h"

cool_status_t hal_logger_init(void)
{
    return COOL_OK;
}

logger_time_t get_hal_logger_time(void)
{
    logger_time_t current_time = {
        .status = COOL_UNKNOWN,
        .hour = 0U,
        .minute = 0U,
        .second = 0U
    };

    return current_time;
}
