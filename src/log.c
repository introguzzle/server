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
            return "[DEBUG] [%s] [File %s on line %d]: %s\n";
        case LEVEL_INFO:
            return "[INFO] [%s] [File %s on line %d]: %s\n";
        case LEVEL_WARNING:
            return "[WARNING] [%s] [File %s on line %d]: %s\n";
        case LEVEL_ERROR:
            return "[ERROR] [%s] [File %s on line %d]: %s\n";
        case LEVEL_CRITICAL:
            return "[CRITICAL] [%s] [File %s on line %d]: %s\n";
        default:
            return "[UNKNOWN] [%s] [File %s on line %d]: %s\n";
    }
}

LoggingLevel logMessage(
    const char* file,
    const int line,
    const LoggingLevel level,
    const char* format,
    ...
) {
    setConsoleColor(level);

    const time_t now = time(NULL);
    char *timeFormat = ctime(&now);
    timeFormat[strlen(timeFormat) - 1] = '\0';

    char formattedMessage[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(formattedMessage, sizeof(formattedMessage), format, args);
    va_end(args);

    fprintf(stdout, formatMessage(level), timeFormat, file, line, formattedMessage);

    setConsoleColor(LOG_COLOR_WHITE);
    return level;
}

void debug(const char* message) {
    typedef unsigned short us;
    const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    for (us i = 0; i < 255; i++) {
        SetConsoleTextAttribute(console, i);
        printf("%3d  %s\n", i, message);
    }
}
