#include <stdio.h>

#include "server.h"
#include "dispatch.h"
#include "request.h"
#include "json.h"
#include "strings.h"

Response* get(Request* request) {
    return NewResponse(request, "1337 GET", 200, CONTENT_TYPE_TEXT_PLAIN);
}

Response* post(Request* request) {
    const char* encoded = NULL;

    if (request->postParams != NULL && request->postParams->size > 0) {
        encoded = JSONEncode(request->postParams, 2048);
    }

    return NewResponse(request, encoded != NULL ? encoded : "1337 POST", 200, CONTENT_TYPE_TEXT_PLAIN);
}

int main(int argc, char** argv) {
    Server* server = ServerInitialize(0, 0);
    Dispatcher* dispatcher = NewDispatcher();

    server->dispatcher = dispatcher;
    const Path path = newPath("api/v1/clowns");

    AddHandlerToDispatcher(dispatcher, path, METHOD_GET, get);
    AddHandlerToDispatcher(dispatcher, path, METHOD_POST, post);

    StartServer(server);

    return 0;
}
