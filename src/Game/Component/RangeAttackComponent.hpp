#pragma once
#include <Game/Component/IComponent.hpp>

namespace sw
{
	class MapObject;
	struct IUpdateContext;

	struct RangeAttackComponent : public IComponent
	{
		RangeAttackComponent();

		bool TryAttack(MapObject& self, IUpdateContext& context);

		static const uint32_t MIN_RANGE;
	};

}
