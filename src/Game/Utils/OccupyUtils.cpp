#include "OccupyUtils.hpp"

#include <Game/Component/OccupyingComponent.hpp>
#include <Game/IUpdateContext.hpp>
#include <Game/Map.hpp>
#include <Game/MapObjects/MapObject.hpp>

namespace sw
{

	bool IsOccupied(IUpdateContext& context, const Point& pos)
	{
		auto Ids = context.GetMap()->GetIdsAt(pos);

		return std::ranges::any_of(
			Ids,
			[&context](const auto& id)
			{ return context.GetMapObject(id)->template GetComponent<OccupyingComponent>() != nullptr; });
	}

}
