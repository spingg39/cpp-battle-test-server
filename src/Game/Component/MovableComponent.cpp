#include "MovableComponent.hpp"

#include <Game/Component/HealthComponent.hpp>
#include <Game/Component/OccupyingComponent.hpp>
#include <Game/IUpdateContext.hpp>
#include <Game/Map.hpp>
#include <Game/MapObjects/MapObject.hpp>
#include <Game/Utils/GeometryUtils.hpp>
#include <Game/Utils/OccupyUtils.hpp>

namespace sw
{
	MovableComponent::MovableComponent(uint32_t _moveSpeed) :
			moveSpeed(_moveSpeed)
	{}

	MovableComponent::~MovableComponent() = default;

	bool MovableComponent::TryMove(MapObject& self, IUpdateContext& context)
	{
		if (!target.has_value())
		{
			return false;
		}

		auto nextPos = ShiftTo(self.GetPos(), *target, self.GetComponent<MovableComponent>()->GetMoveSpeed());

		if (self.GetPos() == nextPos)
		{
			target.reset();
			return false;
		}

		if (self.GetComponent<OccupyingComponent>() && IsOccupied(context, nextPos))
		{
			return false;
		}

		context.GetMap()->RemoveObject(self.GetPos(), self.GetId());
		self.SetPos(nextPos);
		context.GetMap()->AddObject(self.GetPos(), self.GetId());

		context.OnEvent(io::UnitMoved(self.GetId(), self.GetX(), self.GetY()));

		if (self.GetPos() == target)
		{
			context.OnEvent(io::MarchEnded(self.GetId(), target->x, target->y));
			target.reset();
		}

		return true;
	}

	uint32_t MovableComponent::GetMoveSpeed() const
	{
		return moveSpeed;
	}

	void MovableComponent::SetTarget(const Point& pos)
	{
		target = pos;
	}
}
