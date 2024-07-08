//
// Created by 666 on 07.07.2024.
//

#include "../include/vector.h"

#include <stdlib.h>

Vector* newVector() {
    Vector* vector = malloc(sizeof(Vector));

    vector->items = malloc(sizeof(void*) * VECTOR_INITIAL_CAPACITY);
    if (vector->items == NULL) {
        free(vector);
        return NULL;
    }

    vector->size = 0;
    vector->capacity = VECTOR_INITIAL_CAPACITY;

    return vector;
}

int vectorInsert(Vector* vector, const size_t index, void* item) {
    if (vector == NULL || vector->items == NULL) {
        return -1;
    }

    if (index > vector->size) {
        return -1;
    }

    if (index == vector->size) {
        if (vectorPush(vector, item) == -1) {
            return -1;
        }
    } else {
        if (vector->size == vector->capacity) {
            if (vectorResize(vector, 2 * vector->capacity) == -1)
                return -1;
        }

        for (int i = vector->size; i > index; i--) {
            vector->items[i] = vector->items[i - 1];
        }

        vector->items[index] = item;
        vector->size++;
    }

    return 0;
}

int vectorResize(Vector* vector, const size_t newCapacity) {
    void** items = realloc(vector->items, newCapacity * sizeof(void*));
    if (items == NULL) {
        return -1;
    }

    if (items == vector->items) {
        free(items);
        return -1;
    }

    vector->capacity = newCapacity;
    vector->items = items;

    return 0;
}

int vectorPush(Vector* vector, void* item) {
    if (vector == NULL || vector->items == NULL) {
        return -1;
    }

    if (vector->size == vector->capacity) {
        const int ret = vectorResize(vector, 2 * vector->capacity);
        if (ret == -1)
            return -1;
    }

    vector->items[vector->size++] = item;

    return 0;
}

int vectorDelete(Vector *vector, const size_t index) {
    if (vector == NULL || vector->items == NULL) {
        return -1;
    }

    if (index >= vector->size) {
        return -1;
    }

    vector->size--;

    for (int i = index; i < vector->size; i++) {
        vector->items[i] = vector->items[i + 1];
    }

    const size_t quarter = vector->size == vector->capacity / 4;
    if (quarter && vector->capacity > VECTOR_INITIAL_CAPACITY) {
        if (vectorResize(vector, vector->capacity / 2) == -1) {
            return -1;
        }
    }

    return 0;
}

void* vectorPop(Vector* vector) {
    if (vector->size == (size_t) 0) {
        return NULL;
    }

    return vector->items[--vector->size];
}

void freeVector(Vector* vector) {
    free(vector->items);
    free(vector);
}

int vectorContains(Vector* vector, void* item) {
    if (vector == NULL) {
        return -1;
    }

    for (size_t i = 0; i < vector->size; i++) {
        if (vector->items[i] == item) {
            return 1;
        }
    }

    return -1;
}

int vectorIndexOf(Vector* vector, void* item) {
    if (vector == NULL) {
        return -1;
    }

    for (size_t i = 0; i < vector->size; i++) {
        if (vector->items[i] == item)
            return i;
    }

    return -1;
}



