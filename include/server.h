#ifndef P_SERVER_H
#define P_SERVER_H

#include <winsock2.h>

#include "request.h"
#include "response.h"

#define PORT 8080
#define FATAL 1

Request* acceptConnection();
Response* handleRequest(const Request* request);
int sendResponse(Response* response);

struct WSAData initializeWSAData();
SOCKET createServerSocket();
void startServer();

#endif
