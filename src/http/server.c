#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <signal.h>

#include "../../include/log.h"
#include "../../include/server.h"
#include "../../include/request.h"

struct WSAData initializeWSAData() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        logCritical("WSAStartup failed: %d", WSAGetLastError());
        exit(FATAL);
    }

    logInfo("Winsock initialized. \nDescription: %s. \nSystem status: %s", wsaData.szDescription, wsaData.szSystemStatus);
    return wsaData;
}

Request* acceptConnection(Server* server, const SOCKET clientSocket) {
    char incoming[1024];
    const int bytesReceived = recv(clientSocket, incoming, sizeof(incoming) - 1, 0);

    if (bytesReceived <= 0) {
        return NULL;
    }

    incoming[bytesReceived] = '\0';

    Request* request = parseRequest(incoming);
    request->client = clientSocket;

    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    const int ret = getpeername(clientSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);

    if (ret == 0) {
        request->clientIp = inet_ntoa(clientAddr.sin_addr);
        request->clientPort = ntohs(clientAddr.sin_port);
        return request;
    }

    logError("Failed to get client address.");
    request->clientIp = "UNKNOWN";
    request->clientPort = 0;

    return request;
}

Response* handleRequest(Server* server, Request* request) {
    Response* response = dispatch(server->dispatcher, request);
    return response;
}

int sendResponse(Server* server, Response* response) {
    char* buffer = createBuffer(response);
    const int bytesSent = send(response->client, buffer, strlen(buffer), 0);

    free(buffer);
    return bytesSent;
}

Server* initializeServer(const SOCKET master, const unsigned short port) {
    initializeWSAData();
    Server* server = malloc(sizeof(Server));

    if (master == 0) {
        server->master = socket(AF_INET, SOCK_STREAM, 0);
    } else {
        server->master = master;
    }

    if (port == 0) {
        server->port = DEFAULT_PORT;
    } else {
        server->port = port;
    }

    if (server->master == INVALID_SOCKET) {
        logCritical("Socket creation failed.");
        WSACleanup();
        exit(FATAL);
    }

    if (setsockopt(server->master, SOL_SOCKET, SO_REUSEADDR, (char *)&server->socketOptions, sizeof(server->socketOptions)) < 0) {
        logError("Setting socket options failed");
        closesocket(server->master);
        WSACleanup();
        exit(FATAL);
    }

    for (int i = 0; i < DEFAULT_MAX_CLIENTS; i++) {
        server->clients[i] = 0;
    }

    server->address.sin_family = AF_INET;
    server->address.sin_addr.s_addr = INADDR_ANY;
    server->address.sin_port = htons(DEFAULT_PORT);

    if (bind(server->master, (struct sockaddr*)&server->address, sizeof(server->address)) == SOCKET_ERROR) {
        logCritical("Bind failed.");
        closesocket(server->master);
        WSACleanup();
        exit(FATAL);
    }

    if (listen(server->master, SOMAXCONN) == SOCKET_ERROR) {
        logCritical("Listen failed.");
        closesocket(server->master);
        WSACleanup();
        exit(FATAL);
    }

    logInfo("Socket bound to port %d", server->port);

    server->socketOptions = 1;
    InitializeCriticalSection(&server->mutex);

    return server;
}

void startServer(Server* server) {
    logInfo("Listening for connections... Socket: %d", server->master);

    signal(SIGINT, signalHandler);
    fd_set set;
    while (__RUNNING__) {
        FD_ZERO(&set);
        FD_SET(server->master, &set);

        int maxSocketDescriptor = server->master;

        for (int i = 0; i < DEFAULT_MAX_CLIENTS; i++) {
            const SOCKET clientSocket = server->clients[i];
            if (clientSocket > 0) {
                FD_SET(clientSocket, &set);
                logInfo("FD_SET: %d", clientSocket);
            }

            if (clientSocket > maxSocketDescriptor) {
                maxSocketDescriptor = clientSocket;
            }
        }

        const int activity = select(maxSocketDescriptor + 1, &set, NULL, NULL, NULL);

        if (activity < 0 && WSAGetLastError() != WSAEINTR) {
            logError("Select error");
            break;
        }

        if (FD_ISSET(server->master, &set)) {
            const SOCKET newSocket = accept(server->master, NULL, NULL);
            if (newSocket == INVALID_SOCKET) {
                logError("Accept failed");
                continue;
            }

            logInfo("New connection, socket is %d", newSocket);

            for (int i = 0; i < DEFAULT_MAX_CLIENTS; i++) {
                if (server->clients[i] == 0) {
                    server->clients[i] = newSocket;
                    break;
                }
            }
        }

        for (int i = 0; i < DEFAULT_MAX_CLIENTS; i++) {
            const SOCKET clientSocket = server->clients[i];
            if (FD_ISSET(clientSocket, &set)) {
                Request* request = acceptConnection(server, clientSocket);
                if (request == NULL) {
                    closesocket(clientSocket);
                    server->clients[i] = 0;
                    continue;
                }

                logRequest(request);

                Response* response = handleRequest(server, request);
                logInfo("Response created");
                sendResponse(server, response);
                logInfo("Response sent");

                freeRequest(request);
                freeResponse(response);
            }
        }
    }

    DeleteCriticalSection(&server->mutex);
    closesocket(server->master);
    WSACleanup();
}