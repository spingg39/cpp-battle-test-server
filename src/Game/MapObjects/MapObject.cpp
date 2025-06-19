#include "MapObject.hpp"

#include <algorithm>

namespace sw
{
	MapObject::MapObject(uint32_t id, std::string type, uint32_t _x, uint32_t _y) :
			m_unitId(id),
			m_Type(std::move(type)),
			m_Pos(_x, _y)
	{}

	MapObject::~MapObject() = default;

	uint32_t MapObject::GetId() const
	{
		return m_unitId;
	}

	const std::string& MapObject::GetType() const
	{
		return m_Type;
	}

	uint32_t MapObject::GetX() const
	{
		return m_Pos.x;
	}

	uint32_t MapObject::GetY() const
	{
		return m_Pos.y;
	}

	const Point& MapObject::GetPos() const
	{
		return m_Pos;
	}

	void MapObject::SetPos(const Point& pos)
	{
		m_Pos = pos;
	}
}
