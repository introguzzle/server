#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "strings.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>

#include "log.h"
#include "vector.h"

char* NewString(const size_t length) {
    char* s = malloc((length + 1) * sizeof(char));
    if (s != NULL) {
        s[0] = '\0';
        s[length] = '\0';
    }

    return s;
}

char* NewStringPrefix(const size_t length, const char* prefix) {
    char* s = NewString(length);

    if (s != NULL) {
        s = concat(s, prefix, NULL);
        s[length] = '\0';
        return s;
    }

    return NULL;
}

char* concatDelim(const int flags, const char* s, ...) {
    va_list args;
    va_start(args, s);

    size_t totalLength = strlen(s) + 1;
    char* str;
    va_list args_copy;
    va_copy(args_copy, args);
    while ((str = va_arg(args_copy, char*)) != NULL) {
        if (flags & FLAG_CONCAT_LINE_FEED) {
            totalLength += 1;
        }

        if (flags & FLAG_CONCAT_CARRIAGE_RETURN) {
            totalLength += 1;
        }

        totalLength += strlen(str);
    }
    va_end(args_copy);

    char* result = malloc(totalLength);
    if (result == NULL) {
        logError("Memory allocation failed");
        va_end(args);
        return NULL;
    }

    result[0] = '\0';
    strncat(result, s, totalLength - 1);

    va_start(args, s);
    size_t currentLength = strlen(result);
    while ((str = va_arg(args, char*)) != NULL) {
        if (flags & FLAG_CONCAT_CARRIAGE_RETURN) {
            strncat(result, "\r", totalLength - currentLength - 1);
            currentLength += 1;
        }
        if (flags & FLAG_CONCAT_LINE_FEED) {
            strncat(result, "\n", totalLength - currentLength - 1);
            currentLength += 1;
        }

        strncat(result, str, totalLength - currentLength - 1);
        currentLength += strlen(str);
    }

    va_end(args);

    return result;
}

Vector* split(const char* s, const char* delimiter) {
    Vector* vector = NewVector();

    size_t size;
    char** splitted = _split(s, delimiter, &size);
    size_t i = 0;
    while (splitted[i] != NULL) {
        VectorPush(vector, splitted[i]);
        i++;
    }

    return vector;
}

size_t countTokens(const char* s, const char *delimiter) {
    int count = 0;
    char *copy = strdup(s);
    const char *token = strtok(copy, delimiter);

    while (token != NULL) {
        count++;
        token = strtok(NULL, delimiter);
    }

    free(copy);
    return count;
}

char* ltrim(const char* s) {
    while (isspace(*s)) {
        s++;
    }

    return strdup(s);
}

char* rtrim(const char* s) {
    char* copy = strdup(s);
    if (copy == NULL) {
        return NULL;
    }

    char* back = copy + strlen(copy) - 1;
    while (back >= copy && isspace(*back)) {
        *back = '\0';
        back--;
    }
    return copy;
}

char* trim(const char* s) {
    char* leftTrimmed = ltrim(s);
    if (leftTrimmed == NULL) {
        return NULL;
    }

    char* fullyTrimmed = rtrim(leftTrimmed);
    free(leftTrimmed);

    return fullyTrimmed;
}

void freeSplit(char **tokens) {
    for (int i = 0; tokens[i] != NULL; i++) {
        free(tokens[i]);
    }
    free(tokens);
}

char** _split(const char* s, const char* delimiter, size_t* size) {
    *size = countTokens(s, delimiter);

    char** result = malloc((*size + 1) * sizeof(char*));
    char* copy = strdup(s);
    const char* token = strtok(copy, delimiter);

    int i = 0;

    while (token != NULL) {
        result[i] = strdup(token);
        i++;
        token = strtok(NULL, delimiter);
    }

    result[i] = NULL;
    free(copy);
    return result;
}