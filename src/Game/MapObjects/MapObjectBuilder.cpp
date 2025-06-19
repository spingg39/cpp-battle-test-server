#include "MapObjectBuilder.hpp"

#include "Types/Hunter.hpp"
#include "Types/Swordsman.hpp"

namespace sw
{
	std::shared_ptr<MapObject> MapObjectBuilder::Create(const io::SpawnSwordsman& state)
	{
		return std::make_shared<Swordsman>(state);
	}

	std::shared_ptr<MapObject> MapObjectBuilder::Create(const io::SpawnHunter& state)
	{
		return std::make_shared<Hunter>(state);
	}
}
