#include "RangeAttackTask.hpp"

#include "TaskUtils.hpp"

#include <Game/MapObject.hpp>
#include <stdexcept>

namespace sw
{

	const uint32_t RangeAttackTask::MIN_RANGE = 2U;

	RangeAttackTask::RangeAttackTask(MapObject& _parent, uint32_t _priority) :
			MapObjectTask(_parent, _priority)
	{
		if (!_parent.GetAgility().has_value())
		{
			throw std::runtime_error(
				"Can't create RangeAttack: Object hasn't agility property: " + std::to_string(_parent.GetId()));
		}
		if (!_parent.GetRange().has_value())
		{
			throw std::runtime_error(
				"Can't create RangeAttack: Object hasn't range property: " + std::to_string(_parent.GetId()));
		}
		if (_parent.GetRange() < MIN_RANGE)
		{
			throw std::runtime_error(
				"Can't create RangeAttack: range property < MIN_RANGE: " + std::to_string(_parent.GetId()));
		}
	}

	bool RangeAttackTask::Update()
	{
		return AttackImpl(GetMapObject(), *GetMapObject().GetAgility(), MIN_RANGE, *GetMapObject().GetRange());
	}

	bool RangeAttackTask::IsComplete() const
	{
		return false;
	}

}
