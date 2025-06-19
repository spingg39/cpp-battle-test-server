#pragma once
#include <cstdint>

namespace sw
{

	class MapObject;
	struct IUpdateContext;

	bool AttackImpl(
		IUpdateContext& context, MapObject& attacker, uint32_t damage, uint32_t minRadius, uint32_t maxRadius);

}
