#pragma once

#include "IComponent.hpp"

namespace sw
{
	class MapObject;
	struct IUpdateContext;

	class HealthComponent : public IComponent
	{
	public:
		HealthComponent(uint32_t _hp);

		uint32_t GetHP() const;

		void SetHP(uint32_t _hp);

		bool IsDead() const;

		bool ReceiveDamage(MapObject& self, IUpdateContext& context, MapObject& attacker, uint32_t damage);

	private:
		uint32_t hp;
	};
}
