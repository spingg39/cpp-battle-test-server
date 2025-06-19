#pragma once

#include <IO/Commands/CreateMap.hpp>
#include <IO/Commands/March.hpp>
#include <IO/Commands/SpawnHunter.hpp>
#include <IO/Commands/SpawnSwordsman.hpp>
#include <variant>

namespace sw
{
	using InputCommand = std::variant<io::CreateMap, io::SpawnSwordsman, io::SpawnHunter, io::March>;
}
