#include "board.hpp"
#include "util.hpp"

using namespace std;

Board::Board()
{
	selected_ = 0;
	cells_.resize (SIZE*SIZE);
	animating_ = false;
}

void Board::tick (Action action, int x, int y)
{
	selected_unit().tick (action); // передаёт выбранному юниту tick с его действием

	for (int i = 0; i < army_size(); ++i) // а всем остальным юнитам - с бездействием
		if (i != selected()) army_.at(i).tick (IDLE);

	if (action == ATTACK) // анимирует взрыв 
	{
		animator_.tick (EXPLOSION, x, y);
		animating_ = !animator_.complete();
	}
}

void Board::draw() const
{

	for (int i = 0; i < army_size(); ++i) // вызывает прорисовку юнитов
		army_.at(i).draw (i == selected_); // отправит true, если i юнит выбран
	if (animating_) animator_.draw(); // рисует взрыв
/*
	color (0, 0, 0);
	for (int i = 0; i < HEIGHT/SIZE; ++i) // сетка
		line (0, i*HEIGHT/SIZE, HEIGHT, i*HEIGHT/SIZE);
	for (int i = 0; i < WIDTH/SIZE; ++i)
		line (i*WIDTH/SIZE, 0, i*WIDTH/SIZE, WIDTH);
*/
}

void Board::state (int x, int y, int s) // изменяет состояние клетки
{
	int pos = SIZE*y + x;

	if (!s) cells_.at(pos) -= cells_.at(pos) % 10;
	else cells_.at(pos) += s;

	/* board[i] может принимать значения {0, 1, 2, 10, 11, 12}
	   Первая цифра обознчает состояние клетки: свобонда или занята (0, 1)
	   Вторая - возможное действие с клеткой: ничего нельзя, можно пойти или атаковать (0, 1, 2)
	*/
}

void Board::clear()
{
	for (int x = 0; x < SIZE; ++x) // сбрасывает возможные действия с прошлого хода
		for (int y = 0; y < SIZE; ++y)	
			state (x, y, 0);
}

int Board::action (int x, int y) const // возвращает возможное действие с клеткой
{
	int pos = SIZE*y + x;
	return cells_.at(pos) % 10;
}

bool Board::empty (int x, int y) const // проверяет клетку на отсутствие юнита
{
	int pos = SIZE*y + x;
	return !(cells_.at(pos) / 10);
}

int Board::next_unit() // возвращает номер следующего живого юнита
{
	if (selected_ < 0) return -1;

	for (int i = selected_; i < army_size(); i += 2)
		if (unit(i).ap()) return i;
	return -1;
}

void Board::attack (int x, int y)
{
	for (int i = 0; i < army_size(); ++i)
		if (unit(i).x() == x && unit(i).y() == y && unit(i).alive())
		{
			selected_unit().attack (unit(i));

			if (!unit(i).alive()) // освобождает клетку под убитым юнитом
				state (unit(i).x(), unit(i).y(), -10);
			break;
		}
}

void Board::move (int x, int y)
{
	state (selected_unit().x(), selected_unit().y(), -10);
	selected_unit().move (x, y);
	state (x, y, 10);
}

void Board::possible_move (int x, int y, int ap) // рекурсивно ищет возможные клетки для хода
{	
	if (x >= 1 && empty (x-1, y) && !action(x-1, y))
	{
		state (x-1, y, MOVE);
		if (ap > 1) possible_move (x-1, y, ap-1);
	}

	if (SIZE - x > 1 && empty (x+1, y) && !action(x+1, y))
	{
		state (x+1, y, MOVE);
		if (ap > 1) possible_move (x+1, y, ap-1);
	}

	if (y >= 1 && empty (x, y-1) && !action(x, y-1))
	{
		state (x, y-1, MOVE);
		if (ap > 1) possible_move (x, y-1, ap-1);
	}

	if (SIZE - y > 1 && empty (x, y+1) && !action(x, y+1))
	{
		state (x, y+1, MOVE);
		if (ap > 1) possible_move (x, y+1, ap-1);
	}
}
 
void Board::possible_attack (Warrior& w) // ищет возможные клетки для атаки в диапазоне range
{
	int min_x = w.x(), min_y = w.y();
   	int max_x = w.x(), max_y = w.y();

	for (int i = 1; i <= w.range(); ++i)
	{											
		if (w.x() >= i)
			--min_x;
		if (w.y() >= i)
			--min_y;
		if (SIZE - w.x() > i)
			++max_x;
		if (SIZE - w.y() > i)
			++max_y;
	}

	for (int i = 0; i < army_.size(); ++i)
		if (unit(i).x() >= min_x && unit(i).x() <= max_x && unit(i).y() >= min_y && unit(i).y() <= max_y && unit(i).side() != w.side() && unit(i).alive())
			state (unit(i).x(), unit(i).y(), ATTACK);
}
