#ifndef __COMMON__LOG_H__
#define __COMMON__LOG_H__

#include <stdio.h>
#include <time.h>

#define LOG_TIME_FORMAT "%Y-%m-%d %H:%M:%S"

#define LOG_LEVEL_VERBOSE 0
#define LOG_LEVEL_DEBUG 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_WARNING 3
#define LOG_LEVEL_ERROR 4
#define LOG_LEVEL_FATAL 5

#define LOG_LEVEL_VERBOSE_LABEL "VERBOSE"
#define LOG_LEVEL_DEBUG_LABEL "DEBUG"
#define LOG_LEVEL_INFO_LABEL "INFO"
#define LOG_LEVEL_WARNING_LABEL "WARNING"
#define LOG_LEVEL_ERROR_LABEL "ERROR"
#define LOG_LEVEL_FATAL_LABEL "FATAL"

#define LOG(level, tag, format, ...)                                                    \
    do                                                                                  \
    {                                                                                   \
        time_t t = time(NULL);                                                          \
        struct tm *tm_info = localtime(&t);                                             \
        char time_str[20];                                                              \
        strftime(time_str, sizeof(time_str), LOG_TIME_FORMAT, tm_info);                 \
        const char *level_str;                                                          \
        switch (level)                                                                  \
        {                                                                               \
        case LOG_LEVEL_VERBOSE:                                                         \
            level_str = LOG_LEVEL_VERBOSE_LABEL;                                        \
            break;                                                                      \
        case LOG_LEVEL_DEBUG:                                                           \
            level_str = LOG_LEVEL_DEBUG_LABEL;                                          \
            break;                                                                      \
        case LOG_LEVEL_INFO:                                                            \
            level_str = LOG_LEVEL_INFO_LABEL;                                           \
            break;                                                                      \
        case LOG_LEVEL_WARNING:                                                         \
            level_str = LOG_LEVEL_WARNING_LABEL;                                        \
            break;                                                                      \
        case LOG_LEVEL_ERROR:                                                           \
            level_str = LOG_LEVEL_ERROR_LABEL;                                          \
            break;                                                                      \
        case LOG_LEVEL_FATAL:                                                           \
            level_str = LOG_LEVEL_FATAL_LABEL;                                          \
            break;                                                                      \
        default:                                                                        \
            level_str = "UNKNOWN";                                                      \
        }                                                                               \
        printf("[%s] [%s] [%s] " format "\n", time_str, level_str, tag, ##__VA_ARGS__); \
    } while (0)

#define LOG_V(tag, format, ...) LOG(LOG_LEVEL_VERBOSE, tag, format, ##__VA_ARGS__)
#define LOG_D(tag, format, ...) LOG(LOG_LEVEL_DEBUG, tag, format, ##__VA_ARGS__)
#define LOG_I(tag, format, ...) LOG(LOG_LEVEL_INFO, tag, format, ##__VA_ARGS__)
#define LOG_W(tag, format, ...) LOG(LOG_LEVEL_WARNING, tag, format, ##__VA_ARGS__)
#define LOG_E(tag, format, ...) LOG(LOG_LEVEL_ERROR, tag, format, ##__VA_ARGS__)
#define LOG_F(tag, format, ...) LOG(LOG_LEVEL_FATAL, tag, format, ##__VA_ARGS__)

#endif // __COMMON__LOG_H__