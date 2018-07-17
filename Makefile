CXX = clang++
EXE = ./gl
EXCLUDE = t.cpp p.cpp 
CXXFLAGS = -c -MMD -g -Wall -Wextra -I. # don't remove; used by default (implicit) targets
LDFLAGS = -lglut -lGL -lGLU -o $(EXE)
OBJ = $(patsubst %.cpp, %.o, $(filter-out $(EXCLUDE), $(wildcard *.cpp)))
PREREQ = $(patsubst %.o, %.d, $(OBJ))

all: $(EXE)

$(EXE): $(OBJ)
	$(CXX) $(LDFLAGS) $(OBJ)

-include *.d

run: all
	$(EXE)

clean:
	$(RM) $(OBJ) $(PREREQ) $(EXE)

debug: clean all
	gdb $(EXE)

.PHONY: all run clean debug
