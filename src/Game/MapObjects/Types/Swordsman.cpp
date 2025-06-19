#include "Swordsman.hpp"

#include <Game/Component/AgilityComponent.hpp>
#include <Game/Component/HealthComponent.hpp>
#include <Game/Component/MeleeAttackComponent.hpp>
#include <Game/Component/MovableComponent.hpp>
#include <Game/Component/OccupyingComponent.hpp>
#include <Game/Component/RangeAttackComponent.hpp>
#include <Game/Component/RangeComponent.hpp>
#include <Game/Component/StrengthComponent.hpp>
#include <Game/IUpdateContext.hpp>
#include <IO/Commands/SpawnSwordsman.hpp>

namespace sw
{
	Swordsman::Swordsman(const io::SpawnSwordsman& data) :
			MapObject(data.unitId, "Swordsman", data.x, data.y)
	{
		AddComponent<HealthComponent>(data.hp);
		AddComponent<StrengthComponent>(data.strength);
		AddComponent<MovableComponent>(1);
		AddComponent<OccupyingComponent>();
		AddComponent<MeleeAttackComponent>();
	}

	bool Swordsman::Update(IUpdateContext& context)
	{
		auto hpComp = GetComponent<HealthComponent>();
		if (hpComp && hpComp->IsDead())
		{
			return false;
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
