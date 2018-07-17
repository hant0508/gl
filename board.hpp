#ifndef BOARD_HPP
#define BOARD_HPP

#include "warrior.hpp"
#include <vector>
#include <cstdlib>

class Board
{
public:
	Board();

	void tick (Action action, int x, int y);
	void draw() const;

	void clear();
	void push (Warrior w) { army_.push_back(w); }

	void state (int x, int y, int s);
	int action (int x, int y) const;
	bool empty (int x, int y) const;

	void select (int unit) { selected_ = unit; }
	int next_unit();

	void attack (int x, int y);
	void move (int x, int y);
	void possible_move (int x, int y, int ap);
	void possible_attack (Warrior& unit);

	Warrior& unit (int i) { return army_.at(i); }
	Warrior& selected_unit () { return army_.at(selected_); }
	int army_size() const { return army_.size(); }
	int selected() const { return selected_; }
	bool animated() const { return animator_.complete(); }

private:
	int selected_;
	std::vector<int> cells_;
	std::vector<Warrior> army_;
	bool animating_;
	Animator animator_;
};

#endif // BOARD_HPP
