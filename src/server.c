#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>

#include "log.h"
#include "server.h"
#include "request.h"

SOCKET server;

SOCKET createServerSocket() {
    initializeWSAData();
    const SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) {
        logCritical("Socket creation failed.");
        WSACleanup();
        exit(FATAL);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(s, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        logCritical("Bind failed.");
        closesocket(s);
        WSACleanup();
        return 1;
    }

    logInfo("Socket bound to port 8080");

    return server = s;
}

Request* acceptConnection() {
    if (listen(server, SOMAXCONN) == SOCKET_ERROR) {
        logCritical("Listen failed");
        closesocket(server);
        WSACleanup();
        exit(FATAL);
    }

    const SOCKET client = accept(server, NULL, NULL);
    if (client == INVALID_SOCKET) {
        logCritical("Accept failed.");
        exit(FATAL);
    }

    logInfo("Connection accepted...");

    char incoming[1024];
    const int bytesReceived = recv(client, incoming, sizeof(incoming) - 1, 0);

    if (bytesReceived <= 0) {
        return NULL;
    }

    incoming[bytesReceived - 1] = '\0';

    Request* request = parseRequest(incoming);
    request->client = client;

    return request;
}

Response* handleRequest(const Request* request) {
    Response* response = newResponse(request, "Hello, World", 200);
    return response;
}

int sendResponse(Response* response) {
    char* buffer = createBuffer(response);
    const int bytesSent = send(response->client, buffer, strlen(buffer), 0);

    free(buffer);
    return bytesSent;
}

struct WSAData initializeWSAData() {
    struct WSAData wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        logError("WSAStartup failed.");
        exit(FATAL);
    }

    logInfo("Winsock initialized.");
    return wsaData;
}

void startServer() {
    const SOCKET server = createServerSocket();

    logInfo("Listening for connections... Socket: %d", server);
    while (TRUE) {
        Request* request = acceptConnection(server);
        logRequest(request);

        Response* response = handleRequest(request);
        logInfo("Response created");
        sendResponse(response);
        logInfo("Response send");

        freeRequest(request);
        freeResponse(response);
    }
}
