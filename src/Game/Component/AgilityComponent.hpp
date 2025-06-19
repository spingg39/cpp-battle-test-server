#pragma once

#include "IComponent.hpp"

namespace sw
{
	class AgilityComponent : public IComponent
	{
	public:
		AgilityComponent(uint32_t _agility) :
				agility(_agility)
		{}

		uint32_t GetAgility() const
		{
			return agility;
		}

	private:
		uint32_t agility;
	};
}
