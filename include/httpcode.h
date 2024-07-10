//
// Created by 666 on 10.07.2024.
//

#ifndef HTTPCODE_H
#define HTTPCODE_H

typedef enum HttpStatus {
    HTTP_STATUS_OK                    = 200, /**< OK */
    HTTP_STATUS_CREATED               = 201, /**< Created */
    HTTP_STATUS_ACCEPTED              = 202, /**< Accepted */
    HTTP_STATUS_NO_CONTENT            = 204, /**< No Content */
    HTTP_STATUS_MOVED_PERMANENTLY     = 301, /**< Moved Permanently */
    HTTP_STATUS_FOUND                 = 302, /**< Found */
    HTTP_STATUS_NOT_MODIFIED          = 304, /**< Not Modified */
    HTTP_STATUS_BAD_REQUEST           = 400, /**< Bad Request */
    HTTP_STATUS_UNAUTHORIZED          = 401, /**< Unauthorized */
    HTTP_STATUS_FORBIDDEN             = 403, /**< Forbidden */
    HTTP_STATUS_NOT_FOUND             = 404, /**< Not Found */
    HTTP_STATUS_METHOD_NOT_ALLOWED    = 405, /**< Method Not Allowed */
    HTTP_STATUS_INTERNAL_SERVER_ERROR = 500, /**< Internal Server Error */
    HTTP_STATUS_NOT_IMPLEMENTED       = 501, /**< Not Implemented */
    HTTP_STATUS_BAD_GATEWAY           = 502, /**< Bad Gateway */
    HTTP_STATUS_SERVICE_UNAVAILABLE   = 503, /**< Service Unavailable */
    HTTP_STATUS_UNKNOWN               = 0    /**< Unknown Status */
} HttpStatus;

char* FormatHttpStatus(HttpStatus status);

#endif //HTTPCODE_H
