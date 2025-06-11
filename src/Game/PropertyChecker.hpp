#pragma once
#include <type_traits>

namespace sw
{

#define DEFINE_HAS_MEMBER(Name)                                                                                        \
	template <typename T, typename = std::void_t<>>                                                                    \
	struct has_member_##Name : std::false_type                                                                         \
	{};                                                                                                                \
	template <typename T>                                                                                              \
	struct has_member_##Name<T, std::void_t<decltype(T::Name)>> : std::true_type                                       \
	{};

	DEFINE_HAS_MEMBER(hp);
	DEFINE_HAS_MEMBER(strength);
	DEFINE_HAS_MEMBER(agility);
	DEFINE_HAS_MEMBER(range);

}
