//
// Created by Thomas Woltering on 04.02.25.
//

#ifndef LOGGER_CONFIG_CHECK_H
#define LOGGER_CONFIG_CHECK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "logger_config.h"
#include "logger.h"

#if DEFAULT_LOG_LEVEL < 0
#error "Log level must be greater than 0"
#endif

#if (LOG_PRINT_BUFFER_LEN <= 0u)
#error "Log buffer length must be greater than 0"
#endif


#if defined(LOG_WITH_PRINTF) && defined(LOG_WITH_COOL_PRINT)
#error "Only one print method can be selected: either LOG_WITH_PRINTF or LOG_WITH_COOL_PRINT"
#elif !defined(LOG_WITH_PRINTF) && !defined(LOG_WITH_COOL_PRINT)
#error "No print method selected: define either LOG_WITH_PRINTF or LOG_WITH_COOL_PRINT"
#endif


#if defined (BUILD_DEPENDING_LEVELS)
#if !defined(RELEASE) && !defined(DEBUG) && !defined(TEST)
#error "No Build Depending Defines"
#endif // !defined(RELEASE) && !defined(DEBUG)
#endif // defined (BUILD_DEPENDING_LEVELS)

#ifdef __cplusplus
}
#endif

#endif //LOGGER_CONFIG_CHECK_H
