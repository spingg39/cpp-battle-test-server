#include "Hunter.hpp"

#include <Game/Component/AgilityComponent.hpp>
#include <Game/Component/HealthComponent.hpp>
#include <Game/Component/MeleeAttackComponent.hpp>
#include <Game/Component/MovableComponent.hpp>
#include <Game/Component/OccupyingComponent.hpp>
#include <Game/Component/RangeAttackComponent.hpp>
#include <Game/Component/RangeComponent.hpp>
#include <Game/Component/StrengthComponent.hpp>
#include <Game/IUpdateContext.hpp>
#include <Game/Map.hpp>
#include <IO/Commands/SpawnHunter.hpp>

namespace sw
{
	Hunter::Hunter(const io::SpawnHunter& data) :
			MapObject(data.unitId, "Hunter", data.x, data.y)
	{
		AddComponent<HealthComponent>(data.hp);
		AddComponent<StrengthComponent>(data.strength);
		AddComponent<AgilityComponent>(data.agility);
		AddComponent<RangeComponent>(data.range);
		AddComponent<MovableComponent>(1);
		AddComponent<OccupyingComponent>();
		AddComponent<MeleeAttackComponent>();
		AddComponent<RangeAttackComponent>();
	}

	bool Hunter::Update(IUpdateContext& context)
	{
		auto hpComp = GetComponent<HealthComponent>();
		if (hpComp && hpComp->IsDead())
		{
			return false;
		}

		auto UnitsInMeleeRange = context.GetMap()->GetIdsInArea(GetPos(), 1, 1);
		if (UnitsInMeleeRange.empty())
		{
			if (auto range = GetComponent<RangeAttackComponent>())
			{
				if (range->TryAttack(*this, context))
				{
					return true;
				}
			}
		}

		if (auto melee = GetComponent<MeleeAttackComponent>())
		{
			if (melee->TryAttack(*this, context))
			{
				return true;
			}
		}

		if (auto mov = GetComponent<MovableComponent>())
		{
			if (mov->TryMove(*this, context))
			{
				return true;
			}
		}

		return false;
	}
}
