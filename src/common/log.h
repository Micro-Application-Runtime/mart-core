#ifndef __LOG_H__
#define __LOG_H__

#define VERBOSE_LEVEL_LABEL "[VERBOSE]";
#define DEBUG_LEVEL_LABEL "[ DEBUG ]";
#define INFO_LEVEL_LABEL "[ INFO  ]";
#define WARNNING_LEVEL_LABEL "[ WARN  ]";
#define ERROR_LEVEL_LABEL "[ ERROR ]";
#define FATAL_LEVEL_LABEL "[ FATAL ]";

#define LOG_V(TAG, FMT, ...)
#define LOG_D(TAG, FMT, ...)
#define LOG_I(TAG, FMT, ...)
#define LOG_W(TAG, FMT, ...)
#define LOG_E(TAG, FMT, ...)
#define LOG_F(TAG, FMT, ...)

void log(fmt, ...);

#endif // __LOG_H__