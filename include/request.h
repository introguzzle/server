
#ifndef REQUEST_H
#define REQUEST_H
#include <winsock2.h>
#include "stringmap.h"
#define new(x) ((x*)malloc(sizeof(x)))
static const char* METHOD_UNKNOWN_ALIAS = "UNKNOWN";

typedef enum Method {
    METHOD_GET     = (unsigned int) 1 << 0,
    METHOD_POST    = (unsigned int) 1 << 1,
    METHOD_PUT     = (unsigned int) 1 << 2,
    METHOD_PATCH   = (unsigned int) 1 << 3,
    METHOD_DELETE  = (unsigned int) 1 << 4,
    METHOD_UNKNOWN = (unsigned int) 1 << 5,
} Method;

typedef struct HttpRequest {
    Method       method;
    const char*  path;
    const char*  protocol;
    const char * content;
} HttpRequest;

typedef struct Request {
    HttpRequest*   httpRequest; // HTTP метод
    StringMap*     headers;     // Headers
    SOCKET         client;      // Client socket
    const char*    clientIp;    // IP адрес клиента
    unsigned short clientPort;  // Порт клиента
} Request;

extern const unsigned int METHOD_ALL;

const char* methodToString(Method method);
HttpRequest* parseHttpRequest(const char *data);
Method parseMethod(const char* data);
Request* parseRequest(const char *data);
void logRequest(const Request* request);
void freeRequest(Request* request);

#endif //REQUEST_H
