# The name of the source files
SOURCES = connection.c main.c ../common/getData.c

# The name of the executable
EXE = results

# Flags for compilation (adding warnings are always good)
CFLAGS = -Wall

# Flags for linking (none for the moment)
LDFLAGS =

# Libraries to link with (none for the moment)
LIBS =

# Use the GCC frontend program when linking
LD = gcc

# This creates a list of object files from the source files
OBJECTS = $(SOURCES:%.c=%.o)

# The first target, this will be the default target if none is specified
# This target tells "make" to make the "all" target
default: all

# Having an "all" target is customary, so one could write "make all"
# It depends on the executable program
all: $(EXE)

# This will link the executable from the object files
$(EXE): $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o  $(EXE) $(LIBS)

# This is a target that will compile all needed source files into object files
# We specify the compiler (CC) and compilation flags (CFLAGS)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Target to clean up after us
clean:
	-rm -f $(EXE)      # Remove the executable file
	-rm -f $(OBJECTS)  # Remove the object files

# Finally we need to tell "make" what source and header file each object file depends on
getData.o: getData.c getData.h
connection.o: connection.c connection.h ../common/getData.h
main.o: main.c connection.h ../common/getData.h
