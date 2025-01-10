//
// Created by WART3K on 20.10.24.
//

#include "hal_logger.h"

#include "Cool17/status.h"

cool_status_t hal_logger_init(void)
{
    return COOL_NOT_IMPLEMENTED;
}

cool_status_t get_hal_logger_time(logger_time_t* logger_time)
{
    logger_time->hour = 0u;
    logger_time->minute = 0u;
    logger_time->second = 0u;

    return COOL_UNKNOWN;
}
