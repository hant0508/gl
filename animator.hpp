#ifndef ANIMATOR_HPP
#define ANIMATOR_HPP

#include "util.hpp"

class Animator
{
public:
	Animator(): it_(1), complete_(false) {}

	void init (Id id, int x, int y, int f);
	void move (int cur_x, int to_x, int cur_y, int to_y);

	void tick (Id id, int to_x, int to_y, bool flip);
	void tick (Id id, int to_x, int to_y);
	void draw() const;
	void text_out (bool selected, Side side, int health, int ap) const;

	int convert (int c, int l) const { return c * l/SIZE + l/2/SIZE; }
	bool complete () const { return complete_; }

private:
	Id id_;
  	int	x_, y_, old_x_, old_y_, frame_, it_;
	bool complete_;
};

#endif // ANIMATOR_HPP
