//
// Created by 666 on 07.07.2024.
//

#include "../../include/dispatch.h"

#include "intmap.h"
#include "log.h"
#include "strings.h"
#include "url.h"

Response* defaultNotFoundHandler(Request* request) {
    return newResponse(request, "Not found", 404, CONTENT_TYPE_TEXT_PLAIN);
}

Response* defaultMethodNotAllowedHandler(Request* request) {
    return newResponse(request, "Method not allowed", 405, CONTENT_TYPE_TEXT_PLAIN);
}

Dispatcher* newDispatcher() {
    Dispatcher* dispatcher = malloc(sizeof(Dispatcher));
    if (dispatcher == NULL) {
        logError("Failed to allocate memory for Dispatcher");
        return NULL;
    }

    dispatcher->handlers = newStringMap(10);
    dispatcher->notFoundHandler = defaultNotFoundHandler;
    dispatcher->methodNotAllowedHandler = defaultMethodNotAllowedHandler;

    return dispatcher;
}

DispatcherHandler* findDispatcherHandler(Dispatcher* dispatcher, const Path path) {
    return stringMapGet(dispatcher->handlers, path);
}

void addHandler(Dispatcher* dispatcher, const Path path, const Method method, const Handler handler) {
    char* trimmed = trim(path);
    DispatcherHandler* dispatcherHandler = stringMapGet(dispatcher->handlers, trimmed);

    if (dispatcherHandler == NULL) {
        dispatcherHandler = malloc(sizeof(DispatcherHandler));
        if (dispatcherHandler == NULL) {
            logCritical("Failed to allocate memory for DispatcherHandler");
            free(trimmed);
            return;
        }

        dispatcherHandler->handlers = newIntMap(10);
        stringMapPut(dispatcher->handlers, trimmed, dispatcherHandler);
    }

    intMapPut(dispatcherHandler->handlers, method, handler);
    logCritical("Registered handler: %s %s", trimmed, methodToString(method));
}

void clearHandlers(Dispatcher* dispatcher, const Path path) {
    stringMapRemove(dispatcher->handlers, path);
}

Response* dispatch(Dispatcher* dispatcher, Request* request) {
    const DispatcherHandler* handler = stringMapGet(dispatcher->handlers, request->httpRequest->path);

    if (handler == NULL) {
        logCritical("Couldn't find handler associated with path: %s", request->httpRequest->path);
        logCritical("Fallback to not found handler");
        return dispatcher->notFoundHandler(request);
    }

    const Handler matchedHandler = intMapGet(handler->handlers, request->httpRequest->method);
    if (matchedHandler != NULL) {
        logInfo("Matched method: %s", methodToString(request->httpRequest->method));
        return matchedHandler(request);
    }

    logCritical("Couldn't find associated handler with following method: %s", methodToString(request->httpRequest->method));
    logCritical("Fallback to method not allowed handler");
    return dispatcher->methodNotAllowedHandler(request);
}

void freeDispatcher(Dispatcher* dispatcher) {
    freeStringMap(dispatcher->handlers);
    free(dispatcher);
}