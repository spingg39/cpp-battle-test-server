#pragma once

#include "InputCommand.hpp"

#include <fstream>
#include <iostream>

namespace sw
{
	class Game;

	void ExecuteInputCommand(const io::CreateMap& state, Game& game);
	void ExecuteInputCommand(const io::SpawnSwordsman& state, Game& game);
	void ExecuteInputCommand(const io::SpawnHunter& state, Game& game);
	void ExecuteInputCommand(const io::March& state, Game& game);
}
