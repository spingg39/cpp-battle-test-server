#pragma once

#include "IComponent.hpp"

#include <Game/Utils/Point.hpp>
#include <optional>

namespace sw
{
	class MapObject;
	struct IUpdateContext;

	class MovableComponent : public IComponent
	{
	public:
		MovableComponent(uint32_t _moveSpeed);

		~MovableComponent();

		bool TryMove(MapObject& self, IUpdateContext& context);

		uint32_t GetMoveSpeed() const;

		void SetTarget(const Point& pos);

	private:
		uint32_t moveSpeed;
		std::optional<Point> target;
	};
}
