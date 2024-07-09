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

Dispatcher* newDispatcher();
void addHandler(Dispatcher* dispatcher, Path path, Method method, Handler handler);
void clearHandlers(Dispatcher* dispatcher, Path path);
Response* dispatch(Dispatcher* dispatcher, Request* request);
void freeDispatcher(Dispatcher* dispatcher);
DispatcherHandler* findDispatcherHandler(Dispatcher* dispatcher, Path path);


#endif //DISPATCH_H
