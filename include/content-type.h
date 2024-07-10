//
// Created by 666 on 10.07.2024.
//

#ifndef CONTENT_TYPE_H
#define CONTENT_TYPE_H

typedef enum ContentType {
    CONTENT_TYPE_UNKNOWN,
    CONTENT_TYPE_JSON,
    CONTENT_TYPE_FORM_URLENCODED,
    CONTENT_TYPE_TEXT_HTML,
    CONTENT_TYPE_TEXT_PLAIN,
    CONTENT_TYPE_XML,
    CONTENT_TYPE_MULTIPART_FORM_DATA,
    CONTENT_TYPE_APPLICATION_OCTET_STREAM,
    CONTENT_TYPE_APPLICATION_JSON, // application/json; charset=utf-8
    CONTENT_TYPE_APPLICATION_XML,  // application/xml; charset=utf-8
    CONTENT_TYPE_IMAGE_PNG,        // image/png
    CONTENT_TYPE_IMAGE_JPEG,       // image/jpeg
} ContentType;

ContentType StringToContentType(const char* contentType);
char* FormatContentType(ContentType contentType);

#endif //CONTENT_TYPE_H
