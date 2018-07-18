#include <cstdlib>
#include <ctime>
#include "fight.hpp"
#include "bitmaps.hpp"

using namespace std;
using namespace cnv;

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
	int len = bitmaps::getWindowSize();
	x -= max((glutGet(GLUT_WINDOW_WIDTH) - len)/2, 0);
	y -= max((glutGet(GLUT_WINDOW_HEIGHT) - len)/2, 0);
	if (x < 0 || y < 0 || x > len || y > len) return;
	fight::mouse (button, state, x, y);
}

void Keyboard (unsigned char key, int, int) // glutKeyboardFunc
{
	fight::keyboard (key);
}

void reshape (int width, int height)
{
	int len = min (width, height);
	int x = max((width - len)/2, 0);
	int y = max((height - len)/2, 0);
	glViewport (x, y, len, len);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D (0, len, len, 0); // l, r, b, t
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
	bitmaps::setWindowSize (len);
	fight::board().redraw();
	glutPostRedisplay();
}

int main()
{
	window (DEFAULT_WINDOW_SIZE, DEFAULT_WINDOW_SIZE);

	glutDisplayFunc (redraw);
	glutKeyboardFunc (Keyboard);
	glutMouseFunc (Mouse);
	glutReshapeFunc (reshape);

	srand (time(0));
	bitmaps::load();
	fight::start();
	tick(1);

	glutMainLoop();
}
