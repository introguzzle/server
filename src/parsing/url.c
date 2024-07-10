//
// Created by 666 on 07.07.2024.
//

#include "../../include/url.h"

#include <stdlib.h>
#include <string.h>

#include "iterable.h"
#include "strings.h"

QueryParameter* newQueryParameter(const char* key, const char* value) {
    QueryParameter* q = malloc(sizeof(QueryParameter));

    q->key = key;
    q->value = value;

    return q;
}

Path newPath(const char* s) {
    if (s[0] == '/') {
        return s;
    }

    return trim(concat("/", s, NULL));
}

/**
 *
 * @param path Path
 * @return Vector*<Path>
 */
Vector* pathSplit(const Path path) {
    return split(path, "/");
}

Path pathChild(const Path path) {
    Vector* splitted = pathSplit(path);

    const char* child = strdup(splitted->items[splitted->size - 1]);
    VectorDestroy(splitted);
    return child;
}

Path pathAddParent(const Path path, const char* parent) {
    Path p = newPath(parent);

    if (path[0] != '/') {
        const char* c = concat(p, path, NULL);
        return concat("/", c, NULL);
    }

    return concat(p, path, NULL);
}

StringMap* ParseQueryParameters(const Path path) {
    // GET /api/v1/clowns?x=1&y=2 HTTP/1.1
    StringMap* result = NewStringMap();

    if (path == NULL || path[0] == '\0') {
        return result;
    }

    const char* queryStart = strchr(path, '?');
    if (queryStart == NULL) {
        return result;
    }

    queryStart++;

    Vector* params = split(queryStart, "&");

    char* param;
    foreach(params, param) {
        Vector* paramSplitted = split(param, "=");

        if (paramSplitted->size != 2) {
            VectorDestroy(paramSplitted);
            continue;
        }

        StringMapPut(result, paramSplitted->items[0], paramSplitted->items[1]);
    }

    VectorDestroy(params);
    return result;
}

