#pragma once
#include <optional>
#include <unordered_map>

namespace sw
{

	struct TaskConfig
	{
		uint32_t Priority{0};
	};

	struct MapObjectConfig
	{
		uint32_t MoveSpeed = 1;
		bool OccupyCell = true;

		std::optional<TaskConfig> MeleeAttack;
		std::optional<TaskConfig> RangeAttack;
	};

	inline const TaskConfig DefaultMeleeAttackConfig{.Priority = 0};
	inline const TaskConfig DefaultRangeAttackConfig{.Priority = 1};
	inline const TaskConfig DefaultMarchConfig{.Priority = 2};

	inline const std::unordered_map<std::string, MapObjectConfig> MapObjectConfigs{
		{"Swordsman", {.MeleeAttack = DefaultMeleeAttackConfig}},
		{"Hunter", {.MeleeAttack = DefaultMeleeAttackConfig, .RangeAttack = DefaultRangeAttackConfig}},
	};

}
