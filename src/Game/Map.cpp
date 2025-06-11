#include "Map.hpp"

#include "Game.hpp"
#include "Geometry/Utils.hpp"
#include "MapObject.hpp"

#include <algorithm>

namespace sw
{

	Map::Map(Game& parent, uint32_t width, uint32_t height) :
			m_Game(parent),
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

	bool Map::IsEnded() const
	{
		uint32_t aliveObjects = m_Objects.size() - m_MarkedToRemove.size();
		if (aliveObjects < 2)
		{
			return true;
		}

		return !m_UpdatedLastTick;
	}

	void Map::Update()
	{
		RemoveMarkedObjects();

		bool updated = false;

		for (const auto& obj : m_Objects)
		{
			if (obj->Update())
			{
				updated = true;
			}
		}

		for (const auto& obj : m_Objects)
		{
			if (obj->IsDead())
			{
				MarkObjectToRemove(obj->GetId());
			}
		}

		m_UpdatedLastTick = updated;
	}

	bool Map::HasObject(const MapObject& obj) const
	{
		return m_HashIdObj.contains(obj.GetId());
	}

	bool Map::TryAddObject(std::shared_ptr<MapObject> object)
	{
		if (object == nullptr || HasObject(*object))
		{
			return false;
		}

		if (!object->IsOccupyingCell() || !IsOccupied(object->GetPos()))
		{
			auto& spawnedUnit = m_Objects.emplace_back(std::move(object));
			AddToCoordMap(spawnedUnit.get());
			AddToHashMap(spawnedUnit.get());

			spawnedUnit->SetMap(this);

			m_Game.OnEvent(io::UnitSpawned{
				.unitId = spawnedUnit->GetId(),
				.unitType = spawnedUnit->GetType(),
				.x = spawnedUnit->GetX(),
				.y = spawnedUnit->GetY()});

			return true;
		}

		return false;
	}

	bool Map::TryMoveObject(MapObject& obj, const Point& pos)
	{
		if (!HasObject(obj) || IsOccupied(pos) || obj.GetPos() == pos)
		{
			return false;
		}

		RemoveFromCoordMap(&obj);
		obj.SetPos(pos);
		AddToCoordMap(&obj);

		m_Game.OnEvent(io::UnitMoved(obj.GetId(), obj.GetX(), obj.GetY()));
		return true;
	}

	bool Map::IsOccupied(const Point& pos) const
	{
		return std::ranges::any_of(GetObjectsAtPos(pos), [](const auto& obj) { return obj->IsOccupyingCell(); });
	}

	std::vector<std::shared_ptr<MapObject>> Map::GetObjectsAtPos(const Point& pos) const
	{
		std::vector<std::shared_ptr<MapObject>> Objects;
		auto [itBegin, itEnd] = m_CoordinateToObjectMap.equal_range(pos);
		for (; itBegin != itEnd; ++itBegin)
		{
			Objects.push_back(itBegin->second->shared_from_this());
		}
		return Objects;
	}

	std::vector<std::shared_ptr<MapObject>> Map::GetObjectsInArea(
		const Point& center, uint32_t minRadius, uint32_t maxRadius, MapObjectPredicate Predicate) const
	{
		if (minRadius > maxRadius)
		{
			throw std::invalid_argument("GetObjectsInArea: minRadius > maxRadius");
		}

		std::vector<std::shared_ptr<MapObject>> objectsInArea;

		auto [botLeft, topRight] = GetRect(center, maxRadius);

		for (uint32_t x = botLeft.x; x <= topRight.x; ++x)
		{
			for (uint32_t y = botLeft.y; y <= topRight.y; ++y)
			{
				Point pos{x, y};
				Point dist = Dist(pos, center);
				if (dist.x >= minRadius || dist.y >= minRadius)
				{
					for (const auto& obj : GetObjectsAtPos(pos))
					{
						if (Predicate(*obj))
						{
							objectsInArea.push_back(obj);
						}
					}
				}
			}
		}

		return objectsInArea;
	}

	std::shared_ptr<MapObject> Map::GetObject(uint32_t id) const
	{
		if (auto it = m_HashIdObj.find(id); it != m_HashIdObj.end())
		{
			return it->second->shared_from_this();
		}
		return nullptr;
	}

	Game& Map::GetGame()
	{
		return m_Game;
	}

	void Map::MarkObjectToRemove(uint32_t id)
	{
		m_MarkedToRemove.push_back(id);
	}

	void Map::RemoveMarkedObjects()
	{
		if (m_MarkedToRemove.empty())
		{
			return;
		}

		for (uint32_t Id : m_MarkedToRemove)
		{
			auto Obj = GetObject(Id);
			RemoveFromHashMap(Obj.get());
			RemoveFromCoordMap(Obj.get());
			Obj->SetMap(nullptr);
		}

		auto IsMarked = [this](const auto& obj)
		{
			return std::ranges::find(m_MarkedToRemove, obj->GetId()) != m_MarkedToRemove.end();
		};
		std::erase_if(m_Objects, IsMarked);
		m_MarkedToRemove.clear();
	}

	void Map::RemoveFromCoordMap(MapObject* obj)
	{
		auto [it, itEnd] = m_CoordinateToObjectMap.equal_range(obj->GetPos());
		while (it != itEnd)
		{
			if (it->second == obj)
			{
				it = m_CoordinateToObjectMap.erase(it);
				break;
			}

			++it;
		}
	}

	void Map::RemoveFromHashMap(MapObject* obj)
	{
		m_HashIdObj.erase(obj->GetId());
	}

	void Map::AddToCoordMap(MapObject* obj)
	{
		m_CoordinateToObjectMap.emplace(obj->GetPos(), obj);
	}

	void Map::AddToHashMap(MapObject* obj)
	{
		m_HashIdObj[obj->GetId()] = obj;
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
