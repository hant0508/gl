#include <cmath>
#include <cstdlib>
#include "warrior.hpp"

using namespace std;

Warrior::Warrior (int x, int y, Side side, UnitType type)
{
	health_ = 50;
	power_ = 10;
	speed_ = 2;
	range_ = 1;
	x_ = x;
	y_ = y;
	alive_ = true;
	side_ = side;
	type_ = type;
	ap_ = speed_;
	animated_ = true;

	if (side_ == PLAYER) flip_ = true; // поворачивает спрайт юнита направо или налево
	else flip_ = false;

	animator_.init (type_*10, x, y, flip_); // передаёт информацию о юните в аниматор

	resign (); // переназначает хар-ки юнита в зависимости от типа
}

void Warrior::tick (Action action) // прокручивает кадры юнита
{
	if (animated_ && action == IDLE && rand()%200 != 42)
		return; // фейспалм делает иногда, а не всё время

	Id id = type_*10 + action;
	animator_.tick (id, x_, y_, flip_);
	animated_ = animator_.complete();
}

void Warrior::redraw()
{
	animator_.init (type_*10, x_, y_, flip_);
}

void Warrior::draw (bool selected) const
{
	if (!alive_) return; // пропускает трупы

	animator_.draw(); // рисует юнита 
	animator_.text_out (selected, side_, health_, ap_); // и выводит его хар-ки
}

void Warrior::resign()
{
	if (type_ == ARCHER)
	{
		health_ = 30;
		speed_ = 2;
		range_ = SIZE;
		power_ = 10;
	}
}

void Warrior::damage (int dmg) // получает урон
{
	health_ -= dmg;

	if (health_ <= 0) // и убивает юнит
	{
		alive_ = false;
		ap_ = 0;
	}
}

void Warrior::attack (Warrior& enemy) // атакует другого юнита
{
	ap_ = 0;
	enemy.damage (power_);
}

void Warrior::move (int x, int y)
{
    ap_ -= (abs(x_-x) + abs(y_-y)); // уменьшает АП на модуль перемещения

	animator_.move (x_, x, y_, y); // сообщает аниматору о движении

	if (x > x_) flip_ = true; // поворачивает кадр, если юнит изменил направление движения
	else if (x < x_) flip_ = false;

	x_ = x;
	y_ = y;
}

void Warrior::reset_ap (bool r) // восстанавливает или обнуляет АП
{
	r == false ? ap_ = 0 : ap_ = speed_;
}
