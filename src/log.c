#include <stdio.h>
#include "log.h"

#include <time.h>
#include <windows.h>

static LoggingLevel current_level = LEVEL_DEBUG;

int setConsoleColor(const LoggingLevel level) {
    unsigned short word;

    switch (level) {
        case LEVEL_DEBUG:
            word = LOG_COLOR_WHITE;
            break;
        case LEVEL_INFO:
            word = LOG_COLOR_PINK;
            break;
        case LEVEL_WARNING:
            word = LOG_COLOR_BLUE;
            break;
        case LEVEL_ERROR:
            word = LOG_COLOR_RED;
            break;
        case LEVEL_CRITICAL:
            word = LOG_COLOR_RED;
            break;
        default:
            word = LOG_COLOR_RED;
            break;
    }

    const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    const WINBOOL winbool = SetConsoleTextAttribute(console, word);
    return winbool;
}

void setLoggingLevel(const LoggingLevel level) {
    current_level = level;
}

char* formatMessage(const LoggingLevel level) {
    switch (level) {
        case LEVEL_DEBUG:
            return "[DEBUG] [%s] : %s\n";
        case LEVEL_INFO:
            return "[INFO] [%s] : %s\n";
        case LEVEL_WARNING:
            return "[WARNING] [%s]: %s\n";
        case LEVEL_ERROR:
            return "[ERROR] [%s] : %s\n";
        case LEVEL_CRITICAL:
            return "[CRITICAL] [%s]: %s\n";
        default:
            return "[UNKNOWN] [%s]: %s\n";
    }
}

LoggingLevel logMessage(
    const LoggingLevel level,
    const char* format,
    ...
) {
    setConsoleColor(level);

    const time_t now = time(NULL);
    char *timeFormat = ctime(&now);
    timeFormat[strlen(timeFormat) - 1] = '\0';

    // Обработка переменных аргументов
    char formattedMessage[1024]; // Предположим, что это максимальный размер вашего сообщения
    va_list args;
    va_start(args, format);
    vsnprintf(formattedMessage, sizeof(formattedMessage), format, args);
    va_end(args);

    // Вывод сообщения
    fprintf(stdout, formatMessage(level), timeFormat, formattedMessage);

    setConsoleColor(LOG_COLOR_WHITE);
    return level;
}

LoggingLevel logDebug(const char* format, ...) {
    char formattedMessage[1024];
    va_list args;
    va_start(args, formattedMessage);
    vsnprintf(formattedMessage, sizeof(formattedMessage), format, args);
    va_end(args);

    return logMessage(LOG_LEVEL_DEBUG, formattedMessage, args);
}

LoggingLevel logInfo(const char* format, ...) {
    char formattedMessage[1024];
    va_list args;
    va_start(args, formattedMessage);
    vsnprintf(formattedMessage, sizeof(formattedMessage), format, args);
    va_end(args);

    return logMessage(LOG_LEVEL_INFO, formattedMessage, args);
}

LoggingLevel logWarning(const char* format, ...) {
    char formattedMessage[1024];
    va_list args;
    va_start(args, formattedMessage);
    vsnprintf(formattedMessage, sizeof(formattedMessage), format, args);
    va_end(args);

    return logMessage(LOG_LEVEL_WARNING, formattedMessage, args);
}

LoggingLevel logError(const char* format, ...) {
    char formattedMessage[1024];
    va_list args;
    va_start(args, formattedMessage);
    vsnprintf(formattedMessage, sizeof(formattedMessage), format, args);
    va_end(args);

    return logMessage(LOG_LEVEL_ERROR, formattedMessage, args);
}

LoggingLevel logCritical(const char* format, ...) {
    char formattedMessage[1024];
    va_list args;
    va_start(args, formattedMessage);
    vsnprintf(formattedMessage, sizeof(formattedMessage), format, args);
    va_end(args);

    return logMessage(LOG_LEVEL_CRITICAL, formattedMessage, args);
}


void debug(const char* message) {
    typedef unsigned short us;
    const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    for (us i = 0; i < 255; i++) {
        SetConsoleTextAttribute(console, i);
        printf("%3d  %s\n", i, message);
    }
}
