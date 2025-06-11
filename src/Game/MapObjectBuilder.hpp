#pragma once
#include "MapObject.hpp"
#include "MapObjectConfig.hpp"
#include "PropertyChecker.hpp"

#include <stdexcept>
#include <unordered_map>

namespace sw
{
	class MapObjectBuilder
	{
	public:
		MapObjectBuilder& WithCustomConfig(const MapObjectConfig& config);

		template <typename State>
		std::shared_ptr<MapObject> Build(const State& state)
		{
			const std::string& type = GetTypeName(state.Name);
			auto mapObject = std::make_shared<MapObject>(state.unitId, type, state.x, state.y);

			SetProperties(*mapObject, state);

			const auto& config = CustomConfig.has_value() ? *CustomConfig : GetDefaultConfig(type);
			ConfigureObject(*mapObject, config);

			return mapObject;
		}

	private:
		static const MapObjectConfig& GetDefaultConfig(const std::string& type);

		static const std::string& GetTypeName(const std::string& stateName);

		static void ConfigureObject(MapObject& mapObject, const MapObjectConfig& config);

		template <typename State>
		static void SetProperties(MapObject& mapObject, const State& state)
		{
			if constexpr (has_member_hp<State>())
			{
				mapObject.m_hp = state.hp;
			}

			if constexpr (has_member_strength<State>())
			{
				mapObject.m_strength = state.strength;
			}

			if constexpr (has_member_agility<State>())
			{
				mapObject.m_agility = state.agility;
			}

			if constexpr (has_member_range<State>())
			{
				mapObject.m_range = state.range;
			}
		}

	private:
		std::optional<MapObjectConfig> CustomConfig;
	};

}
