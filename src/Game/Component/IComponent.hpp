#pragma once

#include <cstdint>
#include <type_traits>

namespace sw
{

	class IComponent
	{
	public:
		virtual ~IComponent() = default;
	};

	template <typename T>
	concept IsComponent = std::is_base_of_v<IComponent, T>;

}
