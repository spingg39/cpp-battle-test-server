#include "AttackUtils.hpp"

#include <Game/Component/HealthComponent.hpp>
#include <Game/IUpdateContext.hpp>
#include <Game/Map.hpp>
#include <Game/MapObjects/MapObject.hpp>

namespace sw
{

	bool AttackImpl(
		IUpdateContext& context, MapObject& attacker, uint32_t damage, uint32_t minRadius, uint32_t maxRadius)
	{
		std::vector<uint32_t> Targets;
		for (const auto& id : context.GetMap()->GetIdsInArea(attacker.GetPos(), minRadius, maxRadius))
		{
			auto obj = context.GetMapObject(id);
			if (obj->GetId() != attacker.GetId() && obj->GetComponent<HealthComponent>()
				&& !obj->GetComponent<HealthComponent>()->IsDead())
			{
				Targets.push_back(id);
			}
		}

		if (Targets.empty())
		{
			return false;
		}

		uint32_t index = context.GetRandom(0, Targets.size() - 1);
		auto target = context.GetMapObject(Targets[index]);

		return target->GetComponent<HealthComponent>()->ReceiveDamage(*target, context, attacker, damage);
	}

}
