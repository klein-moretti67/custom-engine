# Cross-Platform Makefile for GCC (Linux) and MSVC (Windows)
# Detect OS
ifeq ($(OS),Windows_NT)
    detected_OS := Windows
else
    detected_OS := $(shell uname)
endif

# Name of the output executable
TARGET = engine

# Source files
SRCS = main.cpp src/glad.c

# Include directories
INCLUDES = -Iinclude -I.

# Platform Specific Configuration
ifeq ($(detected_OS),Windows)
    # MSVC Configuration
    CC = cl
    # /EHsc: standard C++ exception handling
    # /Zi: generate debug information
    # /W3: warning level 3
    # /D_CRT_SECURE_NO_WARNINGS: suppress security warnings for CRT functions
    CFLAGS = /nologo /W3 /EHsc /Zi $(INCLUDES) /D_CRT_SECURE_NO_WARNINGS
    
    # Linker flags (passed to cl after /link)
    # /DEBUG: generate debug file
    LDFLAGS = /link /DEBUG /OUT:$(TARGET).exe
    
    # Libraries to link against on Windows
    # opengl32.lib is standard, glfw3.lib should be in your library path
    LIBS = user32.lib gdi32.lib shell32.lib opengl32.lib glfw3.lib
    
    EXE_EXT = .exe
    OBJ_EXT = obj
    RM = del /Q
    
    # Build command for MSVC (compiles and links in one go for simplicity)
    BUILD_CMD = $(CC) $(CFLAGS) $(SRCS) /Fe$(TARGET) $(LIBS) $(LDFLAGS)
else
    # GCC Configuration (Linux/macOS)
    CC = g++
    CFLAGS = -Wall -Wextra -g $(INCLUDES)
    
    # Libraries to link against on Linux
    LIBS = -lglfw -lGL -lm -ldl -lpthread
    
    EXE_EXT =
    OBJ_EXT = o
    RM = rm -f
    
    # Build command for GCC
    BUILD_CMD = $(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LIBS)
endif

# Default rule
all: $(TARGET)

$(TARGET): $(SRCS)
	$(BUILD_CMD)

# Clean up build artifacts
clean:
	$(RM) $(TARGET)$(EXE_EXT) *.$(OBJ_EXT) *.pdb *.ilk

.PHONY: all clean
