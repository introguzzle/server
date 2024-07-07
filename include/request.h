
#ifndef REQUEST_H
#define REQUEST_H
#include <winsock2.h>
#include <map.h>
#define new(x) ((x*)malloc(sizeof(x)))
static const char* METHOD_UNKNOWN_ALIAS = "UNKNOWN";

typedef enum Method {
    METHOD_GET,
    METHOD_POST,
    METHOD_PUT,
    METHOD_PATCH,
    METHOD_DELETE,
    METHOD_UNKNOWN,
} Method;

typedef struct HttpRequest {
    Method       method;
    const char*  path;
    const char*  protocol;
    const char * content;
} HttpRequest;

typedef struct Request {
    HttpRequest* httpRequest; // HTTP метод
    Map*         headers;     // Headers
    SOCKET       client;      // Client socket
} Request;

const char* methodToString(Method method);
HttpRequest* parseHttpRequest(const char *data);
Method parseMethod(const char* data);
Request* parseRequest(const char *data);
void logRequest(const Request* request);
void freeRequest(Request* request);

#endif //REQUEST_H
