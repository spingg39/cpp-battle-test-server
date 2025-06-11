#pragma once
#include "MapObjectTask.hpp"

namespace sw
{

	struct RangeAttackTask : public MapObjectTask
	{
		RangeAttackTask(MapObject& _parent, uint32_t _priority);

		bool Update() override;

		bool IsComplete() const override;

		static const uint32_t MIN_RANGE;
	};

}
