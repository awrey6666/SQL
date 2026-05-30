# Cross-Platform Makefile for SQL Server/Client
# Supports: Linux, Windows (MinGW), and macOS

CXX = g++
BUILD_DIR = build

# Detect OS
UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)

# Platform-specific configuration
ifeq ($(UNAME_S),Linux)
    # Linux-specific settings
    PLATFORM = linux
    CXXFLAGS = -std=c++17 -Iinclude -pthread -static-libgcc -static-libstdc++
    LDFLAGS = 
    SERVER_OUT = $(BUILD_DIR)/app
    USER_OUT = $(BUILD_DIR)/user
    STATIC_FLAG = -static
endif

ifeq ($(UNAME_S),Darwin)
    # macOS-specific settings (no static linking)
    PLATFORM = macos
    CXXFLAGS = -std=c++17 -Iinclude -pthread
    LDFLAGS = 
    SERVER_OUT = $(BUILD_DIR)/app
    USER_OUT = $(BUILD_DIR)/user
    STATIC_FLAG = 
endif

ifdef WINDIR
    # Windows (MinGW/MSYS) detection
    PLATFORM = windows
    CXXFLAGS = -std=c++17 -Iinclude -pthread -static-libgcc -static-libstdc++
    LDFLAGS = -lws2_32
    SERVER_OUT = $(BUILD_DIR)/app.exe
    USER_OUT = $(BUILD_DIR)/user.exe
    STATIC_FLAG = -static
endif

# If OS not detected, default to Windows
ifndef PLATFORM
    PLATFORM = windows
    CXXFLAGS = -std=c++17 -Iinclude -pthread -static-libgcc -static-libstdc++
    LDFLAGS = -lws2_32
    SERVER_OUT = $(BUILD_DIR)/app.exe
    USER_OUT = $(BUILD_DIR)/user.exe
    STATIC_FLAG = -static
endif

# Source files
SERVER_SRC = src/main.cpp \
             src/compiler.cpp \
             src/file_system.cpp \
             src/SocketManager.cpp \
             src/SQL.cpp \
             src/Table.cpp \
             src/utils.cpp \
             src/thread_pool.cpp

USER_SRC = src/user.cpp

# Default target
all: server

# Build server
server: $(SERVER_OUT)

$(SERVER_OUT): $(SERVER_SRC)
	@mkdir -p $(BUILD_DIR)
	@echo "[$(PLATFORM)] Building server..."
	$(CXX) $(SERVER_SRC) $(CXXFLAGS) $(STATIC_FLAG) $(LDFLAGS) -o $(SERVER_OUT)
	@echo "Build complete: $(SERVER_OUT)"

# Build user client
user: $(USER_OUT)

$(USER_OUT): $(USER_SRC)
	@mkdir -p $(BUILD_DIR)
	@echo "[$(PLATFORM)] Building user client..."
	$(CXX) $(USER_SRC) $(CXXFLAGS) $(STATIC_FLAG) $(LDFLAGS) -o $(USER_OUT)
	@echo "Build complete: $(USER_OUT)"

# Run server
run_server: server
	@echo "[$(PLATFORM)] Running server..."
	@$(SERVER_OUT)

# Run user client
run_user: user
	@echo "[$(PLATFORM)] Running user client..."
	@$(USER_OUT)

# Clean build artifacts
clean:
	@echo "Cleaning build directory..."
	@rm -rf $(BUILD_DIR)
	@echo "Clean complete"

# Print platform info (for debugging)
info:
	@echo "Detected Platform: $(PLATFORM)"
	@echo "UNAME_S: $(UNAME_S)"
	@echo "UNAME_M: $(UNAME_M)"
	@echo "Server output: $(SERVER_OUT)"
	@echo "User output: $(USER_OUT)"
	@echo "CXXFLAGS: $(CXXFLAGS)"
	@echo "LDFLAGS: $(LDFLAGS)"
	@echo "STATIC_FLAG: $(STATIC_FLAG)"

.PHONY: all server user run_server run_user clean info