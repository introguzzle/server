#ifndef P_SERVER_H
#define P_SERVER_H

#include <winsock2.h>
#include <signal.h>

#include "dispatch.h"
#include "request.h"
#include "response.h"

#define DEFAULT_PORT 8080
#define DEFAULT_MAX_CLIENTS 30
#define FATAL 1

static volatile sig_atomic_t __RUNNING__ = 1;
static void signalHandler(const int signum) {
    (void)signum;
    __RUNNING__ = 0;
}

typedef struct Server {
    SOCKET                master;
    int                   socketOptions;

    struct sockaddr_in    address;
    int                   port;
    int*                  clients;
    size_t                maxClients;

    Dispatcher*           dispatcher;

    CRITICAL_SECTION      mutex;
} Server;

Request* ServerAcceptConnection(Server* server, SOCKET clientSocket);
Response* ServerHandleRequest(Server* server, Request* request);
int ServerSendResponse(Server* server, Response* response);

void StartServer(Server* server);
Server* ServerInitialize(SOCKET master, unsigned short port, size_t maxClients);

#endif
