//
// Created by 666 on 07.07.2024.
//

#include "../../include/url.h"

#include <stdlib.h>
#include <string.h>

Path newPath(const char* s) {
    if (s[0] == '/') {
        return s;
    }

    char* path = malloc(strlen(s) + 2);
    path[0] = '/';
    strcpy(path + 1, s);
    return path;
}

