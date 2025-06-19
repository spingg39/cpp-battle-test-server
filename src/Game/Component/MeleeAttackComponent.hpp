#pragma once
#include <Game/Component/IComponent.hpp>

namespace sw
{

	class MapObject;
	struct IUpdateContext;

	class MeleeAttackComponent : public IComponent
	{
	public:
		MeleeAttackComponent();

		bool TryAttack(MapObject& self, IUpdateContext& context);
	};

}
