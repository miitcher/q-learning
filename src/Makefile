CC=g++
CPPFLAGS=-c -g -std=c++0x -Wall -Wextra -pedantic -pthread
LDFLAGS=-lpthread
SOURCES=main.cpp
OBJECTS=main.o
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CPPFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

run: main
	./main
