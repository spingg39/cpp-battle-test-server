#pragma once

#include "Geometry/Point.hpp"

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace sw
{

	class MapObjectTask;
	class Map;

	class MapObject : public std::enable_shared_from_this<MapObject>
	{
		friend class MapObjectBuilder;

	public:
		MapObject(uint32_t id, std::string type, uint32_t _x, uint32_t _y);

		~MapObject();

		uint32_t GetId() const;

		const std::string& GetType() const;

		uint32_t GetX() const;

		uint32_t GetY() const;

		const Point& GetPos() const;

		void SetPos(const Point& pos);

		const std::optional<uint32_t>& GetHP() const;

		void SetHP(uint32_t _hp);

		const std::optional<uint32_t>& GetStrength() const;

		void SetStrength(uint32_t _strength);

		const std::optional<uint32_t>& GetAgility() const;

		void SetAgility(uint32_t _agility);

		const std::optional<uint32_t>& GetRange() const;

		void SetRange(uint32_t _range);

		bool IsOccupyingCell() const;

		void SetOccupyCell(bool val);

		uint32_t GetMoveSpeed() const;

		void SetMoveSpeed(uint32_t speed);

		bool Update();

		bool IsDead() const;

		bool CanReceiveDamage() const;

		bool ReceiveDamage(MapObject& attacker, uint32_t damage);

		void AddTask(std::unique_ptr<MapObjectTask> task);

		void VisitTasks(std::function<void(const MapObjectTask&)> visitor) const;

		void SetMap(Map* m);

		Map* GetMap() const;

	private:
		Map* m_Map{};
		uint32_t m_unitId;
		std::string m_Type;
		Point m_Pos;

		bool m_OccupyCell{true};
		uint32_t m_MoveSpeed{0};

		std::vector<std::unique_ptr<MapObjectTask>> m_Tasks;

		std::optional<uint32_t> m_hp;
		std::optional<uint32_t> m_strength;
		std::optional<uint32_t> m_agility;
		std::optional<uint32_t> m_range;
	};

}
