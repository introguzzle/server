#include "../include/intmap.h"
#include <stdlib.h>

#include "log.h"

IntMap* newIntMap(const size_t initialCapacity) {
    IntMap* map = malloc(sizeof(IntMap));
    if (map == NULL) {
        return NULL;
    }

    map->items = (IntPair*) malloc(initialCapacity * sizeof(IntPair));
    if (map->items == NULL) {
        free(map);
        return NULL;
    }

    map->size = 0;
    map->capacity = initialCapacity;

    return map;
}

int intMapPut(IntMap* map, const size_t key, void* value) {
    for (size_t i = 0; i < map->size; ++i) {
        if (map->items[i].key == key) {
            map->items[i].value = value;
            return 0;
        }
    }

    if (map->size == map->capacity) {
        const size_t newCapacity = map->capacity * 2;
        IntPair* newItems = realloc(map->items, newCapacity * sizeof(IntPair));
        if (newItems == NULL) {
            return -1;
        }
        map->items = newItems;
        map->capacity = newCapacity;
    }

    map->items[map->size].key = key;
    map->items[map->size].value = value;
    map->size++;
    return 0;
}

void* intMapGet(const IntMap* map, const size_t key) {
    for (size_t i = 0; i < map->size; ++i) {
        if (map->items[i].key == key) {
            return map->items[i].value;
        }
    }
    return NULL;
}

void* intMapCompute(IntMap* map, size_t key, IntMapFunction remappingFunction) {
    for (size_t i = 0; i < map->size; ++i) {
        if (map->items[i].key == key) {
            map->items[i].value = remappingFunction(map->items[i].key, map->items[i].value);
            return map->items[i].value;
        }
    }

    if (map->size == map->capacity) {
        map->capacity *= 2;
        map->items = (IntPair*) realloc(map->items, map->capacity * sizeof(IntPair));
        if (map->items == NULL) {
            return NULL;
        }
    }

    map->items[map->size].key = key;
    map->items[map->size].value = remappingFunction(key, NULL);
    map->size++;
    return map->items[map->size - 1].value;
}

void intMapIterate(const IntMap* map, IntMapConsumer consumer) {
    for (size_t i = 0; i < map->size; ++i) {
        consumer(map->items[i].key, map->items[i].value);
    }
}

int intMapRemove(IntMap* map, const size_t key) {
    for (size_t i = 0; i < map->size; ++i) {
        if (map->items[i].key == key) {
            for (size_t j = i; j < map->size - 1; ++j) {
                map->items[j] = map->items[j + 1];
            }
            map->size--;
            return 0;
        }
    }
    return -1;
}

void freeIntMap(IntMap* map) {
    free(map->items);
    free(map);
}
