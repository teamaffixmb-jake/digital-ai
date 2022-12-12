SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

CPPFLAGS+=-g
CPPFLAGS+=-std=c++20
CPPFLAGS+=-pthread
CPPFLAGS+=-I.
CPPFLAGS+=-I"../"

BINARY=main

all: $(BINARY)

$(BINARY): $(OBJECTS)
	# Build the final binary
	g++ $(CPPFLAGS) -o $@ $^
	
%.o: %.cpp
	# Build a single object file
	g++ $(CPPFLAGS) -c -o $@ $^
	
clean:
	rm -rf *.o
	rm -rf $(BINARY)
