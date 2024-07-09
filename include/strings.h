#ifndef STRINGS_H
#define STRINGS_H

char* ltrim(const char* s);
char* rtrim(const char* s);
char* trim(const char* s);

#define FLAG_CONCAT_LINE_FEED 0x01
#define FLAG_CONCAT_CARRIAGE_RETURN 0x04

char* concat(int flags, const char* s, ...);
size_t countTokens(const char* s, const char *delimiter);
void freeSplit(char **tokens);
char** split(const char* s, const char* delimiter, size_t* size);
#endif // STRINGS_H
