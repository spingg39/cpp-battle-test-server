#pragma once
#include <cstdint>

namespace sw
{
	
	class MapObject;

	bool AttackImpl(MapObject& attacker, uint32_t damage, uint32_t minRadius, uint32_t maxRadius);
	
}
