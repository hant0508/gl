#ifndef FIGHT_HPP
#define FIGHT_HPP

#include <queue>
#include "board.hpp"

class State; 

class Fight
{
public:
	static Fight &inst()
	{
		static Fight fight;
		return fight;
	}

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
	Fight() {}
	Fight (const Fight&) {}
	Fight &operator= (const Fight&);

	Id background_;
	Board board_;
	std::queue<State*> states_;
};

namespace fight
{
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

	Board& board();
	void push_state (State* s);
	Id background();
	} // namespace fight

#endif // FIGHT_HPP
