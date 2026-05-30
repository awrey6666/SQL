#pragma once

/*
 * Cross-Platform Network Compatibility Layer
 * Provides platform-independent abstractions for Windows, Linux, and macOS
 */

#ifdef _WIN32
// Windows Socket Headers
#include <winsock2.h>
#include <ws2tcpip.h>

// Type Aliases
typedef SOCKET SocketHandle;

// Socket Constants
#define INVALID_SOCKET_FD INVALID_SOCKET
#define SOCKET_ERROR_FD SOCKET_ERROR

// Socket Close Function
#define CLOSE_SOCKET(s) closesocket(s)

// WSA Initialization/Cleanup
#define WSA_INIT() WSAStartup(MAKEWORD(2, 2), &wsaData)
#define WSA_CLEANUP() WSACleanup()

#else
// Unix/Linux/macOS Headers
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// Type Aliases
typedef int SocketHandle;

// Socket Constants
#define INVALID_SOCKET_FD -1
#define SOCKET_ERROR_FD -1

// Socket Close Function
#define CLOSE_SOCKET(s) close(s)

// WSA Initialization/Cleanup (no-op on Unix)
#define WSA_INIT() 0
#define WSA_CLEANUP() \
	do                  \
	{                   \
	} while (0)

#endif

// Portable socket error checking
#define IS_SOCKET_ERROR(result) ((result) == SOCKET_ERROR_FD)
#define IS_INVALID_SOCKET(sock) ((sock) == INVALID_SOCKET_FD)
