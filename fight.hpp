#ifndef FIGHT_HPP
#define FIGHT_HPP

#include <queue>
#include "board.hpp"

class State; 

class Fight
{
public:
	void tick();
	void draw();
	void next_state();

	void start();
	void select (int num, Board& b);
	void act (int x, int y, Board& b);
	int value (Board before, Board after);
	Board make_move();
	void ai();

	void mouse (int button, int state, int x, int y);
	void keyboard (unsigned char key);

	Board& board() { return board_; }
	void push_state (State* s) { states_.push (s); }
	Id background () const { return background_; }

private:
	Id background_;
	Board board_;
	std::queue<State*> states_;
};

#endif // FIGHT_HPP
