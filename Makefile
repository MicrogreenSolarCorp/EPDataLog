CC = gcc
CFLAGS = -std=c99 -Wall -mconsole
LDFLAGS = -mconsole -lkernel32

# Default directories for sources and objects
COMMON_SRC_DIR = common
WINDOWS_SRC_DIR = windows
MACOS_SRC_DIR = macos

# Default software version and platform
VERSION = Mewyeah
PLATFORM = windows

# Determine the platform-specific file extensions and clean command
ifeq ($(OS),Windows_NT)
    EXECUTABLE_SUFFIX = .exe
    CLEAN_CMD = del
else
    EXECUTABLE_SUFFIX =
    CLEAN_CMD = rm -f
endif

# Directories where source files are located
COMMON_SRC = $(VERSION)/$(COMMON_SRC_DIR)
PLATFORM_SRC = $(VERSION)/$(PLATFORM_SRC_DIR)

# Source files
COMMON_SOURCES = $(wildcard $(COMMON_SRC)/*.c)
PLATFORM_SOURCES = $(wildcard $(PLATFORM_SRC)/*.c)

# Object files
COMMON_OBJECTS = $(COMMON_SOURCES:.c=.o)
PLATFORM_OBJECTS = $(PLATFORM_SOURCES:.c=.o)

# Determine the executable name based on version and platform
EXECUTABLE = EPDataLog_$(VERSION)_$(PLATFORM)$(EXECUTABLE_SUFFIX)

all: $(EXECUTABLE)

$(EXECUTABLE): $(COMMON_OBJECTS) $(PLATFORM_OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(CLEAN_CMD) $(EXECUTABLE) $(COMMON_OBJECTS) $(PLATFORM_OBJECTS)
