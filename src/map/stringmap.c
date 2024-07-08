#include "../../include/stringmap.h"
#include <stdlib.h>
#include <string.h>

StringMap* newStringMap(const size_t initialCapacity) {
    StringMap* map = malloc(sizeof(StringMap));
    if (map == NULL) {
        return NULL;
    }

    map->items = (StringPair*) malloc(initialCapacity * sizeof(StringPair));
    if (map->items == NULL) {
        free(map);
        return NULL;
    }

    map->size = 0;
    map->capacity = initialCapacity;

    return map;
}

static int mapFindKey(const StringMap* map, const char* key) {
    for (size_t i = 0; i < map->size; ++i) {
        if (strcmp(map->items[i].key, key) == 0) {
            return i;
        }
    }
    return -1;
}

int stringMapPut(StringMap* map, const char* key, void* value) {
    const int index = mapFindKey(map, key);
    if (index != -1) {
        map->items[index].value = value;
        return 0;
    }

    if (map->size == map->capacity) {
        const size_t newCapacity = map->capacity * 2;
        StringPair* newItems = realloc(map->items, newCapacity * sizeof(StringPair));
        if (newItems == NULL) {
            return -1;
        }
        map->items = newItems;
        map->capacity = newCapacity;
    }

    map->items[map->size].key = strdup(key);
    if (map->items[map->size].key == NULL) {
        return -1;
    }
    map->items[map->size].value = value;
    map->size++;
    return 0;
}

void* stringMapGet(const StringMap* map, const char* key) {
    const int index = mapFindKey(map, key);
    if (index != -1) {
        return map->items[index].value;
    }
    return NULL;
}

void* stringMapCompute(StringMap* map, const char* key, const StringMapFunction remappingFunction) {
    for (size_t i = 0; i < map->size; ++i) {
        if (strcmp(map->items[i].key, key) == 0) {
            map->items[i].value = remappingFunction(map->items[i].key, map->items[i].value);
            return map->items[i].value;
        }
    }

    if (map->size == map->capacity) {
        const size_t newCapacity = map->capacity * 2;
        StringPair* newItems = realloc(map->items, newCapacity * sizeof(StringPair));
        if (newItems == NULL) {
            return NULL;
        }
        map->items = newItems;
        map->capacity = newCapacity;
    }

    map->items[map->size].key = strdup(key);
    if (map->items[map->size].key == NULL) {
        return NULL;
    }
    map->items[map->size].value = remappingFunction(key, NULL);
    map->size++;
    return map->items[map->size - 1].value;
}

void stringMapIterate(const StringMap* map, const StringMapConsumer consumer) {
    for (size_t i = 0; i < map->size; ++i) {
        consumer(map->items[i].key, map->items[i].value);
    }
}

int stringMapRemove(StringMap* map, const char* key) {
    const int index = mapFindKey(map, key);
    if (index == -1) {
        return -1;
    }

    for (size_t i = index; i < map->size - 1; ++i) {
        map->items[i] = map->items[i + 1];
    }

    map->size--;
    return 0;
}

void freeStringMap(StringMap* map) {
    free(map->items);
    free(map);
}
