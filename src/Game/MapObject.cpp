#include "MapObject.hpp"

#include "Game.hpp"
#include "Map.hpp"
#include "MapObjectTasks/MapObjectTask.hpp"

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

	const std::optional<uint32_t>& MapObject::GetHP() const
	{
		return m_hp;
	}

	void MapObject::SetHP(uint32_t _hp)
	{
		m_hp = _hp;
	}

	const std::optional<uint32_t>& MapObject::GetStrength() const
	{
		return m_strength;
	}

	void MapObject::SetStrength(uint32_t _strength)
	{
		m_strength = _strength;
	}

	const std::optional<uint32_t>& MapObject::GetAgility() const
	{
		return m_agility;
	}

	void MapObject::SetAgility(uint32_t _agility)
	{
		m_agility = _agility;
	}

	const std::optional<uint32_t>& MapObject::GetRange() const
	{
		return m_range;
	}

	void MapObject::SetRange(uint32_t _range)
	{
		m_range = _range;
	}

	bool MapObject::IsOccupyingCell() const
	{
		return m_OccupyCell;
	}

	void MapObject::SetOccupyCell(bool val)
	{
		m_OccupyCell = val;
	}

	uint32_t MapObject::GetMoveSpeed() const
	{
		return m_MoveSpeed;
	}

	void MapObject::SetMoveSpeed(uint32_t speed)
	{
		m_MoveSpeed = speed;
	}

	bool MapObject::Update()
	{
		if (GetMap() == nullptr)
		{
			throw std::runtime_error("MapObject::Update: trying update without map: " + std::to_string(GetId()));
		}

		bool updated = false;

		for (const auto& task : m_Tasks)
		{
			if (IsDead())
			{
				break;
			}
			if (task->Update())
			{
				updated = true;
				break;
			}
		}

		std::erase_if(m_Tasks, [](const auto& task) { return task->IsComplete(); });

		return updated;
	}

	bool MapObject::IsDead() const
	{
		return m_hp.has_value() && m_hp == 0;
	}

	bool MapObject::CanReceiveDamage() const
	{
		return m_hp.has_value();
	}

	bool MapObject::ReceiveDamage(MapObject& attacker, uint32_t damage)
	{
		if (!CanReceiveDamage())
		{
			return false;
		}

		uint32_t old_hp = *GetHP();
		uint32_t new_hp = old_hp < damage ? 0 : old_hp - damage;

		SetHP(new_hp);

		m_Map->GetGame().OnEvent(io::UnitAttacked(attacker.GetId(), GetId(), damage, *GetHP()));

		if (m_hp == 0)
		{
			m_Map->GetGame().OnEvent(io::UnitDied(GetId()));
		}
		return true;
	}

	void MapObject::AddTask(std::unique_ptr<MapObjectTask> task)
	{
		m_Tasks.push_back(std::move(task));
		std::ranges::sort(m_Tasks, [](const auto& l, const auto& r) { return l->GetPriority() < r->GetPriority(); });
	}

	void MapObject::VisitTasks(std::function<void(const MapObjectTask&)> visitor) const
	{
		for (const auto& task : m_Tasks)
		{
			visitor(*task);
		}
	}

	void MapObject::SetMap(Map* m)
	{
		m_Map = m;
	}

	Map* MapObject::GetMap() const
	{
		return m_Map;
	}

}
