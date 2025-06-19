#include "HealthComponent.hpp"

#include <Game/IUpdateContext.hpp>
#include <Game/MapObjects/MapObject.hpp>

namespace sw
{

	HealthComponent::HealthComponent(uint32_t _hp) :
			hp(_hp)
	{}

	uint32_t HealthComponent::GetHP() const
	{
		return hp;
	}

	void HealthComponent::SetHP(uint32_t _hp)
	{
		hp = _hp;
	}

	bool HealthComponent::IsDead() const
	{
		return hp == 0;
	}

	bool HealthComponent::ReceiveDamage(MapObject& self, IUpdateContext& context, MapObject& attacker, uint32_t damage)
	{
		if (IsDead())
		{
			return false;
		}

		SetHP(hp < damage ? 0 : hp - damage);

		context.OnEvent(io::UnitAttacked(attacker.GetId(), self.GetId(), damage, hp));

		if (IsDead())
		{
			context.OnEvent(io::UnitDied(self.GetId()));
		}
		return true;
	}

}
