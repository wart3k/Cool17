# Cool Logger

## Introduction
A simple logger lib for c. 
Use The Makros CLOGx to log data like printf. For logging an array use CLOG_ARRAY.
To configure the library use the logger_conf.h

## Future Tasks
- implement more configurations
- improvements for embedded logging
- better configuration checks in logger_config_check
- adding tests

## Known Issues
- No float and double output with log array
- binary output not correct with log_array
- no "new line" after binary output