#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <signal.h>

#include "../../include/log.h"
#include "../../include/server.h"
#include "../../include/request.h"

static volatile sig_atomic_t __RUNNING__ = 1;
static void signalHandler(const int signum) {
    (void)signum;
    __RUNNING__ = 0;
}

SOCKET __MASTER__;
struct sockaddr_in __SERVER_ADDRESS__;
int __CLIENTS__[MAX_CLIENTS];
int __OPTION__ = 1;

Handler* handlers;

struct WSAData initializeWSAData() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        logCritical("WSAStartup failed: %d", WSAGetLastError());
        exit(FATAL);
    }

    logInfo("Winsock initialized.");
    return wsaData;
}

SOCKET createServerSocket() {
    initializeWSAData();
    __MASTER__ = socket(AF_INET, SOCK_STREAM, 0);
    if (__MASTER__ == INVALID_SOCKET) {
        logCritical("Socket creation failed.");
        WSACleanup();
        exit(FATAL);
    }

    for (int i = 0; i < MAX_CLIENTS; i++) {
        __CLIENTS__[i] = 0;
    }

    if (setsockopt(__MASTER__, SOL_SOCKET, SO_REUSEADDR, (char *)&__OPTION__, sizeof(__OPTION__)) < 0) {
        logError("Setting socket opt failed");
        closesocket(__MASTER__);
        WSACleanup();
        exit(FATAL);
    }

    __SERVER_ADDRESS__.sin_family = AF_INET;
    __SERVER_ADDRESS__.sin_addr.s_addr = INADDR_ANY;
    __SERVER_ADDRESS__.sin_port = htons(PORT);

    if (bind(__MASTER__, (struct sockaddr*)&__SERVER_ADDRESS__, sizeof(__SERVER_ADDRESS__)) == SOCKET_ERROR) {
        logCritical("Bind failed.");
        closesocket(__MASTER__);
        WSACleanup();
        exit(FATAL);
    }

    if (listen(__MASTER__, SOMAXCONN) == SOCKET_ERROR) {
        logCritical("Listen failed.");
        closesocket(__MASTER__);
        WSACleanup();
        exit(FATAL);
    }

    logInfo("Socket bound to port %d", PORT);

    return __MASTER__;
}

Request* acceptConnection(const SOCKET clientSocket) {
    char incoming[1024];
    const int bytesReceived = recv(clientSocket, incoming, sizeof(incoming) - 1, 0);

    if (bytesReceived <= 0) {
        return NULL;
    }

    incoming[bytesReceived] = '\0';

    Request* request = parseRequest(incoming);
    request->client = clientSocket;

    return request;
}

Response* handleRequest(const Request* request) {
    Response* response = newResponse(request, request->httpRequest->path, 200);
    return response;
}

int sendResponse(Response* response) {
    char* buffer = createBuffer(response);
    const int bytesSent = send(response->client, buffer, strlen(buffer), 0);

    free(buffer);
    return bytesSent;
}

void startServer() {
    __MASTER__ = createServerSocket();
    logInfo("Listening for connections... Socket: %d", __MASTER__);

    signal(SIGINT, signalHandler);
    fd_set readfds;
    while (__RUNNING__) {
        FD_ZERO(&readfds);
        FD_SET(__MASTER__, &readfds);

        int maxSocketDescriptor = __MASTER__;

        for (int i = 0; i < MAX_CLIENTS; i++) {
            const SOCKET clientSocket = __CLIENTS__[i];
            if (clientSocket > 0) {
                FD_SET(clientSocket, &readfds);
                logInfo("FD_SET: %d", clientSocket);
            }

            if (clientSocket > maxSocketDescriptor) {
                maxSocketDescriptor = clientSocket;
            }
        }

        const int activity = select(maxSocketDescriptor + 1, &readfds, NULL, NULL, NULL);

        if (activity < 0 && WSAGetLastError() != WSAEINTR) {
            logError("Select error");
            break;
        }

        if (FD_ISSET(__MASTER__, &readfds)) {
            const SOCKET newSocket = accept(__MASTER__, NULL, NULL);
            if (newSocket == INVALID_SOCKET) {
                logError("Accept failed");
                continue;
            }

            logInfo("New connection, socket is %d", newSocket);

            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (__CLIENTS__[i] == 0) {
                    __CLIENTS__[i] = newSocket;
                    break;
                }
            }
        }

        for (int i = 0; i < MAX_CLIENTS; i++) {
            const SOCKET clientSocket = __CLIENTS__[i];
            if (FD_ISSET(clientSocket, &readfds)) {
                Request* request = acceptConnection(clientSocket);
                if (request == NULL) {
                    closesocket(clientSocket);
                    __CLIENTS__[i] = 0;
                    continue;
                }

                logRequest(request);

                Response* response = handleRequest(request);
                logInfo("Response created");
                sendResponse(response);
                logInfo("Response sent");

                freeRequest(request);
                freeResponse(response);
            }
        }
    }

    closesocket(__MASTER__);
    WSACleanup();
}
