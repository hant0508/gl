#include "glcanvas.hpp"
#include "lodepng.h"

namespace cnv 
{
TextOut text_out;

void Image::load(const std::string& name)
{
	release();
	std::vector<unsigned char> image;
	const unsigned int error = lodepng::decode(image, width_, height_, name);

	if(error)
		throw std::runtime_error("Can't load image: " + name);

	glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D, id_);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);	

	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width_, height_, GL_RGBA, 
                      GL_UNSIGNED_BYTE, &image[0]);		
}

void Image::release() 
{
	if(empty()) return;
	glDeleteTextures(1, &id_);
	id_ = GL_INVALID_VALUE;
}

void Image::draw(float x1, float y1, float x2, float y2)
{
	glEnable(GL_TEXTURE_2D);
   	glBindTexture(GL_TEXTURE_2D, id_);
	glBegin(GL_QUADS);
		glTexCoord2d(0, 1); glVertex2f(x1, y1);
		glTexCoord2d(1, 1); glVertex2f(x2, y1);
		glTexCoord2d(1, 0); glVertex2f(x2, y2);
		glTexCoord2d(0, 0); glVertex2f(x1, y2);
	glEnd();
	glColor3f(1,1,1);
	glDisable(GL_TEXTURE_2D);

}

void Image::draw(float x, float y, float size)
{
	draw(x, y, x+float(width_)*size, y + float(height_)*size);
}

void init()
{
	char* s = new char[100 + 1];
	strcpy(s, "noparams");
	int n = 1;
	glutInit(&n, &s);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
}

void window()
{
	window(600, 600);
}

void window(int w, int h)
{
	init();
	glutInitWindowSize(w, h);
	glutCreateWindow("glcanvas");
 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND); 
	clear(1, 1, 1, 1);
	color(0, 0, 0);
}

void repaint()
{
	glutSwapBuffers();
}

void start(DisplayFunc f)
{
	window();
	position(0, 0);
	glutDisplayFunc(f);
	glutMainLoop();
}

void dot(float x, float y)
{
	glBegin(GL_POINTS);
	glVertex2f(x, y);
	glEnd();
}


void circle(float x, float y, float radius)
{
	circle(x, y, radius, GL_LINES);
}

void circle_fill(float x, float y, float radius)
{
	circle(x, y, radius, GL_TRIANGLE_FAN);
}

void circle(float x, float y, float radius, int vtype)
{
	glBegin(vtype);
	float vx = x + radius;
	float vy = y;	

	const int n = 360;

	for(int i = 0; i <= n; ++i)
	{
		glVertex2f(vx,vy);
		vx = x + radius*cos(float(i) * 2*CNV_PI/n);
		vy = y + radius*sin(float(i) * 2*CNV_PI/n);
		glVertex2f(vx,vy);
	
	}
	glEnd();
}

void line(float x1, float y1, float x2, float y2)
{
	glBegin(GL_LINES);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
	glEnd();
}

void rect_fill(float x1, float y1, float x2, float y2)
{
	glRectf(x1, y1, x2, y2);
}

void rect(float x1, float y1, float x2, float y2)
{
	glBegin(GL_LINES);
		glVertex2f(x1, y1);
		glVertex2f(x1, y2);
		glVertex2f(x1, y2);
		glVertex2f(x2, y2);
		glVertex2f(x2, y2);
		glVertex2f(x2, y1);
		glVertex2f(x2, y1);
		glVertex2f(x1, y1);
	glEnd();
//	vertex_out << GL_LINES << Pos(x1, y1) << Pos(x1, y2) << Pos(x1, y2) << Pos(x2, y2) << Pos(x2, y2) << Pos(x2, y1) << Pos(x2, y1) << Pos(x1, y1) << end;
}

} // namespace cnv


