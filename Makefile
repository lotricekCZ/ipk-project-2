CC=g++
CFLAGS=-c -g -Wall -std=c++20 -Ilib
LDFLAGS=-Llib
LIBS=-lpcap -lnet
SOURCES=$(wildcard src/*.cpp)
SOURCES += $(wildcard src/decoders/*.cpp)
SOURCES += $(wildcard src/encoders/*.cpp)
SOURCES += $(wildcard src/formats/*.cpp)
SOURCES += $(wildcard src/fsm/*.cpp)
SOURCES += $(wildcard src/io_handler/*.cpp)
SOURCES += $(wildcard src/receivers/*.cpp)
SOURCES += $(wildcard src/senders/*.cpp)
SOURCES += $(wildcard src/transceivers/*.cpp)
OBJECTS=$(patsubst src/%.cpp,build/%.o,$(SOURCES))
EXECUTABLE=ipk25-chat

all: build_dirs $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $@

build/%.o: src/%.cpp | build_dirs
	$(CC) $(CFLAGS) $< -o $@

build_dirs:
	mkdir -p build $(patsubst src/%,build/%,$(dir $(SOURCES)))

clean:
	rm -rf build bin

.PHONY: all clean