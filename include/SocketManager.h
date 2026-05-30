#pragma once

#include <thread>
#include "SQL.h"
#include "network_compat.h"

class SocketManager
{
private:
    SocketHandle serverSocket;
    bool running;
    std::thread worker;

public:
    void start(SQL &db);
    void stop();
    
private:
    void loop(SQL &db);
    void setupListeningSocket();
    void handleClient(SocketHandle client, SQL& db);
};