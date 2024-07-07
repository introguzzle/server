#ifndef PARSING_H
#define PARSING_H

char* ltrim(const char* s);
char* rtrim(const char* s);
char* trim(const char* s);

size_t countTokens(const char* s, const char *delimiter);
void freeSplit(char **tokens);
char** split(const char* s, const char* delimiter, size_t* size);
#endif // PARSING_H
