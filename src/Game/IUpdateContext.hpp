#pragma once

#include <Game/Event.hpp>
#include <cstdint>
#include <random>

namespace sw
{

	class Map;
	class MapObject;

	struct IUpdateContext
	{
		virtual ~IUpdateContext() = default;

		virtual Map* GetMap() = 0;

		virtual void OnEvent(Event event) = 0;

		virtual uint32_t GetRandom(uint32_t min, uint32_t max) = 0;

		virtual std::shared_ptr<MapObject> GetMapObject(uint32_t id) const = 0;
	};

}
