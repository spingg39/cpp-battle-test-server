#include "MeleeAttackTask.hpp"

#include "TaskUtils.hpp"

#include <Game/MapObject.hpp>
#include <stdexcept>

namespace sw
{

	MeleeAttackTask::MeleeAttackTask(MapObject& _parent, uint32_t _priority) :
			MapObjectTask(_parent, _priority)
	{
		if (!_parent.GetStrength().has_value())
		{
			throw std::runtime_error(
				"Can't create MeleeAttack: Object hasn't strength property: " + std::to_string(_parent.GetId()));
		}
	}

	bool MeleeAttackTask::Update()
	{
		return AttackImpl(GetMapObject(), *GetMapObject().GetStrength(), 1, 1);
	}

	bool MeleeAttackTask::IsComplete() const
	{
		return false;
	}

}
