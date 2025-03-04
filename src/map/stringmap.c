#include "../../include/stringmap.h"
#include <stdlib.h>
#include <string.h>
#include "iterable.h"
#include <stdbool.h>

// This field is for determining if void* pointers are actually this struct
static const long long STRING_MAP_MAGIC_FIELD = 21728121;

StringMap* NewStringMap() {
    return NewStringMapCapacity(STRING_MAP_INITIAL_CAPACITY);
}

StringMap* NewStringMapCapacity(const size_t initialCapacity) {
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
    map->____ = STRING_MAP_MAGIC_FIELD;

    return map;
}

int IsStringMap(void* p) {
    if (p == NULL) {
        return false;
    }

    const StringMap* m = p;

    if (m->____ == STRING_MAP_MAGIC_FIELD) {
        return true;
    }

    return false;
}

static int StringMapFindKey(const StringMap* map, const char* key) {
    for (size_t i = 0; i < map->size; ++i) {
        if (strcmp(map->items[i].key, key) == 0) {
            return i;
        }
    }

    return -1;
}

int StringMapPut(StringMap* map, const char* key, void* value) {
    const int index = StringMapFindKey(map, key);
    if (index != -1) {
        map->items[index].value = value;
        return true;
    }

    if (map->size == map->capacity) {
        const size_t newCapacity = map->capacity * 2;
        StringPair* newItems = realloc(map->items, newCapacity * sizeof(StringPair));
        if (newItems == NULL) {
            return false;
        }
        map->items = newItems;
        map->capacity = newCapacity;
    }

    map->items[map->size].key = strdup(key);
    if (map->items[map->size].key == NULL) {
        return false;
    }
    map->items[map->size].value = value;
    map->size++;
    return true;
}

void* StringMapGet(const StringMap* map, const char* key) {
    const int index = StringMapFindKey(map, key);
    if (index != -1) {
        return map->items[index].value;
    }
    return NULL;
}

void* StringMapCompute(StringMap* map, const char* key, const StringMapFunction remappingFunction) {
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

void StringMapIterate(const StringMap* map, const StringMapConsumer consumer) {
    for (size_t i = 0; i < map->size; ++i) {
        consumer(map->items[i].key, map->items[i].value);
    }
}

int StringMapRemove(StringMap* map, const char* key) {
    const int index = StringMapFindKey(map, key);
    if (index == -1) {
        return false;
    }

    for (size_t i = index; i < map->size - 1; ++i) {
        map->items[i] = map->items[i + 1];
    }

    map->size--;
    return true;
}

int StringMapContainsKey(StringMap* map, const char* key) {
    StringPair pair;
    foreach(map, pair) {
        if (strcmp(pair.key, key)) {
            return true;
        }
    }

    return false;
}

int StringMapIndexOfKey(StringMap* map, const char* key) {
    for (size_t i = 0; i < map->size; ++i) {
        if (strcmp(map->items[i].key, key)) {
            return i;
        }
    }

    return -1;
}

int StringMapIndexOfValue(StringMap* map, void* value) {
    for (size_t i = 0; i < map->size; ++i) {
        if (map->items[i].key == value) {
            return i;
        }
    }

    return -1;
}

int StringMapContainsValue(StringMap* map, void* value) {
    StringPair pair;
    foreach(map, pair) {
        if (pair.value == value) {
            return 1;
        }
    }

    return -1;
}

void StringMapDestroy(StringMap* map) {
    free(map->items);
    free(map);
}
