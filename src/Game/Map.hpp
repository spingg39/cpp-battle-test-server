#pragma once

#include "Utils/Point.hpp"
#include "Utils/Rect.hpp"

#include <map>
#include <vector>

namespace sw
{

	class Map
	{
	public:
		Map(uint32_t width, uint32_t height);

		~Map();

		uint32_t GetWidth() const;

		uint32_t GetHeight() const;

		bool IsValidPos(const Point& pos) const;

		void AddObject(const Point& pos, uint32_t id);

		void RemoveObject(const Point& pos, uint32_t id);

		std::vector<uint32_t> GetIdsAt(const Point& pos) const;

		std::vector<uint32_t> GetIdsInArea(const Point& center, uint32_t minRadius, uint32_t maxRadius) const;

	private:
		Rect GetRect(const Point& center, uint32_t radius) const;

	private:
		uint32_t m_Width{};
		uint32_t m_Height{};
		std::multimap<Point, uint32_t> m_CoordinateToObjectMap;
	};

}
