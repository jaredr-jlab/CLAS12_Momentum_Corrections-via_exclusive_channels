CC=g++
CFLAGS= -c -O3 -g -Wall `root-config --cflags`
LDFLAGS= -g -O3 -D_REENTRANT `root-config --glibs`
SOURCES=ePipPimPX_main.cpp sliceFit.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE= ePipPimPX_slice_fit

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f ./*.o ./$(EXECUTABLE)
