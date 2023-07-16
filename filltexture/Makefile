CC = g++
CPPFLAGS = -c -Wall -Werror -pedantic -std=c++17 -O3 -fopenmp 

CPPINCLUDES = \
	-I../qhull/install/include \
	-I../alglib/src \

LDFLAGS = \
	-L../qhull/install/lib \
	-L../alglib/build \

LDLIBS = \
	-lqhullcpp \
	-lqhullstatic_r \
	-lalglib \
	-lgomp \

EXECUTABLE=filltexture

SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(SOURCES) $(OBJECTS) $(EXECUTABLE)

debug: CPPFLAGS = -c -Wall -Werror -pedantic -std=c++17 -DDEBUG -g -fopenmp 
debug: all

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS) $(LDLIBS) 

.cpp.o:
	$(CC) $(CPPINCLUDES)  $(CPPFLAGS)$< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

cleano:
	rm -f $(OBJECTS)

