//
// Created by 666 on 08.07.2024.
//

#ifndef INTMAP_H
#define INTMAP_H
#include <stddef.h>

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

IntMap* newIntMap(size_t initialCapacity);
int intMapPut(IntMap* map, size_t key, void* value);
void* intMapGet(const IntMap* map, size_t key);
void* intMapCompute(IntMap* map, size_t key, IntMapFunction remappingFunction);
void intMapIterate(const IntMap* map, IntMapConsumer consumer);
int intMapRemove(IntMap* map, size_t key);
void freeIntMap(IntMap* map);

#endif //INTMAP_H
