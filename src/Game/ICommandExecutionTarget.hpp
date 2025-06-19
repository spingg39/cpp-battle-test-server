#pragma once

#include "IUpdateContext.hpp"

namespace sw
{

	struct ICommandExecutionTarget : public IUpdateContext
	{
		virtual void CreateMap(uint32_t w, uint32_t h) = 0;

		virtual void AddMapObject(std::shared_ptr<MapObject> object) = 0;
	};

}
