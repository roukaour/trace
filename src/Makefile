CC=g++
CFLAGS=-W -Wall -O3
DEBUGFLAGS=-g3 -ggdb3 -pg
LDFLAGS=-lglut32 -lglu32 -lopengl32 -lm
HEADERS=Color.h Vector.h Ray.h Material.h Light.h Entity.h Sphere.h Plane.h Triangle.h Intersection.h Camera.h Scene.h Renderer.h Parser.h
SOURCES=trace.cpp $(HEADERS:.h=.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=trace
DEBUGEXE=trace-debug
RM=rm -rf

all: $(SOURCES) $(EXECUTABLE) $(DEBUGEXE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(DEBUGEXE): $(OBJECTS)
	$(CC) $(CFLAGS) $(DEBUGFLAGS) $^ -o $@ $(LDFLAGS)

.cpp.o: $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

check: $(SOURCES)
	cppcheck --enable=all .

clean:
	$(RM) $(EXECUTABLE) $(EXECUTABLE).exe $(DEBUGEXE) $(DEBUGEXE).exe *.o *.h.gch *.stackdump *~ core *conflicted\ copy*
