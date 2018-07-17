#include "state.hpp"
#include "fight.hpp"
#include "board.hpp"
#include "glcanvas.hpp"

using namespace std;
using namespace cnv;

Board& State::board()
{
	return fight_.board();
}

void State::on_tick()
{
	board().tick(IDLE, 0, 0); // постоянно отправляет юнитам tick, чтобы они иногда делали фейспалм
}

/*** PlayerTurn ***/

void PlayerTurn::on_paint()
{
	board().draw(); // рисует фон и юнитов

	for (int i = 0; i < SIZE; ++i) // рисует возможные действия для юнитов игрока
		for (int j = 0; j < SIZE; ++j)
		{
			int x = i * WIDTH/SIZE + WIDTH/2/SIZE;
			int y = j * HEIGHT/SIZE + HEIGHT/2/SIZE + 30;

			if (board().action (i, j) == MOVE) // если можно пойти
			{	
				color (0, 0, 1);
				circle_fill (x, y, 3);
			}

			else if (board().action (i, j) == ATTACK) // если можно атаковать
			{
				color (0, 1, 0);
				circle_fill (x, y, 3);
			}
		}
}

void PlayerTurn::on_mouse (int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) // клик ЛКМ
	{
		fight_.act (x, y, board()); // делает ход

		// добавляет в очередь анимацию атаки или движения
		if (board().action (x, y) == MOVE) fight_.push_state (new Animation (fight_, MOVE, x, y));
		else if (board().action (x, y) == ATTACK) fight_.push_state (new Animation (fight_, ATTACK, x, y));
		
		// и следующий ход
		if (board().next_unit() >= 0) fight_.push_state (new PlayerTurn (fight_)); // пока есть юниты игрока
		else fight_.push_state (new AiTurn (fight_));

		finished_ = true;
	}
}

void PlayerTurn::on_keyboard (unsigned char key)
{
	if (key == ' ') // пропуск хода
	{
		board().selected_unit().reset_ap(0); // собственно, пропускает ход

		// добавляет в очередь следующий ход
		if (board().next_unit() >= 0) fight_.push_state (new PlayerTurn (fight_));
		else fight_.push_state (new AiTurn (fight_));

		fight_.select (board().next_unit(), board()); // выбирает следующий юнит

		finished_ = true;
	}
}	

/*** AiTurn ***/

void AiTurn::on_paint()
{
	board().draw(); // рисует фон и юнитов, не рисует точки
}

void AiTurn::on_turn()
{
	fight_.ai(); // вызывает ИИ, который делает ход

	if (board().next_unit() >= 0) fight_.push_state (new AiTurn (fight_)); // пока не закончились юниты бота
	else fight_.push_state (new PlayerTurn (fight_));

	finished_ = true;
}

/*** Animation ***/

void Animation::on_paint()
{
	board().draw();
}

void Animation::on_tick()
{
	board().tick (action_, x_, y_);

	finished_ = (action_ == ATTACK ? board().animated() : board().selected_unit().animated());

	if (finished_) fight_.select (board().next_unit(), board()); // выбирает следующий юнит
}
