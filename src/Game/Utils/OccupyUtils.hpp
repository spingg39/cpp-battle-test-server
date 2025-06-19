#pragma once

namespace sw
{

	struct IUpdateContext;
	struct Point;

	bool IsOccupied(IUpdateContext& context, const Point& pos);

}
