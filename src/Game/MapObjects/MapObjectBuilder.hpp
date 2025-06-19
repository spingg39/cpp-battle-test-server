#pragma once
#include <memory>

namespace sw
{
	class MapObject;

	namespace io
	{
		struct SpawnSwordsman;
		struct SpawnHunter;
	}

	class MapObjectBuilder
	{
	public:
		template <typename State>
		std::shared_ptr<MapObject> Build(const State& state)
		{
			return Create(state);
		}

	private:
		std::shared_ptr<MapObject> Create(const io::SpawnSwordsman& state);
		std::shared_ptr<MapObject> Create(const io::SpawnHunter& state);
	};
}
