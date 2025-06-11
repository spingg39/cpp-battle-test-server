#include "TaskUtils.hpp"

#include <Game/Game.hpp>
#include <Game/Map.hpp>
#include <Game/MapObject.hpp>

namespace sw
{

	bool AttackImpl(MapObject& attacker, uint32_t damage, uint32_t minRadius, uint32_t maxRadius)
	{
		auto TargetPredicate = [&attacker](const MapObject& obj)
		{
			return obj.GetId() != attacker.GetId() && obj.CanReceiveDamage() && !obj.IsDead();
		};

		auto Targets = attacker.GetMap()->GetObjectsInArea(attacker.GetPos(), minRadius, maxRadius, TargetPredicate);
		if (Targets.empty())
		{
			return false;
		}

		uint32_t index = attacker.GetMap()->GetGame().GetRandom(0, Targets.size() - 1);
		auto& target = Targets[index];

		target->ReceiveDamage(attacker, damage);

		return true;
	}

}
