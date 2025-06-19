#include "RangeAttackComponent.hpp"

#include <Game/Component/AgilityComponent.hpp>
#include <Game/Component/RangeComponent.hpp>
#include <Game/MapObjects/MapObject.hpp>
#include <Game/Utils/AttackUtils.hpp>
#include <cassert>
#include <stdexcept>

namespace sw
{

	const uint32_t RangeAttackComponent::MIN_RANGE = 2U;

	RangeAttackComponent::RangeAttackComponent() = default;

	bool RangeAttackComponent::TryAttack(MapObject& self, IUpdateContext& context)
	{
		assert(self.GetComponent<AgilityComponent>());
		assert(self.GetComponent<RangeComponent>());

		auto agi = self.GetComponent<AgilityComponent>()->GetAgility();
		auto range = self.GetComponent<RangeComponent>()->GetRange();

		return AttackImpl(context, self, agi, MIN_RANGE, range);
	}
}
