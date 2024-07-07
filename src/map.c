#include <stddef.h>
#include <stdlib.h>
#include "map.h"

#include <stdio.h>
#include <string.h>

#include "log.h"

int stringComparator(const void* a, const void* b) {
    return strcmp(a, b);
}

int intComparator(const void* a, const void* b) {
    const int* _a = a;
    const int* _b = b;

    return *_a == *_b;
}

void logPair(const void* a, const void* b) {
    logInfo("Key:%s, Value:%s", (const char*) a, (const char*) b);
}

void logMap(const Map* map, int flags) {
    if (flags == 0) {
        flags = FLAG_SHOW_ITEMS | FLAG_SHOW_META;
    }

    if (flags & FLAG_SHOW_META) {
        logInfo("Map:%s\n", mapInfo(map));
    }

    if (flags & FLAG_SHOW_ITEMS) {
        logInfo("############ MAP ITEMS        ############ ");
        mapIterate(map, logPair);
        logInfo("############ END OF MAP ITEMS ############ ");
    }
}

Map* newMap(const size_t initialCapacity) {
    Map* map = malloc(sizeof(Map));
    if (map == NULL) {
        return NULL;
    }

    map->items = (Pair*) malloc(initialCapacity * sizeof(Pair));
    if (map->items == NULL) {
        free(map);
        return NULL;
    }

    map->size = 0;
    map->capacity = initialCapacity;

    return map;
}

static int mapFindKey(const Map* map, const void* key, const Comparator comparator) {
    for (size_t i = 0; i < map->size; ++i) {
        if (comparator(map->items[i].key, key) == 0) {
            return i;
        }
    }

    return -1;
}

const char* mapInfo(const Map* map) {
    size_t infoSize = 100;
    char* info = malloc(infoSize);

    if (info == NULL) {
        return NULL;
    }

    const int written = snprintf(
        info,
        infoSize,
        "Size: %llu\nCapacity: %llu\n",
        map->size,
        map->capacity
    );

    if (written >= infoSize) {
        infoSize = written + 1;
        info = (char*)realloc(info, infoSize);
        if (info == NULL) {
            return NULL;
        }

        snprintf(
            info,
            infoSize,
            "Size: %llu\nCapacity: %llu\n",
            map->size,
            map->capacity
        );
    }

    return info;
}

int mapPut(Map* map, void* key, void* value, const Comparator comparator) {
    const int index = mapFindKey(map, key, comparator);
    if (index != -1) {
        map->items[index].value = value;
        return 0;
    }

    if (map->size == map->capacity) {
        const size_t newCapacity = map->capacity * 2;
        Pair* newItems = realloc(map->items, newCapacity * sizeof(Pair));

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

void* mapGet(const Map* map, const void* key, const Comparator comparator) {
    const int index = mapFindKey(map, key, comparator);
    if (index != -1) {
        return map->items[index].value;
    }

    return NULL;
}



void* mapCompute(Map* map, void* key, const BiFunction remappingFunction, const Comparator comparator) {
    for (size_t i = 0; i < map->size; ++i) {
        if (comparator(map->items[i].key, key) == 0) {
            map->items[i].value = remappingFunction(map->items[i].key, map->items[i].value);
            return map->items[i].value;
        }
    }

    if (map->size == map->capacity) {
        map->capacity *= 2;
        map->items = (Pair*)realloc(map->items, map->capacity * sizeof(Pair));
        if (map->items == NULL) {
            return NULL;
        }
    }

    map->items[map->size].key = key;
    map->items[map->size].value = remappingFunction(key, NULL);
    map->size++;
    return map->items[map->size - 1].value;
}

void mapIterate(const Map* map, const Consumer consumer) {
    for (size_t i = 0; i < map->size; i++) {
        consumer(map->items[i].key, map->items[i].value);
    }
}

int mapRemove(Map* map, const void* key, const Comparator comparator) {
    const int index = mapFindKey(map, key, comparator);
    if (index == -1) {
        return -1;
    }

    for (size_t i = index; i < map->size - 1; ++i) {
        map->items[i] = map->items[i + 1];
    }
    map->size--;
    return 0;
}

void freeMap(Map* map) {
    free(map->items);
    free(map);
}


