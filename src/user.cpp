#include <iostream>
#include <cstring>
#include <string>
#include "../include/network_compat.h"

class User
{
    SocketHandle clientSocket;

public:
    bool connectToServer()
    {

#ifdef _WIN32
        WSADATA wsaData;
#endif

        if (WSA_INIT() != 0)
        {
            std::cout << "WSA initialization failed\n";
            return false;
        }

        clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        if (IS_INVALID_SOCKET(clientSocket))
        {
            std::cout << "Socket creation failed\n";

            CLOSE_SOCKET(clientSocket);
            WSA_CLEANUP();

            return false;
        }

        sockaddr_in serverAddr{};

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(8080);

        std::cout << "Connecting to server...\n";

        int inet_res =
            inet_pton(AF_INET,
                      "172.26.160.1",
                      &serverAddr.sin_addr);

        if (inet_res <= 0)
        {
            std::cout << "Invalid address\n";

            CLOSE_SOCKET(clientSocket);
            WSA_CLEANUP();

            return false;
        }

        int result =
            connect(clientSocket,
                    (sockaddr *)&serverAddr,
                    sizeof(serverAddr));

        if (result == SOCKET_ERROR_FD)
        {
            std::cout << "Connection failed\n";

            CLOSE_SOCKET(clientSocket);
            WSA_CLEANUP();

            return false;
        }

        std::cout << "Connected!\n";

        return true;
    }

    void disconnect()
    {
        CLOSE_SOCKET(clientSocket);

        WSA_CLEANUP();
    }

    bool sendCommand(const std::string &command)
    {
        int result =
            send(clientSocket,
                 command.c_str(),
                 (int)command.size(),
                 0);

        return !IS_SOCKET_ERROR(result);
    }

    std::string receiveResponse()
    {
        char buffer[4096];

        int bytes =
            recv(clientSocket,
                 buffer,
                 sizeof(buffer) - 1,
                 0);

        if (bytes <= 0)
            return "Disconnected";

        buffer[bytes] = '\0';

        return std::string(buffer);
    }
};

int main()
{
    User user;

    if (!user.connectToServer())
    {
        return 1;
    }

    while (true)
    {
        std::cout << "SQL|> ";

        std::string code;

        std::getline(std::cin, code);

        if (code == "EXIT;" || code == "exit;")
        {
            break;
        }

        if (!user.sendCommand(code))
        {
            std::cout << "Send failed!\n";
            break;
        }

        std::string response =
            user.receiveResponse();

        std::cout
            << response
            << std::endl;
    }

    user.disconnect();

    return 0;
}