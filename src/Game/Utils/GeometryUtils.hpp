#pragma once
#include "Point.hpp"

namespace sw
{

	uint32_t Dist(uint32_t a, uint32_t b);

	Point Dist(const Point& a, const Point& b);

	Point ShiftTo(Point start, Point target, uint32_t max_delta);

}
