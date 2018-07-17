#include <cassert>
#include <cstdlib>
#include "bitmaps.hpp"
#include "fight.hpp"
#include "state.hpp"
 
using namespace std;

void Fight::tick()
{
	states_.front()->on_tick();
	next_state();
}

void Fight::draw() 
{
	bitmaps::draw (BATTLE_BG, background_, 0, 0, 600);

	states_.front()->on_paint();

	// тут будет рисоваться что-то ещё
}

void Fight::next_state () // удаляет завершённые состояния из очереди
{
	if (states_.front()->finished())
	{
		delete states_.front();
		states_.pop();
		assert (!states_.empty() && "Fight: states_ is empty");
		states_.front()->on_turn();
	}
}

void Fight::start ()
{
	for (int i = 0; i < SIZE*SIZE; ++i) // расставляет юниты
	{
		if (i % SIZE == 0) // игрока (слева)
		{
			Warrior warrior_p (i%SIZE, i/SIZE, PLAYER, rand()%2+1);
			board_.push (warrior_p);
			board_.state (i%SIZE, i/SIZE, 10);
		}

		if (i % SIZE == (SIZE-1)) // и бота (справа)
		{
			Warrior warrior_b (i%SIZE, i/SIZE, AI, rand()%2+1);
			board_.push (warrior_b);
			board_.state (i%SIZE, i/SIZE, 10);
		}
	}

	background_ = rand() % 5; // выбирает фон
	states_.push (new PlayerTurn (*this)); // добавляет в очередь ход игрока
	select (0, board_); // выбирает первый юнит игрока
}

void Fight::select (int num, Board& b)
{
/*
   Функция получает номер юнита, которым мы хотим пойти и ссылку на борду, на которой нужно сделать ход.
   Она записывает номер этого юнита в board::selected() и расставляет на борде все возможные ходы для него.

   Если вместо номера юнита пришла -1, функция пытается передать ход другой стороне;
   в случае невозможности завершает игру.
 */

	if (num == -1) // если сделали ход все юниты
	{
		for (int i = 0; i < b.army_size(); ++i) // восстанавливает АП юнитов противника
			if (b.unit(i).side() == !b.selected_unit().side() && b.unit(i).alive())
				b.unit(i).reset_ap (1);

		b.select (!(b.selected() % 2)); // и передаёт ход другой стороне
		num = b.next_unit();

		if (num == -1) exit(0); // если передавать некому, бой окончен
	}

	Warrior& w = b.unit(num);

	b.clear(); // сбрасывает значения с прошлого хода
	b.select (num); // записывает номер юнита в board::selected()
	b.possible_move (w.x(), w.y(), w.ap()); // расставляет на борде возможные позиции для хода юнита
	b.possible_attack (w);
}

int Fight::value (Board before, Board after) // оценка выгодности хода по изменению ХП
{
	int p1 = 0, c1 = 0, p2 = 0, c2 = 0;

	for(int i = 0; i < before.army_size(); ++i) // считает общее ХП "до" и "после"
	{
		if (i%2 == 0) // игрока
		{
			p1 += before.unit(i).health();
			p2 += after.unit(i).health();
		}

		else // и компьютера
		{
			c1 += before.unit(i).health();
			c2 += after.unit(i).health();
		}
	}

	if (before.selected_unit().side() == PLAYER) return c1 - c2 + 1; // и возвращает разность
	return p1 - p2 + 1; // 1 прибавляется для исключения нулевых значений
}

Board Fight::make_move()
{
	Board b = board_;

	vector<int> v (1); // тут лежит та самая "прямая", в "отрезок" которой мы рандомно тыкаем
	vector<int> a (1); // а тут - номера клеток, соответствующих каждому "отрезку"
	int sel = -1; // номер выбранной клетки

	for (int y = 0; y < SIZE; ++y)
		for (int x = 0; x < SIZE; ++x)
			if (b.action(x,y)) // ищет на поле клетки, с которыми можно взаимодействовать
			{
				Board t = b;
				act (x, y, t); // ходит на копии борды

				v.push_back (v.back() + value (b, t)); // сохраняет оценку хода
				a.push_back (x + y*SIZE); // и номер клетки, в которую был сделан ход
			}

	int s;
	if (v.back()) s = rand() % v.back(); // выбирает случайное значение
	else b.selected_unit().reset_ap(0); // пропускает ход, если идти некуда

	for (size_t i = 1; i < a.size(); ++i) // ищет "отрезок", в который это значение попадает
		if (v[i] >= s)
		{
		   	sel = a[i]; // и выбирает соответсвующее действие
			break;
		}

	const int x = sel%SIZE;
	const int y = sel/SIZE;

	// добавляет в очередь анимацию атаки или движения
	if (b.action (x, y) == MOVE) states_.push (new Animation (*this, MOVE, x, y));
	else if (b.action (x, y) == ATTACK) states_.push (new Animation (*this, ATTACK, x, y));

	if (sel >= 0) act (x, y, b); // делает выбранный ход

	return b;
}

void Fight::ai ()
{
	board_ = make_move();
	next_state();
}

void Fight::act (int x, int y, Board& b)
{
	if (b.action (x, y) == MOVE) // движение
		b.move (x, y);

	else if (b.action (x, y) == ATTACK) // атака
		b.attack (x, y);
}

void Fight::mouse (int button, int state, int x, int y)
{
	const int cell_x = x/(WIDTH/SIZE), cell_y = y/(HEIGHT/SIZE);

	states_.front()->on_mouse (button, state, cell_x, cell_y);
	next_state();
}

void Fight::keyboard (unsigned char key)
{
	states_.front()->on_keyboard (key);
	next_state();

	if (key == 'q') exit(0); // выход из игры
}
