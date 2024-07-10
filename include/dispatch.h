//
// Created by 666 on 07.07.2024.
//

#ifndef DISPATCH_H
#define DISPATCH_H
#include "intmap.h"
#include "request.h"
#include "response.h"
#include "url.h"

typedef Response* (*Handler)(Request*);
typedef struct DispatcherHandler {
    IntMap* handlers; // IntMap*<unsigned int(Method), Handler>;
} DispatcherHandler;

typedef struct Dispatcher {
    StringMap* handlers; // StringMap*<const char*(Path), DispatcherHandler>
    Handler    notFoundHandler;
    Handler    internalServerErrorHandler;
    Handler    methodNotAllowedHandler;
} Dispatcher;

Dispatcher* NewDispatcher();
void AddHandlerToDispatcher(Dispatcher* dispatcher, Path path, Method method, Handler handler);
void ClearDispatcherHandlers(Dispatcher* dispatcher, Path path);
Response* Dispatch(Dispatcher* dispatcher, Request* request);
void DispatcherDestroy(Dispatcher* dispatcher);
DispatcherHandler* FindDispatcherHandler(Dispatcher* dispatcher, Path path);


#endif //DISPATCH_H
