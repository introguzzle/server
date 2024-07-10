//
// Created by 666 on 07.07.2024.
//

#include "../include/vector.h"

#include <stdlib.h>
#include <iterable.h>
#include <stdbool.h>

#ifndef VECTOR_MAP_MAGIC_FIELD_DEFINED
#define VECTOR_MAP_MAGIC_FIELD_DEFINED
static const long long VECTOR_MAP_MAGIC_FIELD = 872817821L;
#endif

Vector* NewVector() {
    return NewVectorCapacity(VECTOR_INITIAL_CAPACITY);
}

Vector* NewVectorCapacity(const size_t initialCapacity) {
    Vector* vector = malloc(sizeof(Vector));

    vector->items = malloc(sizeof(void*) * initialCapacity);
    if (vector->items == NULL) {
        free(vector);
        return NULL;
    }

    vector->size = 0;
    vector->capacity = initialCapacity;
    vector->____ = VECTOR_MAP_MAGIC_FIELD;

    return vector;
}

bool IsVector(void* p) {
    if (p == NULL) {
        return false;
    }

    const Vector* v = p;
    if (v->____ == VECTOR_MAP_MAGIC_FIELD) {
        return true;
    }

    return false;
}

Vector* VectorReverse(const Vector* vector) {
    Vector* result = NewVectorCapacity(vector->capacity);

    void* item;
    foreachReversed(vector, item) {
        VectorPush(result, item);
    }

    return result;
}

bool VectorReverseMutate(Vector* vector) {
    Vector* result = NewVectorCapacity(vector->capacity);

    void* item;
    foreachReversed(vector, item) {
        VectorPush(result, item);
    }

    vector->items = result->items;
    return true;
}

bool VectorInsert(Vector* vector, const size_t index, void* item) {
    if (vector == NULL || vector->items == NULL) {
        return false;
    }

    if (index > vector->size) {
        return false;
    }

    if (index == vector->size) {
        if (VectorPush(vector, item) == false) {
            return false;
        }
    } else {
        if (vector->size == vector->capacity) {
            if (VectorResize(vector, 2 * vector->capacity) == false)
                return false;
        }

        for (int i = vector->size; i > index; i--) {
            vector->items[i] = vector->items[i - 1];
        }

        vector->items[index] = item;
        vector->size++;
    }

    return true;
}

bool VectorResize(Vector* vector, const size_t newCapacity) {
    void** items = realloc(vector->items, newCapacity * sizeof(void*));
    if (vector == NULL || items == NULL) {
        return false;
    }

    vector->items = items;
    vector->capacity = newCapacity;

    return true;
}

bool VectorPush(Vector* vector, void* item) {
    if (vector == NULL || vector->items == NULL) {
        return false;
    }

    if (vector->size == vector->capacity) {
        const int ret = VectorResize(vector, 2 * vector->capacity);
        if (ret == false)
            return false;
    }

    vector->items[vector->size++] = item;

    return true;
}

bool VectorDelete(Vector *vector, const size_t index) {
    if (vector == NULL || vector->items == NULL) {
        return false;
    }

    if (index >= vector->size) {
        return false;
    }

    vector->size--;

    for (int i = index; i < vector->size; i++) {
        vector->items[i] = vector->items[i + 1];
    }

    const size_t quarter = vector->size == vector->capacity / 4;
    if (quarter && vector->capacity > VECTOR_INITIAL_CAPACITY) {
        if (VectorResize(vector, vector->capacity / 2) == false) {
            return false;
        }
    }

    return true;
}

void* VectorPop(Vector* vector) {
    if (vector->size == (size_t) 0) {
        return NULL;
    }

    return vector->items[--vector->size];
}

bool VectorContains(Vector* vector, void* item) {
    if (vector == NULL) {
        return false;
    }

    for (size_t i = 0; i < vector->size; i++) {
        if (vector->items[i] == item) {
            return true;
        }
    }

    return false;
}

int VectorIndexOf(Vector* vector, void* item) {
    if (vector == NULL) {
        return -1;
    }

    for (size_t i = 0; i < vector->size; i++) {
        if (vector->items[i] == item)
            return i;
    }

    return -1;
}

IntMap* VectorToIntMap(Vector* vector) {
    IntMap* map = newIntMap(vector->capacity);

    for (size_t i = 0; i < vector->size; i++) {
        intMapPut(map, i, vector->items[i]);
    }

    return map;
}

void VectorDestroy(Vector* vector) {
    free(vector->items);
    free(vector);
}


