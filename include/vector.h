//
// Created by 666 on 07.07.2024.
//

#ifndef VECTOR_H
#define VECTOR_H
#include <stddef.h>
#include "log.h"

#define VECTOR_INITIAL_CAPACITY 1 << 2

#define initializeVector(vector, type, initialCapacity) \
    do { \
        (vector) = malloc(sizeof(Vector)); \
        if ((vector) == NULL) { logError("Failed to allocate memory for Vector"); } \
        (vector)->items = malloc(sizeof(type) * initialCapacity); \
        if ((vector)->items == NULL) { free(vector);  logError("Failed to allocate memory for Vector items"); } \
        (vector)->size = 0; \
        (vector)->capacity = initialCapacity; \
    } while (0)

typedef struct Vector {
    void** items;
    size_t size;
    size_t capacity;
} Vector;

Vector* newVector();
int vectorInsert(Vector* vector, size_t index, void* item);
int vectorResize(Vector* vector, size_t newCapacity);
int vectorPush(Vector* vector, void* item);
int vectorDelete(Vector *vector, size_t index);
void* vectorPop(Vector* vector);
int vectorContains(Vector* vector, void* item);
int vectorIndexOf(Vector* vector, void* item);
void freeVector(Vector* vector);


#endif //VECTOR_H
