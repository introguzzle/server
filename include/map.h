//
// Created by 666 on 07.07.2024.
//

#ifndef MAP_H
#define MAP_H

#include <stddef.h>

#define FLAG_SHOW_META  0x01 // 0000 0001
#define FLAG_SHOW_ITEMS 0x04 // 0000 0100

typedef struct Pair {
    void* key;
    void* value;
} Pair;

typedef struct Map {
    Pair*  items;
    size_t size;
    size_t capacity;
} Map;

typedef void* (*BiFunction)(const void*, const void*);
typedef void (*Consumer)(const void*, const void*);
typedef int (*Comparator)(const void* , const void*);

void logPair(const void* a, const void* b);
void logMap(const Map* map, int flags);

Map* newMap(size_t initialCapacity);

int stringComparator(const void* a, const void* b);
int intComparator(const void* a, const void* b);

int mapPut(Map* map, void* key, void* value, Comparator comparator);
void* mapGet(const Map* map, const void* key, Comparator comparator);
int mapRemove(Map* map, const void* key, Comparator comparator);

void mapIterate(const Map* map, Consumer consumer);
void* mapCompute(Map* map, void* key, BiFunction remappingFunction, Comparator comparator);

const char* mapInfo(const Map* map);
void freeMap(Map* map);

#endif //MAP_H
