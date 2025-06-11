#pragma once
#include <IO/Events/MapCreated.hpp>
#include <IO/Events/MarchEnded.hpp>
#include <IO/Events/MarchStarted.hpp>
#include <IO/Events/UnitAttacked.hpp>
#include <IO/Events/UnitDied.hpp>
#include <IO/Events/UnitMoved.hpp>
#include <IO/Events/UnitSpawned.hpp>
#include <variant>

namespace sw
{
	using Event = std::variant<
		io::MapCreated,
		io::MarchEnded,
		io::MarchStarted,
		io::UnitAttacked,
		io::UnitDied,
		io::UnitMoved,
		io::UnitSpawned>;

}
