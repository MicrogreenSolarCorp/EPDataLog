CODE_VERSION = 1.2
BMS_VERSION = Daly

CC = gcc
CFLAGS = -Wall
LDFLAGS =
# Sample Output
# EPDataLog_Daly_Windows_v1.2
TARGET = myprogram
UNIVERSAL_TARGET = $(TARGET)-universal

# Directories
SRCDIR = .
OBJDIR = obj
BINDIR = bin

# Platform-specific files
MAC_FILES = $(wildcard $(SRCDIR)/macos/*.c)
WIN_FILES = $(wildcard $(SRCDIR)/windows/*.c)
COMMON_FILES = $(wildcard $(SRCDIR)/*.c)

# Object files
MAC_OBJS = $(MAC_FILES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
WIN_OBJS = $(WIN_FILES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
COMMON_OBJS = $(COMMON_FILES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# For Mac, create universal binaries
.PHONY: all mac windows clean

all: mac windows

mac: $(UNIVERSAL_TARGET)

$(UNIVERSAL_TARGET): $(MAC_OBJS) $(COMMON_OBJS)
	$(CC) $(CFLAGS) -arch x86_64 -o $(BINDIR)/$(TARGET)-x86_64 $^ $(LDFLAGS)
	$(CC) $(CFLAGS) -arch arm64 -o $(BINDIR)/$(TARGET)-arm64 $^ $(LDFLAGS)
	lipo -create -output $(BINDIR)/$(UNIVERSAL_TARGET) $(BINDIR)/$(TARGET)-x86_64 $(BINDIR)/$(TARGET)-arm64

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Add a windows target if you need to compile on Windows
windows: $(WIN_OBJS) $(COMMON_OBJS)
	# Compilation command for Windows

clean:
	rm -f $(OBJDIR)/*.o $(BINDIR)/$(TARGET)-x86_64 $(BINDIR)/$(TARGET)-arm64 $(BINDIR)/$(UNIVERSAL_TARGET)
