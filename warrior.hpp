#ifndef WARRIOR_HPP
#define WARRIOR_HPP

#include "util.hpp"
#include "animator.hpp"

class Warrior
{
public:
	Warrior (int x, int y, Side side, UnitType type);

	void tick (Action action);
	void draw (bool selected) const;

	void resign();
	
	void damage (int dmg);
	void attack (Warrior& enemy);
	void move (int x, int y);

	void reset_ap (bool r);

	int x() const { return x_; }
	int y() const { return y_; }

	int speed() const { return speed_; }
	int health() const { return health_; }
	int range() const { return range_; }
	int ap() const { return ap_; }
	bool alive() const { return alive_; }
	bool animated() const { return animator_.complete(); }
	Side side() const { return side_; }
	UnitType type() const { return type_; }
	
private:
	int health_, power_, speed_, range_, x_, y_, ap_;
	bool alive_, flip_, animated_;
	Side side_;
	UnitType type_;
	Animator animator_;
};

#endif // WARRIOR_HPP
