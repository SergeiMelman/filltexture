SHORT_NAME = alglib

CC = g++
CFLAGS = -Wall -Werror -pedantic -std=c++17 -O

LIBRARY_NAME = build/lib$(SHORT_NAME).a
SOURCE_FILES = $(wildcard src/*.cpp)
OBJECT_FILES = $(patsubst src/%.cpp, src/%.o, $(SOURCE_FILES))

.PHONY: all clean

all: 
	mkdir -p build
all: $(LIBRARY_NAME)
all: clean

clean:
	rm -f $(OBJECT_FILES)

$(LIBRARY_NAME): $(OBJECT_FILES)
	ar rcs $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@
