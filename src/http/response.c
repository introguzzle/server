#include "../../include/response.h"

#include <stdio.h>

#include "../../include/log.h"
#include "../../include/request.h"
#include "../../include/content-type.h"

static const char* DELIMITER = "\r\n";

Response* NewResponse(const Request* request, const char* body, const HttpStatus status, const ContentType contentType) {
    Response* response = malloc(sizeof(Response));

    if (response == NULL) {
        logError("Memory allocation failed for Response.");
        return NULL;
    }

    response->headers = NewStringMap();

    if (response->headers == NULL) {
        logError("Memory allocation failed for Map.");
        free(response);
        return NULL;
    }

    response->status = status;
    response->statusText = FormatHttpStatus(status);
    response->body = body;
    response->client = request->client;

    AppendHeader(response, "Content-Type", FormatContentType(contentType));

    char contentLength[256];
    snprintf(contentLength, sizeof(contentLength), "%lu", strlen(body));
    AppendHeader(response, "Content-Length", contentLength);
    AppendHeader(response, "Connection", "close");

    return response;
}

char* formatStatusLine(const Response* response) {
    const char* statusFormat = "HTTP/1.1 %d %s\r\n";
    char *statusBuffer = malloc(strlen(statusFormat) + sizeof(response->status) + strlen(response->statusText));

    if (statusBuffer == NULL) {
        logError("Failed to allocate memory for status line");
        return NULL;
    }

    sprintf(statusBuffer, statusFormat, response->status, response->statusText);
    return statusBuffer;
}

static int DetermineSize(Response* response) {
    size_t headersLength = 0;
    const size_t bodyLength = strlen(response->body);

    for (size_t i = 0; i < response->headers->size; i++) {
        const char *key = response->headers->items[i].key;
        const char* value = response->headers->items[i].value;
        headersLength += strlen(key) + strlen(value) + 4;
    }

    response->size = headersLength + bodyLength + 4 + 1;
    return response->size;
}

char* CreateBuffer(Response* response) {
    DetermineSize(response);
    const char* statusLine = formatStatusLine(response);
    char* buffer = malloc(response->size + strlen(statusLine));

    if (buffer == NULL) {
        logError("Memory allocation failed for buffer.");
        return NULL;
    }

    logInfo("Buffer initialized");

    buffer[0] = '\0';
    strcat(buffer, statusLine);

    for (size_t i = 0; i < response->headers->size; i++) {
        const char *key = response->headers->items[i].key;
        const char* value = response->headers->items[i].value;

        strcat(buffer, key);
        strcat(buffer, ": ");
        strcat(buffer, value);
        strcat(buffer, DELIMITER);
    }

    strcat(buffer, DELIMITER);
    strcat(buffer, response->body);

    logInfo("Buffer to send: \n%s", buffer);
    return buffer;
}

void AppendHeader(const Response* response, const char* key, const char* value) {
    char* copiedKey = strdup(key);
    char* copiedValue = strdup(value);

    if (copiedKey == NULL || copiedValue == NULL) {
        logError("Memory allocation failed for header.");
        free(copiedKey);
        free(copiedValue);
        return;
    }

    StringMapPut(response->headers, copiedKey, copiedValue);
}

void RemoveHeader(const Response* response, const char* key) {
    StringMapRemove(response->headers, key);
}

void ResponseDestroy(Response* response) {
    StringMapDestroy(response->headers);
    free(response);
}
