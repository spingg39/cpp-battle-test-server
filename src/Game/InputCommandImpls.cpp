#include "Game.hpp"
#include "InputCommandImpl.hpp"
#include "Map.hpp"
#include "MapObject.hpp"
#include "MapObjectBuilder.hpp"
#include "MapObjectConfig.hpp"
#include "MapObjectTasks/MarchTask.hpp"

#include <stdexcept>

namespace sw
{

	void ExecuteInputCommand(const io::CreateMap& state, Game& game)
	{
		game.CreateMap(state.width, state.height);
	}

	void ExecuteInputCommand(const io::SpawnSwordsman& state, Game& game)
	{
		if (Map* map = game.GetMap())
		{
			map->TryAddObject(MapObjectBuilder().Build(state));
		}
		else
		{
			throw std::runtime_error("Can't spawn unit. Map is not created");
		}
	}

	void ExecuteInputCommand(const io::SpawnHunter& state, Game& game)
	{
		if (Map* map = game.GetMap())
		{
			map->TryAddObject(MapObjectBuilder().Build(state));
		}
		else
		{
			throw std::runtime_error("Can't spawn unit. Map is not created");
		}
	}

	void ExecuteInputCommand(const io::March& state, Game& game)
	{
		if (Map* map = game.GetMap())
		{
			auto obj = map->GetObject(state.unitId);
			if (obj == nullptr)
			{
				throw std::runtime_error("Can't find Map Object with Id");
			}

			obj->AddTask(
				std::make_unique<MarchTask>(*obj, DefaultMarchConfig.Priority, Point{state.targetX, state.targetY}));
		}
		else
		{
			throw std::runtime_error("Can't march unit. Map is not created");
		}
	}

}
