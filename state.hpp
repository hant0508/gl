#ifndef STATE_HPP
#define STATE_HPP

#include <queue>
#include "util.hpp"

class Fight; 
class Board; 

class State
{
public:
	State (Fight& f): fight_ (f), finished_(false) {}
	virtual ~State () {}

	bool finished () const { return finished_; }

	virtual void on_tick();

	virtual void on_paint() {}
	virtual void on_turn() {}
	virtual void on_mouse (int button, int state, int x, int y) {}
	virtual void on_keyboard (unsigned char key) {}

protected:
	Board& board();

	Fight& fight_;
	bool finished_;
};

class PlayerTurn: public State
{
public:
	PlayerTurn (Fight& f): State(f) {}
	void on_paint();
	void on_keyboard (unsigned char key);
	void on_mouse (int button, int state, int x, int y);
};

class AiTurn: public State
{
public:
	AiTurn (Fight& f): State(f) {}
	void on_paint();
	void on_turn();
};

class Animation: public State
{
public:
	Animation (Fight& f, Action a, int x, int y): State(f), action_(a), x_(x), y_(y) {}
	void on_tick();
	void on_paint();
	void on_keyboard (unsigned char key);

private:
	Action action_;
	int x_, y_;
};

#endif // STATE_HPP
