//
// Created by 666 on 10.07.2024.
//

#include "content-type.h"
#include "string.h"

char* FormatContentType(const ContentType contentType) {
    switch (contentType) {
        case CONTENT_TYPE_TEXT_HTML:                return "text/html; charset=utf-8";
        case CONTENT_TYPE_TEXT_PLAIN:               return "text/plain; charset=utf-8";
        case CONTENT_TYPE_APPLICATION_JSON:         return "application/json; charset=utf-8";
        case CONTENT_TYPE_APPLICATION_XML:          return "application/xml; charset=utf-8";
        case CONTENT_TYPE_FORM_URLENCODED:          return "application/x-www-form-urlencoded";
        case CONTENT_TYPE_MULTIPART_FORM_DATA:      return "multipart/form-data";
        case CONTENT_TYPE_APPLICATION_OCTET_STREAM: return "application/octet-stream";
        case CONTENT_TYPE_IMAGE_PNG:                return "image/png";
        case CONTENT_TYPE_IMAGE_JPEG:               return "image/jpeg";
        default:                                    return "text/plain; charset=utf-8";
    }
}

ContentType StringToContentType(const char* contentType) {
    if (contentType == NULL) {
        return CONTENT_TYPE_UNKNOWN;
    }

    if (strcmp(contentType, "application/json") == 0) {
        return CONTENT_TYPE_JSON;
    }

    if (strcmp(contentType, "application/x-www-form-urlencoded") == 0) {
        return CONTENT_TYPE_FORM_URLENCODED;
    }

    if (strcmp(contentType, "text/html") == 0) {
        return CONTENT_TYPE_TEXT_HTML;
    }

    if (strcmp(contentType, "text/plain") == 0) {
        return CONTENT_TYPE_TEXT_PLAIN;
    }

    if (strcmp(contentType, "application/xml") == 0) {
        return CONTENT_TYPE_XML;
    }

    if (strcmp(contentType, "multipart/form-data") == 0) {
        return CONTENT_TYPE_MULTIPART_FORM_DATA;
    }

    if (strcmp(contentType, "application/octet-stream") == 0) {
        return CONTENT_TYPE_APPLICATION_OCTET_STREAM;
    }

    return CONTENT_TYPE_UNKNOWN;
}
