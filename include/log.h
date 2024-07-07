#ifndef P_LOG_H
#define P_LOG_H

#define LOG_LEVEL_DEBUG    0
#define LOG_LEVEL_INFO     1
#define LOG_LEVEL_WARNING  2
#define LOG_LEVEL_ERROR    3
#define LOG_LEVEL_CRITICAL 4

#define LOG_COLOR_WHITE 7
#define LOG_COLOR_GREEN 2
#define LOG_COLOR_RED 12
#define LOG_COLOR_BLUE 9
#define LOG_COLOR_PINK 13

typedef enum LoggingLevel {
    LEVEL_DEBUG    = LOG_LEVEL_DEBUG,
    LEVEL_INFO     = LOG_LEVEL_INFO,
    LEVEL_WARNING  = LOG_LEVEL_WARNING,
    LEVEL_ERROR    = LOG_LEVEL_ERROR,
    LEVEL_CRITICAL = LOG_LEVEL_CRITICAL
} LoggingLevel;

void setLogLevel(LoggingLevel level);
LoggingLevel logMessage(LoggingLevel level, const char* format, ...);

LoggingLevel logDebug(const char* format, ...);
LoggingLevel logInfo(const char* format, ...);
LoggingLevel logWarning(const char* format, ...);
LoggingLevel logError(const char* format, ...);
LoggingLevel logCritical(const char* format, ...);

void debug(const char* message);
#endif