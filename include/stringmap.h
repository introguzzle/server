//
// Created by 666 on 08.07.2024.
//

#ifndef STRINGMAP_H
#define STRINGMAP_H
#include <stddef.h>

#define STRING_MAP_INITIAL_CAPACITY (size_t) 1 << 3

typedef void* (*StringMapFunction)(const char*, void*);
typedef void (*StringMapConsumer)(const char*, void*);

typedef struct StringPair {
    const char* key;
    void*       value;
} StringPair;

typedef struct StringMap {
    StringPair*    items;
    size_t         size;
    size_t         capacity;
    long long      ____;
} StringMap;

StringMap* NewStringMap();
StringMap* NewStringMapCapacity(size_t initialCapacity);

int IsStringMap(void* p);

int StringMapPut(StringMap* map, const char* key, void* value);
void* StringMapGet(const StringMap* map, const char* key);
void* StringMapCompute(StringMap* map, const char* key, StringMapFunction remappingFunction);
void StringMapIterate(const StringMap* map, StringMapConsumer consumer);
int StringMapRemove(StringMap* map, const char* key);
int StringMapContainsKey(StringMap* map, const char* key);
int StringMapContainsValue(StringMap* map, void* value);
int StringMapIndexOfKey(StringMap* map, const char* key);
int StringMapIndexOfValue(StringMap* map, void* value);

void StringMapDestroy(StringMap* map);


#endif //STRINGMAP_H
