#pragma once

#include "InputCommand.hpp"

#include <fstream>
#include <iostream>

namespace sw
{
	struct ICommandExecutionTarget;

	void ExecuteInputCommand(const io::CreateMap& state, ICommandExecutionTarget& target);
	void ExecuteInputCommand(const io::SpawnSwordsman& state, ICommandExecutionTarget& target);
	void ExecuteInputCommand(const io::SpawnHunter& state, ICommandExecutionTarget& target);
	void ExecuteInputCommand(const io::March& state, ICommandExecutionTarget& target);
}
