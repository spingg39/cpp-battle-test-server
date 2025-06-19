#pragma once

#include "IComponent.hpp"

namespace sw
{
	class RangeComponent : public IComponent
	{
	public:
		RangeComponent(uint32_t _range) :
				range(_range)
		{}

		uint32_t GetRange() const
		{
			return range;
		}

	private:
		uint32_t range;
	};
}
