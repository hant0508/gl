#include "glcanvas.hpp"
#include "animator.hpp"

using namespace cnv;
using namespace std;

void Animator::init (Id id, int x, int y, int f) // задаёт начальные значения
{
	x_ = convert (x, bitmaps::getWindowSize()); // convert переводит координаты из игровых в оконные
	y_ = convert (y, bitmaps::getWindowSize());
	old_x_ = x_;
	old_y_ = y_;

	id_ = id;
	frame_ = f;
}

void Animator::move (int cur_x, int to_x, int cur_y, int to_y)
{
	// вызывется юнитом при его передвижении
	
	it_ = max (abs(cur_x-to_x), abs(cur_y-to_y)); // количество циклов, которые нужно выполнить
	// обычно он один, но при движении на несколько клеток их будет больше

	x_ = convert (cur_x, bitmaps::getWindowSize());
	y_ = convert (cur_y, bitmaps::getWindowSize());
	old_x_ = x_;
	old_y_ = y_;
}

void Animator::tick (Id id, int to_x, int to_y, bool flip) // tick для юнитов
{
	if ((frame_&1) && !flip) --frame_; // поворачивает кадр при необходимости
	else if (!(frame_&1) && flip) ++frame_;
	frame_ += 2; // переключает кадр (через один, потому что пак содержит отражённые кадры)

	id_ = id;
	int frames = bitmaps::size (id_);

	to_x = convert (to_x, bitmaps::getWindowSize());
	to_y = convert (to_y, bitmaps::getWindowSize());

	x_ += (to_x - old_x_)/(frames/2*it_);
	y_ += (to_y - old_y_)/(frames/2*it_);

	if (frame_ >= frames) // если кончились кадры
	{
		if (!--it_) // если все циклы выполнены 
		{
			complete_ = true;
			x_ = to_x;
			y_ = to_y;
			it_ = 1;
		}

		frame_ = flip;
		old_x_ = x_;
		old_y_ = y_;
	}

	else complete_ = false;
}

void Animator::tick (Id id, int x, int y) // tick для более простых кадров (взрыв и т.д.) 
{
	frame_++; // тут зеркальных кадров нет
	id_ = id;
	int frames = bitmaps::size (id_);

	x_ = convert (x, bitmaps::getWindowSize());
	y_ = convert (y, bitmaps::getWindowSize());

	if (frame_ >= frames) // цикл всегда один
	{
		frame_ = 0;
		complete_ = true;
	}

	else complete_ = false;
}

void Animator::draw() const
{
	bitmaps::draw (id_, frame_, x_, y_);
}

void Animator::text_out (bool selected, Side side, int health, int ap) const
{
	if (selected) color (0, 0, 0.8); // цвет выбранного юнита
	else if (side == AI) color (0.8, 0, 0); // цвет юнитов противника
	else if (side == PLAYER) color (0.8, 0.9, 0); // цвет юнитов игрока

	size_t sz = bitmaps::getWindowSize();
	position (x_ + sz/30, y_); // выводит ХП и АП юнита
	cnv::text_out << "hp:" << health;
	position (x_ + sz/24, y_ + sz/40);
	cnv::text_out << "ap:" << ap;
}
