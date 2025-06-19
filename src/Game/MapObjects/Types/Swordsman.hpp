#pragma once

#include <Game/MapObjects/MapObject.hpp>

namespace sw
{
	namespace io
	{
		struct SpawnSwordsman;
	}

	class Swordsman : public MapObject
	{
	public:
		Swordsman(const io::SpawnSwordsman&);

		virtual bool Update(IUpdateContext& context) override;
	};

}
