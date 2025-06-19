#pragma once

#include "IComponent.hpp"

namespace sw
{
	class StrengthComponent : public IComponent
	{
	public:
		StrengthComponent(uint32_t _str) :
				strength(_str)
		{}

		uint32_t GetStrength() const
		{
			return strength;
		}

	private:
		uint32_t strength;
	};
}
