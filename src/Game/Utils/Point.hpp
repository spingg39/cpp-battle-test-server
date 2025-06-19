#pragma once
#include <compare>
#include <cstdint>

namespace sw
{

	struct Point
	{
		uint32_t x;
		uint32_t y;

		auto operator<=>(const Point&) const = default;
	};

}
