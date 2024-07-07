#ifndef RESPONSE_H
#define RESPONSE_H

#include <winsock2.h>
#include "map.h"
#include "request.h"

typedef struct Response {
    unsigned short status;
    const char*    statusText;
    Map*           headers;
    const char*    body;
    size_t         size;
    SOCKET         client;
} Response;

Response* newResponse(const Request* request, const char* body, unsigned short status);
int determineSize(Response* response);
char* createBuffer(Response* response);

void appendHeader(const Response* response, const char* key, const char* value);
void removeHeader(const Response* response, const char* key);

void freeResponse(Response* response);

#endif // RESPONSE_H
