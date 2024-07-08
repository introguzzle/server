#include "../../include/response.h"

#include <stdio.h>

#include "../../include/log.h"
#include "../../include/request.h"

static const char* DELIMITER = "\r\n";
static const char* formatStatusText(const unsigned short status) {
    switch (status) {
        case 200: return "OK";
        case 201: return "Created";
        case 202: return "Accepted";
        case 204: return "No Content";
        case 301: return "Moved Permanently";
        case 302: return "Found";
        case 304: return "Not Modified";
        case 400: return "Bad Request";
        case 401: return "Unauthorized";
        case 403: return "Forbidden";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        case 500: return "Internal Server Error";
        case 501: return "Not Implemented";
        case 502: return "Bad Gateway";
        case 503: return "Service Unavailable";
        default:  return "Unknown Status";
    }
}

static char* formatContentType(const ContentType contentType) {
    switch (contentType) {
        case CONTENT_TYPE_TEXT_HTML:        return "text/html; charset=utf-8";
        case CONTENT_TYPE_TEXT_PLAIN:       return "text/plain; charset=utf-8";
        case CONTENT_TYPE_APPLICATION_JSON: return "application/json; charset=utf-8";
        case CONTENT_TYPE_APPLICATION_XML:  return "application/xml; charset=utf-8";
        case CONTENT_TYPE_IMAGE_PNG:        return "image/png";
        case CONTENT_TYPE_IMAGE_JPEG:       return "image/jpeg";
        default:                            return "text/plain; charset=utf-8";
    }
}

Response* newResponse(const Request* request, const char* body, const unsigned short status, const ContentType contentType) {
    Response* response = malloc(sizeof(Response));

    if (response == NULL) {
        logError("Memory allocation failed for Response.");
        return NULL;
    }

    response->headers = newStringMap(32);

    if (response->headers == NULL) {
        logError("Memory allocation failed for Map.");
        free(response);
        return NULL;
    }

    response->status = status;
    response->statusText = formatStatusText(status);
    response->body = body;
    response->client = request->client;

    appendHeader(response, "Content-Type", formatContentType(contentType));

    char contentLength[256];
    snprintf(contentLength, sizeof(contentLength), "%lu", strlen(body));
    appendHeader(response, "Content-Length", contentLength);
    appendHeader(response, "Connection", "close");

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

char* createBuffer(Response* response) {
    determineSize(response);
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

int determineSize(Response* response) {
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

void appendHeader(const Response* response, const char* key, const char* value) {
    char* copiedKey = strdup(key);
    char* copiedValue = strdup(value);

    if (copiedKey == NULL || copiedValue == NULL) {
        logError("Memory allocation failed for header.");
        free(copiedKey);
        free(copiedValue);
        return;
    }

    stringMapPut(response->headers, copiedKey, copiedValue);
}

void removeHeader(const Response* response, const char* key) {
    stringMapRemove(response->headers, key);
}

void freeResponse(Response* response) {
    freeStringMap(response->headers);
    free(response);
}
