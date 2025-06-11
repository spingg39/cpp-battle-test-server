#pragma once

#include <cstdint>

namespace sw
{

	class MapObject;

	class MapObjectTask
	{
	public:
		MapObjectTask(MapObject& _parent, uint32_t _priority);

		virtual ~MapObjectTask() = 0;

		virtual bool Update() = 0;

		virtual bool IsComplete() const = 0;

		MapObject& GetMapObject() const;

		uint32_t GetPriority() const;

	private:
		MapObject& m_parent;
		uint32_t m_priority{0};
	};

}
