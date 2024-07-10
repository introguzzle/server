#include "request.h"

#include <stdlib.h>
#include <string.h>

#include "iterable.h"
#include "json.h"
#include "log.h"
#include "strings.h"
#include "url.h"

static Protocol* ParseProtocol(const char* protocolData) {
    // HTTP/1.1
    const Vector* tokens = split(protocolData, "/");

    Protocol* protocol = malloc(sizeof(Protocol));
    protocol->content = protocolData;
    protocol->protocol = strdup(tokens->items[0]);

    const Vector* version = split(tokens->items[1], ".");

    protocol->major = (size_t) atoi(version->items[0]);
    protocol->minor = (size_t) atoi(version->items[0]);

    return protocol;
}

static HttpRequest* ParseHttpRequest(const char* data) {
    HttpRequest* request = malloc(sizeof(HttpRequest));
    if (request == NULL) {
        return NULL;
    }

    Vector* splitted = split(data, "\n");
    if (splitted == NULL || splitted->size == 0) {
        free(request);
        return NULL;
    }

    const char* firstLine = splitted->items[0];
    Vector* tokens = split(firstLine, " ");
    if (tokens == NULL || tokens->size < 3) {
        free(request);
        VectorDestroy(splitted);
        return NULL;
    }

    request->method = ParseMethod(tokens->items[0]);
    request->path = strdup(trim(tokens->items[1]));
    request->protocol = ParseProtocol(tokens->items[2]);

    request->content = strdup(firstLine);

    VectorDestroy(tokens);
    VectorDestroy(splitted);

    return request;
}

void LogRequest(const Request* request) {
    const char* method = MethodToString(request->httpRequest->method);
    const char* path = request->httpRequest->path;
    const char* ip = request->clientIp;
    const unsigned short port = request->clientPort;
    const Protocol* protocol = request->httpRequest->protocol;

    const char* format = concat("Received request: %s %s \nIP-address: %s \nPort: %d", " Protocol: %s", NULL);
    logInfo(format, method, path, ip, port, protocol->content);

    StringPair pair;
    foreach(request->queryParams, pair) {
        logCritical("Key:%s;Value:%s", pair.key, pair.value);
    }
}

static StringMap* ParseRequestHeaders(const char* data) {
    StringMap* map = NewStringMap(100);
    if (map == NULL) {
        return NULL;
    }

    Vector* lines = split(data, "\n");

    if (lines->size < 1) {
        free(map);
        VectorDestroy(lines);
        return NULL;
    }

    for (size_t i = 0; i < lines->size; ++i) {
        Vector* header = split(lines->items[i], ":");

        if (header != NULL && header->size == 2) {
            const char* key = trim(header->items[0]);
            char* value = trim(header->items[1]);

            StringMapPut(map, key, value);
        }

        VectorDestroy(header);
    }

    return map;
}

Request* ParseRequest(const char* data) {
    Request* request = new(Request);

    request->httpRequest = ParseHttpRequest(data);
    request->headers = ParseRequestHeaders(data);
    request->queryParams = ParseQueryParameters(request->httpRequest->path);

    int isJson = false;

    // POST / HTTP/1.1
    // Host: foo.com
    // Content-Type: application/x-www-form-urlencoded
    // Content-Length: 13
    //
    // say=Hi&to=Mom

    const char* bodyStart = strstr(data, "\r\n\r\n");
    if (bodyStart != NULL) {
        bodyStart += 4;
    } else {
        bodyStart = data + strlen(data);
    }

    request->body = strdup(bodyStart);

    const char* contentType = StringMapGet(request->headers, "Content-Type");

    if (strcmp(contentType, "application/json") == 0) {
        isJson = 1;
    }

    logCritical("Content-Type:%s", contentType);
    logCritical("request->httpRequest->method:%d", request->httpRequest->method);
    logCritical("METHOD_POST:%d", METHOD_POST);
    logCritical("isJson:%d", isJson);

    if (request->httpRequest->method == METHOD_POST && isJson) {
        if (request->body && strlen(request->body) > 0) {
            request->postParams = JSONDecode(request->body);
        }

    } else {
        request->postParams = NewStringMap();
    }

    return request;
}

static void HttpRequestDestroy(HttpRequest* request) {
    if (request != NULL) {
        if (request->content != NULL) {
            free((void*)request->content);
        }
        free(request);
    }
}

void RequestDestroy(Request* request) {
    if (request != NULL) {
        HttpRequestDestroy(request->httpRequest);
        StringMapDestroy(request->headers);
        StringMapDestroy(request->queryParams);
        StringMapDestroy(request->postParams);
        free(request);
    }
}
