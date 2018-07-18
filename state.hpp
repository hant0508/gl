#ifndef STATE_HPP
#define STATE_HPP

#include <queue>
#include "util.hpp"

class Fight; 
class Board; 

class State
{
public:
	State (): finished_(false) {}
	virtual ~State () {}

	bool finished () const { return finished_; }

	virtual void on_tick();

	virtual void on_paint() {}
	virtual void on_turn() {}
	virtual void on_mouse (int, int, int, int) {}
	virtual void on_keyboard (unsigned char) {}

protected:
	Board& board();

	bool finished_;
};

class PlayerTurn: public State
{
public:
	PlayerTurn (): State() {}
	void on_paint();
	void on_keyboard (unsigned char key);
	void on_mouse (int button, int state, int x, int y);
};

class AiTurn: public State
{
public:
	AiTurn (): State() {}
	void on_paint();
	void on_turn();
};

class Animation: public State
{
public:
	Animation (Action a, int x, int y): State(), action_(a), x_(x), y_(y) {}
	void on_tick();
	void on_paint();

private:
	Action action_;
	int x_, y_;
};

#endif // STATE_HPP
