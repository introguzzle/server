//
// Created by 666 on 10.07.2024.
//

#include "method.h"

#include <string.h>

const char* METHOD_UNKNOWN_ALIAS = "UNKNOWN";
const unsigned int METHOD_ALL = METHOD_GET | METHOD_POST | METHOD_PUT | METHOD_PATCH | METHOD_DELETE;

const char* MethodToString(const Method method) {
    switch (method) {
        case METHOD_GET:
            return "GET";
        case METHOD_POST:
            return "POST";
        case METHOD_PUT:
            return "PUT";
        case METHOD_PATCH:
            return "PATCH";
        case METHOD_DELETE:
            return "DELETE";
        default:
            return METHOD_UNKNOWN_ALIAS;
    }
}

Method ParseMethod(const char* methodData) {
    if (strcmp(methodData, "GET") == 0)
        return METHOD_GET;

    if (strcmp(methodData, "POST") == 0)
        return METHOD_POST;

    if (strcmp(methodData, "PUT") == 0)
        return METHOD_PUT;

    if (strcmp(methodData, "PATCH") == 0)
        return METHOD_PATCH;

    if (strcmp(methodData, "DELETE") == 0)
        return METHOD_DELETE;

    return METHOD_UNKNOWN;
}
