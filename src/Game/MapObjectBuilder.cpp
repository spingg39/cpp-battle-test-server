#include "MapObjectBuilder.hpp"

#include "MapObjectConfig.hpp"
#include "MapObjectTasks/MeleeAttackTask.hpp"
#include "MapObjectTasks/RangeAttackTask.hpp"

namespace sw
{

	MapObjectBuilder& MapObjectBuilder::WithCustomConfig(const MapObjectConfig& config)
	{
		CustomConfig = config;
		return *this;
	}

	const MapObjectConfig& MapObjectBuilder::GetDefaultConfig(const std::string& type)
	{
		auto itConfig = MapObjectConfigs.find(type);
		if (itConfig == MapObjectConfigs.end())
		{
			throw std::runtime_error("MapObjectBuilder::GetDefaultConfig: unknown type: " + type);
		}

		return itConfig->second;
	}

	const std::string& MapObjectBuilder::GetTypeName(const std::string& stateName)
	{
		static const std::unordered_map<std::string, std::string> CommandNameToType{
			{"SPAWN_SWORDSMAN", "Swordsman"},
			{"SPAWN_HUNTER", "Hunter"},
		};
		return CommandNameToType.at(stateName);
	}

	void MapObjectBuilder::ConfigureObject(MapObject& mapObject, const MapObjectConfig& config)
	{
		mapObject.SetMoveSpeed(config.MoveSpeed);
		mapObject.SetOccupyCell(config.OccupyCell);

		if (config.MeleeAttack.has_value())
		{
			mapObject.AddTask(std::make_unique<MeleeAttackTask>(mapObject, config.MeleeAttack->Priority));
		}

		if (config.RangeAttack.has_value())
		{
			mapObject.AddTask(std::make_unique<RangeAttackTask>(mapObject, config.RangeAttack->Priority));
		}
	}
}
