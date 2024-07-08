//
// Created by 666 on 08.07.2024.
//

#ifndef STRINGMAP_H
#define STRINGMAP_H
#include <stddef.h>

typedef void* (*StringMapFunction)(const char*, void*);
typedef void (*StringMapConsumer)(const char*, void*);

typedef struct StringPair {
    const char* key;
    void*       value;
} StringPair;

typedef struct StringMap {
    StringPair* items;
    size_t      size;
    size_t      capacity;
} StringMap;

StringMap* newStringMap(size_t initialCapacity);
int stringMapPut(StringMap* map, const char* key, void* value);
void* stringMapGet(const StringMap* map, const char* key);
void* stringMapCompute(StringMap* map, const char* key, StringMapFunction remappingFunction);
void stringMapIterate(const StringMap* map, StringMapConsumer consumer);
int stringMapRemove(StringMap* map, const char* key);
void freeStringMap(StringMap* map);


#endif //STRINGMAP_H
