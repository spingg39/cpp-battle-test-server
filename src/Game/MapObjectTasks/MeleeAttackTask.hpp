#pragma once
#include "MapObjectTask.hpp"

namespace sw
{

	struct MeleeAttackTask : public MapObjectTask
	{
		MeleeAttackTask(MapObject& _parent, uint32_t _priority);

		bool Update() override;

		bool IsComplete() const override;
	};

}
