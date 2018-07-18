#ifndef BITMAPS_HPP
#define BITMAPS_HPP

#include <map>
#include <vector>
#include "util.hpp"
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
	size_t getWindowSize() const { return windowSize; }
	void setWindowSize (size_t size) { windowSize = size; }
	float getScale() const { return windowSize / DEFAULT_WINDOW_SIZE; }

	void draw (Id id, int frame, int x, int y) const;
	void load();


private:
	Bitmaps() {}
	Bitmaps (const Bitmaps&) {}
	Bitmaps& operator= (const Bitmaps&);

	size_t windowSize;
	std::vector<cnv::Image*> find (Id id) const;
	std::map< Id, std::vector<cnv::Image*> > bitmaps_;
};

namespace bitmaps {

void load ();
void draw (Id id, int frame, int x, int y);
int size (Id id);
size_t getWindowSize();
void setWindowSize (size_t size);
float getScale();

} // namespace bitmaps

#endif // BITMAPS_HPP
