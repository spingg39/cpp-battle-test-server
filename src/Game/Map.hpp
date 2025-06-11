#pragma once

#include "Geometry/Point.hpp"
#include "Geometry/Rect.hpp"

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

namespace sw
{

	class MapObject;
	class Game;

	using MapObjectPredicate = std::function<bool(const MapObject&)>;

	inline static constexpr auto MapObjectTruePredicate = [](const MapObject&)
	{
		return true;
	};

	class Map
	{
	public:
		Map(Game& parent, uint32_t width, uint32_t height);

		~Map();

		uint32_t GetWidth() const;

		uint32_t GetHeight() const;

		bool IsEnded() const;

		void Update();

		bool HasObject(const MapObject& obj) const;

		bool TryAddObject(std::shared_ptr<MapObject> object);

		bool TryMoveObject(MapObject& obj, const Point& pos);

		bool IsOccupied(const Point& pos) const;

		std::vector<std::shared_ptr<MapObject>> GetObjectsAtPos(const Point& pos) const;

		std::vector<std::shared_ptr<MapObject>> GetObjectsInArea(
			const Point& center,
			uint32_t minRadius,
			uint32_t maxRadius,
			MapObjectPredicate Predicate = MapObjectTruePredicate) const;

		std::shared_ptr<MapObject> GetObject(uint32_t id) const;

		Game& GetGame();

	private:
		void MarkObjectToRemove(uint32_t id);
		void RemoveMarkedObjects();

		void RemoveFromCoordMap(MapObject* obj);
		void RemoveFromHashMap(MapObject* obj);

		void AddToCoordMap(MapObject* obj);
		void AddToHashMap(MapObject* obj);

		Rect GetRect(const Point& center, uint32_t radius) const;

	private:
		Game& m_Game;
		bool m_UpdatedLastTick{false};
		uint32_t m_Width{};
		uint32_t m_Height{};
		std::vector<std::shared_ptr<MapObject>> m_Objects;
		std::multimap<Point, MapObject*> m_CoordinateToObjectMap;
		std::unordered_map<uint32_t, MapObject*> m_HashIdObj;
		std::vector<uint32_t> m_MarkedToRemove;
	};

}
