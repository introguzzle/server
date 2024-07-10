#ifndef STRINGS_H
#define STRINGS_H
#include "vector.h"

char* NewString(size_t length);
char* NewStringPrefix(const size_t length, const char* prefix);

char* ltrim(const char* s);
char* rtrim(const char* s);
char* trim(const char* s);

/**
 * e.g. "\r"
 */
#define FLAG_CONCAT_LINE_FEED       1 << 0

/**
 * e.g. "\n"
 */
#define FLAG_CONCAT_CARRIAGE_RETURN 1 << 1

/**
 *
 * When calling this macro, last argument must be NULL, so variadic args work properly
 * @param s const char*
 */
#define concat(s, ...) concatDelim(0, s, ##__VA_ARGS__)
char* concatDelim(int flags, const char* s, ...);
size_t countTokens(const char* s, const char *delimiter);


Vector* split(const char* s, const char* delimiter);
char** _split(const char* s, const char* delimiter, size_t* size);
void freeSplit(char **tokens);


#endif // STRINGS_H
