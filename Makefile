all:

	clang++ -g -Wall -Wextra -lglut -lGL -lGLU -I. glcanvas.cpp lodepng.cpp main.cpp warrior.cpp fight.cpp board.cpp state.cpp animator.cpp bitmaps.cpp

run: all

	./a.out

debug: all

	gdb ./a.out
