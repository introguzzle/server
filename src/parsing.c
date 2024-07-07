#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "parsing.h"

#include <ctype.h>

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

char** split(const char* s, const char* delimiter, size_t* size) {
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