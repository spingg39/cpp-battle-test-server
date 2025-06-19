
#include "ExecuteInputCommand.hpp"

#include <Game/Component/MovableComponent.hpp>
#include <Game/Game.hpp>
#include <Game/ICommandExecutionTarget.hpp>
#include <Game/MapObjects/MapObject.hpp>
#include <Game/MapObjects/MapObjectBuilder.hpp>
#include <stdexcept>

namespace sw
{

	void ExecuteInputCommand(const io::CreateMap& state, ICommandExecutionTarget& target)
	{
		target.CreateMap(state.width, state.height);
	}

	void ExecuteInputCommand(const io::SpawnSwordsman& state, ICommandExecutionTarget& target)
	{
		target.AddMapObject(MapObjectBuilder().Build(state));
	}

	void ExecuteInputCommand(const io::SpawnHunter& state, ICommandExecutionTarget& target)
	{
		target.AddMapObject(MapObjectBuilder().Build(state));
	}

	void ExecuteInputCommand(const io::March& state, ICommandExecutionTarget& target)
	{
		if (target.GetMap() == nullptr)
		{
			throw std::runtime_error("Can't march unit. Map is not created");
		}

		auto obj = target.GetMapObject(state.unitId);
		if (obj == nullptr)
		{
			throw std::runtime_error("Can't find Map Object with Id");
		}

		auto movable = obj->GetComponent<MovableComponent>();
		if (movable == nullptr)
		{
			throw std::runtime_error("Can't move object without MovableComponent");
		}

		movable->SetTarget(Point{state.targetX, state.targetY});
		target.OnEvent(io::MarchStarted(obj->GetId(), obj->GetX(), obj->GetY(), state.targetX, state.targetY));
	}

}
