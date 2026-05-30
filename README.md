# SQL Server/Client - Cross-Platform C++ Application

A portable SQL server and client application supporting **Linux**, **Windows** (MinGW), and **macOS**.

## Features

- Cross-platform networking using POSIX-compliant sockets
- Automatic platform detection at build time
- Single-threaded command compilation and execution
- Portable file I/O across Windows, Linux, and macOS
- Database support with table management

## Building

### Prerequisites

- **C++ Compiler**: g++, clang++, or MinGW-w64 (for Windows)
- **C++ Standard**: C++17 or higher
- **Build Tool**: GNU Make (make/mingw32-make)
- **Threading**: POSIX threads (pthread)

### Quick Build

```bash
# Clean previous builds
make clean

# Build server and client
make server user

# Or build individually
make server    # Build server only
make user      # Build client only
```

### Platform-Specific Build Notes

#### Linux
```bash
make clean
make info      # Shows detected platform: linux
make server user
./build/app    # Run server
./build/user   # Run client
```

#### macOS
```bash
make clean
make info      # Shows detected platform: macos
make server user
./build/app    # Run server
./build/user   # Run client
```
**Note**: macOS builds use dynamic linking (no static flags) for better compatibility.

#### Windows (MinGW/MSYS2)
```bash
mingw32-make clean
mingw32-make info      # Shows detected platform: windows
mingw32-make server user
build\app.exe          # Run server
build\user.exe         # Run client
```

## Running

### Start the Server

```bash
# Linux/macOS
./build/app

# Windows
build\app.exe
```

The server will start listening on `127.0.0.1:8080`.

### Connect with Client

In another terminal:

```bash
# Linux/macOS
./build/user

# Windows
build\user.exe
```

Type SQL commands at the `SQL|> ` prompt. Exit with `EXIT;` or `exit;`.

## Available Make Targets

| Target | Purpose |
|--------|---------|
| `make all` | Build server (default) |
| `make server` | Build server application |
| `make user` | Build client application |
| `make run_server` | Build and run server |
| `make run_user` | Build and run client |
| `make clean` | Remove all build artifacts |
| `make info` | Display platform detection and build flags |

## Platform Detection

The Makefile automatically detects the operating system:

```makefile
Linux   → Executable: build/app, build/user
macOS   → Executable: build/app, build/user
Windows → Executable: build/app.exe, build/user.exe
```

Run `make info` to see the detected platform and current build configuration.

## Cross-Platform Compatibility

The project uses a compatibility layer (`include/network_compat.h`) to provide:

- **Windows**: `<winsock2.h>`, `<ws2tcpip.h>` for socket APIs
- **Unix/Linux/macOS**: `<sys/socket.h>`, `<netinet/in.h>`, `<arpa/inet.h>`, `<unistd.h>`

Key abstractions:
- `SocketHandle`: Portable socket type (`SOCKET` on Windows, `int` on Unix)
- `CLOSE_SOCKET()`: Macro for `closesocket()` (Windows) or `close()` (Unix)
- `WSA_INIT()` / `WSA_CLEANUP()`: Network initialization (no-op on Unix)
- `IS_INVALID_SOCKET()`, `IS_SOCKET_ERROR()`: Portable error checking

## File Structure

```
SQL/
├── Makefile                 # Cross-platform build configuration
├── README.md                # This file
├── include/
│   ├── compiler.h
│   ├── file_system.h
│   ├── network_compat.h     # Platform abstraction layer
│   ├── SocketManager.h
│   ├── SQL.h
│   ├── Table.h
│   ├── thread_pool.h
│   └── utils.h
├── src/
│   ├── compiler.cpp
│   ├── file_system.cpp      # Portable file I/O (Windows _findfirst vs Unix opendir)
│   ├── main.cpp
│   ├── SocketManager.cpp    # Platform-independent networking
│   ├── SQL.cpp
│   ├── Table.cpp
│   ├── thread_pool.cpp
│   ├── user.cpp
│   └── utils.cpp
├── DB/
│   ├── table.txt
│   └── users.txt
└── build/                   # Generated executables
    ├── app                  # Server (Linux/macOS)
    ├── app.exe              # Server (Windows)
    ├── user                 # Client (Linux/macOS)
    └── user.exe             # Client (Windows)
```

## Compiler Flags

### Linux
```
-std=c++17 -Iinclude -pthread -static-libgcc -static-libstdc++ -static
```

### macOS
```
-std=c++17 -Iinclude -pthread
```

### Windows (MinGW)
```
-std=c++17 -Iinclude -pthread -static-libgcc -static-libstdc++ -static -lws2_32
```

## Troubleshooting

### "make: command not found" on Windows
Use `mingw32-make` instead of `make` or install MSYS2 with the mingw-w64 toolchain.

### Static linking errors on macOS
macOS does not support static linking of the standard library. The Makefile automatically removes static flags for macOS. Use dynamic linking:
```bash
make clean
make server user
```

### Socket errors on Windows
Ensure the Winsock library (`-lws2_32`) is linked. Run `make info` to verify the build configuration includes this flag.

### Build fails with "io.h not found"
This occurs on Linux/macOS when trying to use Windows-specific headers. The compatibility layer should handle this. If the error persists, ensure `#include "network_compat.h"` is included before any platform-specific headers.

## Testing

A simple smoke test: run the server and client on the same machine:

```bash
# Terminal 1: Start server
./build/app

# Terminal 2: Connect client
./build/user

# In client terminal, type a command
SQL|> SELECT * FROM users;

# Exit with:
SQL|> exit;
```

## License

This project is provided as-is for educational purposes.

## Notes

- The application uses POSIX-compliant socket APIs, making it portable across platforms.
- WSA (Windows Sockets API) initialization is abstracted via macros in the compatibility layer.
- File system operations use platform-native APIs (`_findfirst` on Windows, `opendir`/`readdir` on Unix).
- All platform-specific code is guarded with `#ifdef _WIN32` conditionals.
