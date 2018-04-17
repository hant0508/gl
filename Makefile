CXX = clang++
EXE = ./gl
CXXFLAGS = -c -MMD -g -Wall -Wextra -I.
LDFLAGS = -lglut -lGL -lGLU -o $(EXE)
OBJ = $(patsubst %.cpp, %.o, $(wildcard *.cpp))

all: $(EXE)

$(EXE): $(OBJ)
	$(CXX) $(LDFLAGS) $(OBJ)

-include *.d

run: all
	$(EXE)

clean:
	rm *.o *.d $(EXE)

debug: clean all
	gdb $(EXE)

.PHONY: all run clean debug
