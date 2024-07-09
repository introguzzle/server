#include "request.h"

#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "strings.h"

const unsigned int METHOD_ALL = METHOD_GET | METHOD_POST | METHOD_PUT | METHOD_PATCH | METHOD_DELETE;
const char* methodToString(const Method method) {
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

HttpRequest* parseHttpRequest(const char* data) {
    HttpRequest* request = new(HttpRequest);
    size_t size;
    char** lines = split(data, "\n", &size);

    if (size > 0) {
        const char *firstLine = lines[0];
        size_t tokensCount;
        char** tokens = split(firstLine, " ", &tokensCount);

        request->content = strdup(firstLine);

        char* path = trim(tokens[1]);

        if (tokensCount >= 3) {
            request->method = parseMethod(tokens[0]);
            request->path = path;
            request->protocol = strdup(tokens[2]);
        }

        freeSplit(tokens);
    } else {
        free(request);
        return NULL;
    }

    freeSplit(lines);
    return request;
}

void logRequest(const Request* request) {
    const char* method = methodToString(request->httpRequest->method);
    const char* path = request->httpRequest->path;
    const char* ip = request->clientIp;
    const unsigned short port = request->clientPort;

    logInfo("Received request: %s %s \nIP-address: %s \nPort: %d", method, path, ip, port);
    logInfo("Request headers");
}

Method parseMethod(const char* data) {
    if (strcmp(data, "GET") == 0)
        return METHOD_GET;

    if (strcmp(data, "POST") == 0)
        return METHOD_POST;

    if (strcmp(data, "PUT") == 0)
        return METHOD_PUT;

    if (strcmp(data, "PATCH") == 0)
        return METHOD_PATCH;

    if (strcmp(data, "DELETE") == 0)
        return METHOD_DELETE;

    return METHOD_UNKNOWN;
}

StringMap* parseHeaders(const char* data) {
    StringMap* map = newStringMap(100);

    if (map == NULL) {
        return NULL;
    }

    size_t size;
    char** lines = split(data, "\n", &size);

    if (size < 1) {
        free(map);
        free(lines);
        return NULL;
    }

    size_t i = 0;
    while (lines[i] != NULL) {
        size_t t;
        char** splitted = split(lines[i], ":", &t);

        if (t == 2 && splitted != NULL) {
            char* key = trim(splitted[0]);
            char* value = trim(splitted[1]);

            stringMapPut(map, key, value);
        }

        i++;
        free(splitted);
    }

    return map;
}

Request* parseRequest(const char* data) {
    Request* request = new(Request);

    request->httpRequest = parseHttpRequest(data);
    request->headers = parseHeaders(data);

    return request;
}

void freeHttpRequest(HttpRequest* request) {
    if (request != NULL) {
        if (request->content != NULL) {
            free((void*)request->content);
        }
        free(request);
    }
}

void freeRequest(Request* request) {
    if (request != NULL) {
        freeHttpRequest(request->httpRequest);
        freeStringMap(request->headers);
        free(request);
    }
}
