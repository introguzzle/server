#ifndef RESPONSE_H
#define RESPONSE_H

#include <winsock2.h>

#include "content-type.h"
#include "httpcode.h"
#include "request.h"

typedef struct Response {
    HttpStatus     status;
    const char*    statusText;
    StringMap*     headers;
    const char*    body;
    size_t         size;
    SOCKET         client;
} Response;

Response* NewResponse(const Request* request, const char* body, HttpStatus status, ContentType contentType);
Response* NewHTMLResponse(const Request* request, const char* name, HttpStatus status);
Response* ResponseWithData(Response* response, const StringMap* data);

char* CreateBuffer(Response* response);

void AppendHeader(const Response* response, const char* key, const char* value);
void RemoveHeader(const Response* response, const char* key);

void ResponseDestroy(Response* response);

#endif // RESPONSE_H
