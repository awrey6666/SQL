#include <iostream>
#include <thread>
#include <atomic>
#include "network_compat.h"
#include "SocketManager.h"
#include <functional>
#include "compiler.h"

void SocketManager::start(SQL &db)
{
    running = true;
    setupListeningSocket();
    worker = std::thread(&SocketManager::loop, this, std::ref(db));
}

void SocketManager::stop()
{
    running = false;

    CLOSE_SOCKET(serverSocket);

    if (worker.joinable())
        worker.join();
}

void SocketManager::setupListeningSocket()
{

#ifdef _WIN32
    WSADATA wsaData;
#endif

    if (WSA_INIT() != 0)
    {
        std::cerr << "WSA initialization failed\n";
        return;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (IS_INVALID_SOCKET(serverSocket))
    {
        std::cerr << "Socket creation failed\n";
        return;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR_FD)
    {
        std::cerr << "Bind failed\n";
        return;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR_FD)
    {
        std::cerr << "Listen failed\n";
        return;
    }

    std::cout << "Server listening on port 8080\n";
}
void SocketManager::handleClient( SocketHandle client, SQL& db)
{
    char buffer[1024];

    while (true)
    {
        int bytes =
            recv(client,
                 buffer,
                 sizeof(buffer) - 1,
                 0);

        if (bytes <= 0)
        {
            std::cout << "Client disconnected\n";
            break;
        }

        buffer[bytes] = '\0';

        std::string msg(buffer);

        std::string answer =SQL_compiler(db, msg);

        send(client,answer.c_str(), answer.size(), 0);
    }

    CLOSE_SOCKET(client);
}
void SocketManager::loop(SQL &db)
{
    while (running)
    {
        sockaddr_in clientAddr;
        socklen_t clientSize = sizeof(clientAddr);

        SocketHandle client =
            accept(serverSocket,
                   (sockaddr *)&clientAddr,
                   &clientSize);

        if (IS_INVALID_SOCKET(client))
            continue;

        std::cout << "Client connected\n";

        std::thread(&SocketManager::handleClient,this,client,std::ref(db)).detach();}

    WSA_CLEANUP();
}