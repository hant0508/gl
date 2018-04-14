#ifndef BITMAPS_HPP
#define BITMAPS_HPP

#include <map>
#include <vector>
#include "glcanvas.hpp"

typedef int Id;

class Bitmaps
{
public:
	static Bitmaps& inst()
	{
		static Bitmaps b;
		return b;
	}

	int size (Id id) const;

	void draw (Id id, int frame, int x, int y, int scale) const;
	void load();

private:
	Bitmaps() {}
	Bitmaps (const Bitmaps&) {}
	Bitmaps& operator= (const Bitmaps&);

	std::vector<cnv::Image*> find (Id id) const;

	std::map< Id, std::vector<cnv::Image*> > bitmaps_;
};

namespace bitmaps {

void load ();
void draw (Id id, int frame, int x, int y, int scale);
int size (Id id);

} // namespace bitmaps

#endif // BITMAPS_HPP
