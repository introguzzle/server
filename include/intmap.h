//
// Created by 666 on 08.07.2024.
//

#ifndef INTMAP_H
#define INTMAP_H
#include <stddef.h>

#define INT_MAP_INITIAL_CAPACITY (size_t) 1 << 3

typedef void* (*IntMapFunction)(size_t, const void*);
typedef void (*IntMapConsumer)(size_t, const void*);

typedef struct IntPair {
    size_t key;
    void*  value;
} IntPair;

typedef struct IntMap {
    IntPair* items;
    size_t   size;
    size_t   capacity;
} IntMap;

IntMap* newIntMap();
IntMap* newIntMapCapacity(size_t initialCapacity);
int intMapPut(IntMap* map, size_t key, void* value);
void* intMapGet(const IntMap* map, size_t key);
void* intMapCompute(IntMap* map, size_t key, IntMapFunction remappingFunction);
void intMapIterate(const IntMap* map, IntMapConsumer consumer);
int intMapRemove(IntMap* map, size_t key);
void freeIntMap(IntMap* map);

#endif //INTMAP_H
