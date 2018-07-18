#include <cstdlib>
#include <ctime>
#include "fight.hpp"
#include "bitmaps.hpp"

using namespace std;
using namespace cnv;

//ofstream logger ("gl.log");

void redraw() // glutDisplayFunc
{
	fight::draw();
	glutSwapBuffers();
}

void tick (int)
{
	fight::tick();
	glutPostRedisplay();
	glutTimerFunc(120, tick, 0);
}

void Mouse (int button, int state, int x, int y) // glutMouseFunc
{
	fight::mouse (button, state, x, y);
}

void Keyboard (unsigned char key, int, int) // glutKeyboardFunc
{
	fight::keyboard (key);
}

int main()
{
	window (WIDTH, HEIGHT);
	gluOrtho2D (0, WIDTH, HEIGHT, 0);
	glutDisplayFunc (redraw);
	glutKeyboardFunc (Keyboard);
	glutMouseFunc (Mouse);

	srand (time(0));
	bitmaps::load();
	fight::start();
	tick(1);

	glutMainLoop();
}
