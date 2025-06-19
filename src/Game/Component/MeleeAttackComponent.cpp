#include "MeleeAttackComponent.hpp"

#include <Game/Component/StrengthComponent.hpp>
#include <Game/MapObjects/MapObject.hpp>
#include <Game/Utils/AttackUtils.hpp>
#include <cassert>
#include <stdexcept>

namespace sw
{

	MeleeAttackComponent::MeleeAttackComponent() = default;

	bool MeleeAttackComponent::TryAttack(MapObject& self, IUpdateContext& context)
	{
		assert(self.GetComponent<StrengthComponent>());

		auto str = self.GetComponent<StrengthComponent>()->GetStrength();
		return AttackImpl(context, self, str, 1, 1);
	}
}
