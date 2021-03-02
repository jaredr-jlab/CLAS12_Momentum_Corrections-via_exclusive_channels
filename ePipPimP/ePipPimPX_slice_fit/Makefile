CC=g++
CPPFLAGS= -O3 -g -Wall `root-config --cflags`
LDFLAGS= -O3 -g -D_REENTRANT `root-config --glibs`
SOURCES=ePipPimPX_main.cpp sliceHisto2D.cpp fitSlices.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE= ePipPimPX_SliceFit

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.cpp
	$(CC) -c $(CPPFLAGS) $< -o $@

clean:
	rm -f ./*.o ./$(EXECUTABLE)
