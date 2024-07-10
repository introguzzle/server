//
// Created by 666 on 07.07.2024.
//

#ifndef URL_H
#define URL_H
#include "stringmap.h"
#include "vector.h"

typedef const char* Path;
typedef struct QueryParameter {
    const char* key;
    const char* value;
} QueryParameter;


Path newPath(const char* s);
/**
 * @param path Path
 * @return Vector*<Path>
 */
Vector* pathSplit(Path path);
Path pathChild(Path path);
Path pathAddParent(Path path, const char* parent);

StringMap* ParseQueryParameters(Path path);
#endif //URL_H
