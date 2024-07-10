//
// Created by 666 on 07.07.2024.
//

#ifndef VECTOR_H
#define VECTOR_H
#include <stdbool.h>
#include <stddef.h>

#include "intmap.h"
#include "log.h"

#define VECTOR_INITIAL_CAPACITY 1 << 3

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
    void**         items;
    size_t         size;
    size_t         capacity;
    long long      ____;
} Vector;

Vector* NewVector();
Vector* NewVectorCapacity(size_t initialCapacity);
bool IsVector(void* p);

bool VectorInsert(Vector* vector, size_t index, void* item);
bool VectorResize(Vector* vector, size_t newCapacity);
bool VectorPush(Vector* vector, void* item);
bool VectorDelete(Vector *vector, size_t index);
void* VectorPop(Vector* vector);
bool VectorContains(Vector* vector, void* item);
int VectorIndexOf(Vector* vector, void* item);

Vector* VectorReverse(const Vector* vector);
bool VectorReverseMutate(Vector* vector);

IntMap* VectorToIntMap(Vector* vector);
void VectorDestroy(Vector* vector);

#endif //VECTOR_H
