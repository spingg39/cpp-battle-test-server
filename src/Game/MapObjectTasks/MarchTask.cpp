#include "MarchTask.hpp"

#include <Game/Game.hpp>
#include <Game/Geometry/Utils.hpp>
#include <Game/Map.hpp>
#include <Game/MapObject.hpp>
#include <stdexcept>

namespace sw
{

	MarchTask::MarchTask(MapObject& _parent, uint32_t _priority, const Point& target) :
			MapObjectTask(_parent, _priority),
			m_start(_parent.GetPos()),
			m_target(target)
	{
		if (_parent.GetMoveSpeed() == 0)
		{
			throw std::runtime_error(
				"Can't create MarchTask: Object hasn't MoveSpeed property: " + std::to_string(_parent.GetId()));
		}

		_parent.GetMap()->GetGame().OnEvent(
			io::MarchStarted(_parent.GetId(), m_start.x, m_start.y, m_target.x, m_target.y));
	}

	MarchTask::~MarchTask() = default;

	bool MarchTask::Update()
	{
		if (IsComplete())
		{
			return false;
		}

		const uint32_t speed = GetMapObject().GetMoveSpeed();

		auto nextPos = ShiftTo(GetMapObject().GetPos(), m_target, speed);

		bool moved = GetMapObject().GetMap()->TryMoveObject(GetMapObject(), nextPos);
		if (moved && IsComplete())
		{
			GetMapObject().GetMap()->GetGame().OnEvent(io::MarchEnded(GetMapObject().GetId(), m_target.x, m_target.y));
		}

		return moved;
	}

	bool MarchTask::IsComplete() const
	{
		return GetMapObject().IsDead() || GetMapObject().GetPos() == m_target;
	}

}
