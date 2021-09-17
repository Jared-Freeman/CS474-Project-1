# Check for OS Type
UNAME_S := $(shell uname -s)

# Linux
ifeq ($(UNAME_S), Linux)
	CC=g++
# Mac
else
	CC=clang++
	LIBS= -stdlib=libc++
endif

#Compiler Flags
CXXFLAGS=-g -Wall -std=c++0x

# .o Compilation
O_FILES=main.o camera.o engine.o graphics.o object.o shader.o window.o

# Point to includes of local directories
INCLUDES=-I../include


all: $(O_FILES)
	$(CC) $(CXXFLAGS) -o CS474 $(O_FILES) $(LIBS)
main.o: ../src/main.cpp
	$(CC) $(CXXFLAGS) -c ../src/main.cpp -o main.o $(INCLUDES)


clean:
	-@if rm *.o CS474 2>/dev/null || true; then echo "Main Removed"; else echo "No Main"; fi
