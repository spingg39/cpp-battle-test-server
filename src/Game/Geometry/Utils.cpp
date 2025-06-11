#include "Utils.hpp"

#include <algorithm>

namespace sw
{

	uint32_t Dist(uint32_t a, uint32_t b)
	{
		return a >= b ? a - b : b - a;
	}

	Point Dist(const Point& a, const Point& b)
	{
		return Point(Dist(a.x, b.x), Dist(a.y, b.y));
	}

	Point ShiftTo(Point start, Point target, uint32_t max_delta)
	{
		uint32_t delta_x = std::min(Dist(target.x, start.x), max_delta);
		uint32_t delta_y = std::min(Dist(target.y, start.y), max_delta);

		uint32_t next_x = target.x >= start.x ? start.x + delta_x : start.x - delta_x;
		uint32_t next_y = target.y >= start.y ? start.y + delta_y : start.y - delta_y;

		return Point(next_x, next_y);
	}

}
