#include "MapObjectTask.hpp"

namespace sw
{

	MapObjectTask::MapObjectTask(MapObject& _parent, uint32_t _priority) :
			m_parent(_parent),
			m_priority(_priority)
	{}

	MapObjectTask::~MapObjectTask() = default;

	MapObject& MapObjectTask::GetMapObject() const
	{
		return m_parent;
	}

	uint32_t MapObjectTask::GetPriority() const
	{
		return m_priority;
	}
}
