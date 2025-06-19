#pragma once

#include <Game/MapObjects/MapObject.hpp>

namespace sw
{
	namespace io
	{
		struct SpawnHunter;
	}

	class Hunter : public MapObject
	{
	public:
		Hunter(const io::SpawnHunter&);

		virtual bool Update(IUpdateContext& context) override;
	};
}
