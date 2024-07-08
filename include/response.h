#ifndef RESPONSE_H
#define RESPONSE_H

#include <winsock2.h>
#include "request.h"

typedef enum ContentType {
    CONTENT_TYPE_TEXT_HTML,        // text/html; charset=utf-8
    CONTENT_TYPE_TEXT_PLAIN,       // text/plain; charset=utf-8
    CONTENT_TYPE_APPLICATION_JSON, // application/json; charset=utf-8
    CONTENT_TYPE_APPLICATION_XML,  // application/xml; charset=utf-8
    CONTENT_TYPE_IMAGE_PNG,        // image/png
    CONTENT_TYPE_IMAGE_JPEG,       // image/jpeg
    CONTENT_TYPE_UNKNOWN           // Unknown content type
} ContentType;

typedef struct Response {
    unsigned short status;
    const char*    statusText;
    StringMap*     headers;
    const char*    body;
    size_t         size;
    SOCKET         client;
} Response;

Response* newResponse(const Request* request, const char* body, unsigned short status, ContentType contentType);
int determineSize(Response* response);
char* createBuffer(Response* response);

void appendHeader(const Response* response, const char* key, const char* value);
void removeHeader(const Response* response, const char* key);

void freeResponse(Response* response);

#endif // RESPONSE_H
