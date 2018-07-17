/*
Copyright (c) 2008-2014 by Andrew Stroganov <savhte@gmail.com>

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions 
are met:
Redistributions of source code must retain the above copyright 
notice, this list of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright 
notice, this list of conditions and the following disclaimer in 
the documentation and/or other materials provided with the 
distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
POSSIBILITY OF SUCH DAMAGE.
*/

/*
=== glcanvas. Документация ===
glcanvas -- небольшая библиотека двумерной графики на базе FreeGLUT, 
написанная с целью:

1. сделать возможным использование двумерной графики на занятиях 
по C++ как можно раньше. Таким образом программа должна содержать
как можно меньше вызовов вспомогательных функций, и разных
механизмов инициализации и настройки.

2. предоставить вспомогательные функции для вывода простейших
геометрических фигур, изображений и т.д.

Здесь приведен только перечень функций и объектов библиотеки. Примеры 
использования и задачи будут доступны по адресу: http://savthe.com/edu

=== Обзор функций ===

float frand(float low, float high, float h = 0.00001)
Возвращает случайное число с плавающей точкой в пределах
[low, high] с точностью h.

void color(float r, float g, float b)
Эквивалент: glColor4f(r, g, b, 1)

void position(float x, float y)
Эквивалент: glRasterPos2f(x, y)

void clear()
Эквивалент: glClear(GL_COLOR_BUFFER_BIT)

void clear(float r, float g, float b, float alpha = 1)
Эквивалент:
	glClearColor(r, g, b, alpha); 
	glClear(GL_COLOR_BUFFER_BIT);

void circle(float x = 0, float y = 0, float radius = 0.5);
Окружность с центром в точке (x, y), радиуса radius

void circle_fill(float x = 0, float y = 0, float radius = 0.5);
Круг с центром в точке (x, y), радиуса radius

void dot(float x = 0, float y = 0);
Точка с заданными координатами

void rect(float x1, float y1, float x2, float y2);
Прямоугольник, заданный координатами диагонали (x1, y1), (x2, y2)

void rect_fill(float x1, float y1, float x2, float y2);
Закрашенный прямоугольник, заданный координатами диагонали (x1, y1), (x2, y2)

void init()
Вызывает glutInit() с параметрами "noparams", 1 и glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE).

void start(DisplayFunc)
Эквивалент:
	window();
	glutDisplayFunc(f);
	glutMainLoop();

void repaint()
Эквивалент: glutSwapBuffers();

void window()
Эквивалент: window(600, 600);

void window(int, int)
Вызывает init(). Создает окно размером width x height, настраивает параметры для использования
альфа-канала, очищает экран.

== Объект Image ==
Используется для хранения текстуры и вывода двумерного изображения на экран. Используется формат PNG, 
альфа-канал поддерживается. 

Методы:
void load(const std::string& file_name)
	Загружает изображение из файла в качестве текстуры (GL_TEXTURE_2D)

void draw(float x1, float y1, float x2, float y2)
	Выводит на экран прямоугольник, закрашенный данным изображением

void Image::draw(float x, float y, float size)
	Выводит на экран прямоугольник со сторонами, пропорциональными размеру
	изображения.

=== Вывод текста ===
Для вывода текста используется объект text_out. Пример:
text_out << "Пример вывода" << " текста или чисел: " << frand(-1, 1);
Координаты задаются функцией position().

endl не поддерживается. 
*/

#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <cstring>
#include <sstream>
#include <string>
#include <stdexcept>

#ifdef M_PI
#define CNV_PI M_PI
#else
#define CNV_PI 3.14159265358979323846;
#endif

inline float frand(float low, float high)
{
	const int m = 0x7fff;
	const float r = rand() % m / float(m - 1);
	return r * (high - low) + low;
}

namespace cnv {

typedef void (*DisplayFunc)(void);

class Image
{
public:
	Image(): id_(GL_INVALID_VALUE) {}
	Image(const std::string& name): id_(GL_INVALID_VALUE) { load(name); }
	~Image() { release(); }

	bool empty() const { return id_ == GL_INVALID_VALUE; }

	void load(const std::string& name);
	void release();
	void draw(float x1, float y1, float x2, float y2);
	void draw(float x, float y, float size = 1);


private:
	unsigned int width_;
	unsigned int height_;
	GLuint id_;
};

inline void color(float r, float g, float b)
{
	glColor4f(r, g, b, 1.0);
}

inline void position(float x, float y)
{
	glRasterPos2f(x, y);
}

inline void clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

inline void clear(float r, float g, float b, float alpha = 1)
{
	glClearColor(r, g, b, alpha); 
	glClear(GL_COLOR_BUFFER_BIT);
}

void init();

void start(DisplayFunc);

void repaint();

void dot(float x = 0, float y = 0);

void line(float, float, float, float);
void circle(float x, float y, float radius, int);
void circle(float x = 0, float y = 0, float radius = 0.5);
void circle_fill(float x = 0, float y = 0, float radius = 0.5);

/*
struct Color
{
	Color(float r_, float g_, float b_): r(r_), g(g_), b(b_) {}

	float r, g, b;
};
*/

namespace font
{
	struct bitmap_8_13 {};
} // namespace font

struct Pos
{
	Pos(): x(0), y(0) {}
	Pos(float x_, float y_): x(x_), y(y_) {}
	float x, y;
};

class TextOut {};
class End{};

void rect(float x1, float y1, float x2, float y2);
void rect_fill(float x1, float y1, float x2, float y2);

template<typename T>
TextOut& operator<<(TextOut& o, const T& v)
{
	std::stringstream str;
	str << v;
	const  std::string& s = str.str();
	for(size_t i = 0; i < s.size(); ++i)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, s[i]);

	return o;
}

void window();

void window(int, int);


extern TextOut text_out;
extern int vertex_group_type;
extern End end;
} // namespace cnv

#endif // CANVAS_HPP
