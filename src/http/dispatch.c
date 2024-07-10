//
// Created by 666 on 07.07.2024.
//

#include "../../include/dispatch.h"

#include "intmap.h"
#include "log.h"
#include "strings.h"
#include "url.h"

Response* DefaultNotFoundHandler(Request* request) {
    return NewResponse(request, "Not found", 404, CONTENT_TYPE_TEXT_PLAIN);
}

Response* DefaultMethodNotAllowedHandler(Request* request) {
    return NewResponse(request, "Method not allowed", 405, CONTENT_TYPE_TEXT_PLAIN);
}

Dispatcher* NewDispatcher() {
    Dispatcher* dispatcher = malloc(sizeof(Dispatcher));
    if (dispatcher == NULL) {
        logError("Failed to allocate memory for Dispatcher");
        return NULL;
    }

    dispatcher->handlers = NewStringMap();
    dispatcher->notFoundHandler = DefaultNotFoundHandler;
    dispatcher->methodNotAllowedHandler = DefaultMethodNotAllowedHandler;

    return dispatcher;
}

DispatcherHandler* FindDispatcherHandler(Dispatcher* dispatcher, const Path path) {
    return StringMapGet(dispatcher->handlers, path);
}

void AddHandlerToDispatcher(Dispatcher* dispatcher, const Path path, const Method method, const Handler handler) {
    char* trimmed = trim(path);
    DispatcherHandler* dispatcherHandler = StringMapGet(dispatcher->handlers, trimmed);

    if (dispatcherHandler == NULL) {
        dispatcherHandler = malloc(sizeof(DispatcherHandler));
        if (dispatcherHandler == NULL) {
            logCritical("Failed to allocate memory for DispatcherHandler");
            free(trimmed);
            return;
        }

        dispatcherHandler->handlers = newIntMap(10);
        StringMapPut(dispatcher->handlers, trimmed, dispatcherHandler);
    }

    intMapPut(dispatcherHandler->handlers, method, handler);
    logCritical("Registered handler: %s %s", trimmed, MethodToString(method));
}

void ClearDispatcherHandlers(Dispatcher* dispatcher, const Path path) {
    StringMapRemove(dispatcher->handlers, path);
}

Response* Dispatch(Dispatcher* dispatcher, Request* request) {
    const DispatcherHandler* handler = StringMapGet(dispatcher->handlers, request->httpRequest->path);

    if (handler == NULL) {
        logCritical("Couldn't find handler associated with path: %s", request->httpRequest->path);
        logCritical("Fallback to not found handler");
        return dispatcher->notFoundHandler(request);
    }

    const Handler matchedHandler = intMapGet(handler->handlers, request->httpRequest->method);
    if (matchedHandler != NULL) {
        logInfo("Matched method: %s", MethodToString(request->httpRequest->method));
        return matchedHandler(request);
    }

    logCritical("Couldn't find associated handler with following method: %s", MethodToString(request->httpRequest->method));
    logCritical("Fallback to method not allowed handler");
    return dispatcher->methodNotAllowedHandler(request);
}

void DispatcherDestroy(Dispatcher* dispatcher) {
    StringMapDestroy(dispatcher->handlers);
    free(dispatcher);
}