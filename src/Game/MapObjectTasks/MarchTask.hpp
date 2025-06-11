#pragma once
#include "MapObjectTask.hpp"

#include <Game/Geometry/Point.hpp>
#include <cstdint>

namespace sw
{

	class MarchTask : public MapObjectTask
	{
	public:
		MarchTask(MapObject& _parent, uint32_t _priority, const Point& target);

		~MarchTask() override;

		bool Update() override;

		bool IsComplete() const override;

	private:
		Point m_start;
		Point m_target;
	};

}
