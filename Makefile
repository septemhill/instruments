# Makefile for a C project using Csound

# Compiler and flags
CC = clang
CFLAGS = -Wall -Wextra -g -O2 -I.
LDFLAGS =
RM = rm -f

# Csound installation path (adjust if necessary)
CSOUND_HOME = /opt/homebrew/Cellar/csound/6.18.1_12

# Target executable name
TARGET = csound_example

# Source files
SRCS = main.c instrument_piano.c instruments.c score.c

# Object files
OBJS = $(SRCS:.c=.o)

# Default target
all: build

# OS-specific settings
ifeq ($(OS), Windows_NT)
    # Windows - Csound setup for Windows would be different.
    TARGET := $(TARGET).exe
    RM = del /Q
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S), Linux)
        # Linux
        LDFLAGS += -ldl -lpthread -lm
    endif
    ifeq ($(UNAME_S), Darwin)
        # macOS
        CFLAGS += -I$(CSOUND_HOME)/Frameworks/CsoundLib64.framework/Versions/6.0/Headers
        LDFLAGS += -F$(CSOUND_HOME)/Frameworks -framework CsoundLib64
    endif
endif

# Build target
build: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	$(RM) $(TARGET) $(OBJS)

# Phony targets
.PHONY: all build clean
