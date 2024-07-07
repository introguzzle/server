#ifndef P_SERVER_H
#define P_SERVER_H

#include <winsock2.h>

#include "request.h"
#include "response.h"

#define PORT 8080
#define MAX_CLIENTS 30
#define FATAL 1

typedef struct Server {
    const SOCKET master;

} Server;

typedef Response* (*Handler)(Request*);

Request* acceptConnection(SOCKET clientSocket);
Response* handleRequest(const Request* request);
int sendResponse(Response* response);

struct WSAData initializeWSAData();
SOCKET createServerSocket();
void startServer();

#endif
