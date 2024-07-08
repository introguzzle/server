#include <stdio.h>

#include "server.h"
#include "dispatch.h"
#include "request.h"

Response* get(Request* request) {
    return newResponse(request, "1337 GET", 200, CONTENT_TYPE_TEXT_PLAIN);
}

Response* post(Request* request) {
    return newResponse(request, "1337 POST", 200, CONTENT_TYPE_TEXT_PLAIN);
}

int main() {
    Server* server = initializeServer(0, 0);
    Dispatcher* dispatcher = newDispatcher();

    server->dispatcher = dispatcher;

    addHandler(dispatcher, "/api/v1/clowns", METHOD_GET, get);
    addHandler(dispatcher, "/api/v1/clowns", METHOD_POST, post);

    startServer(server);
    return 0;
}
