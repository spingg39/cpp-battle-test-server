#include "Map.hpp"

#include "Utils/GeometryUtils.hpp"

#include <stdexcept>

namespace sw
{

	Map::Map(uint32_t width, uint32_t height) :
			m_Width(width),
			m_Height(height)
	{}

	Map::~Map() = default;

	uint32_t Map::GetWidth() const
	{
		return m_Width;
	}

	uint32_t Map::GetHeight() const
	{
		return m_Height;
	}

	bool Map::IsValidPos(const Point& pos) const
	{
		return pos.x < m_Width && pos.y < m_Height;
	}

	void Map::AddObject(const Point& pos, uint32_t id)
	{
		if (!IsValidPos(pos))
		{
			throw std::invalid_argument("Map::AddObject: invalid pos");
		}

		m_CoordinateToObjectMap.emplace(pos, id);
	}

	void Map::RemoveObject(const Point& pos, uint32_t id)
	{
		auto [it, itEnd] = m_CoordinateToObjectMap.equal_range(pos);
		while (it != itEnd)
		{
			if (it->second == id)
			{
				it = m_CoordinateToObjectMap.erase(it);
				break;
			}

			++it;
		}
	}

	std::vector<uint32_t> Map::GetIdsAt(const Point& pos) const
	{
		if (!IsValidPos(pos))
		{
			throw std::invalid_argument("Map::GetIdsAt: invalid pos");
		}

		std::vector<uint32_t> Ids;
		auto [itBegin, itEnd] = m_CoordinateToObjectMap.equal_range(pos);
		for (; itBegin != itEnd; ++itBegin)
		{
			Ids.push_back(itBegin->second);
		}
		return Ids;
	}

	std::vector<uint32_t> Map::GetIdsInArea(const Point& center, uint32_t minRadius, uint32_t maxRadius) const
	{
		if (minRadius > maxRadius)
		{
			throw std::invalid_argument("GetIdsInArea: minRadius > maxRadius");
		}

		std::vector<uint32_t> idsInArea;

		auto [botLeft, topRight] = GetRect(center, maxRadius);

		for (uint32_t x = botLeft.x; x <= topRight.x; ++x)
		{
			for (uint32_t y = botLeft.y; y <= topRight.y; ++y)
			{
				Point pos{x, y};
				Point dist = Dist(pos, center);
				if (dist.x >= minRadius || dist.y >= minRadius)
				{
					for (const auto& id : GetIdsAt(pos))
					{
						idsInArea.push_back(id);
					}
				}
			}
		}

		return idsInArea;
	}

	Rect Map::GetRect(const Point& center, uint32_t radius) const
	{
		uint32_t minX = center.x >= radius ? center.x - radius : 0;
		uint32_t minY = center.y >= radius ? center.y - radius : 0;

		uint32_t maxX
			= (center.x + radius < m_Width && center.x + radius >= center.x) ? center.x + radius : m_Width - 1;
		uint32_t maxY
			= (center.y + radius < m_Height && center.y + radius >= center.y) ? center.y + radius : m_Height - 1;
		return Rect(Point(minX, minY), Point(maxX, maxY));
	}

}
