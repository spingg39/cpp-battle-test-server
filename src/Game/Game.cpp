#include "Game.hpp"

#include "Component/HealthComponent.hpp"
#include "Component/OccupyingComponent.hpp"
#include "InputCommands/ExecuteInputCommand.hpp"
#include "Map.hpp"
#include "MapObjects/MapObject.hpp"
#include "Utils/OccupyUtils.hpp"

#include <IO/Events/MapCreated.hpp>
#include <IO/System/EventLog.hpp>
#include <queue>

namespace sw
{

	Game::Game() = default;

	Game::~Game() = default;

	void Game::SetEventLog(std::shared_ptr<EventLog> eventLog)
	{
		m_EventLog = std::move(eventLog);
	}

	EventLog* Game::GetEventLog()
	{
		return m_EventLog.get();
	}

	uint64_t Game::GetTick() const
	{
		return m_Tick;
	}

	void Game::AddInputCommand(const InputCommand& command)
	{
		m_CommandQueue.push(command);
	}

	bool Game::IsEnded() const
	{
		if (HasQueuedCommands())
		{
			return false;
		}

		if (m_Map == nullptr)
		{
			return true;
		}

		uint32_t aliveObjects = m_TurnOrderObjects.size() - m_MarkedToRemove.size();
		if (aliveObjects < 2)
		{
			return true;
		}

		return !m_UpdatedLastTick;
	}

	void Game::Update()
	{
		++m_Tick;
		RunQueuedCommands();

		RemoveMarkedObjects();

		bool updated = false;

		for (const auto& id : m_TurnOrderObjects)
		{
			auto obj = GetMapObject(id);
			if (obj && obj->Update(*this))
			{
				updated = true;
			}
		}

		MarkObjectsToRemove();

		m_UpdatedLastTick = updated;
	}

	bool Game::HasMapObject(uint32_t id) const
	{
		return m_HashIdObj.contains(id);
	}

	void Game::CreateMap(uint32_t w, uint32_t h)
	{
		if (m_Map == nullptr)
		{
			m_Map = std::make_unique<Map>(w, h);
			OnEvent(io::MapCreated{.width = w, .height = h});
		}
		else
		{
			throw std::runtime_error("Can't Create second map");
		}
	}

	void Game::AddMapObject(std::shared_ptr<MapObject> object)
	{
		if (object == nullptr || HasMapObject(object->GetId()))
		{
			throw std::runtime_error("Can't spawn unit");
		}

		if (m_Map)
		{
			if (!m_Map->IsValidPos(object->GetPos()))
			{
				throw std::runtime_error(
					"Can't spawn unit. Invalid coordinate, id: " + std::to_string(object->GetId()));
			}

			if (object->GetComponent<OccupyingComponent>() && IsOccupied(*this, object->GetPos()))
			{
				throw std::runtime_error("Can't spawn unit. Place is occupied");
			}

			m_Map->AddObject(object->GetPos(), object->GetId());
			m_TurnOrderObjects.push_back(object->GetId());
			m_HashIdObj[object->GetId()] = object;

			OnEvent(io::UnitSpawned{
				.unitId = object->GetId(), .unitType = object->GetType(), .x = object->GetX(), .y = object->GetY()});
		}
		else
		{
			throw std::runtime_error("Can't spawn unit. Map is not created");
		}
	}

	Map* Game::GetMap()
	{
		return m_Map.get();
	}

	std::shared_ptr<MapObject> Game::GetMapObject(uint32_t id) const
	{
		if (auto it = m_HashIdObj.find(id); it != m_HashIdObj.end())
		{
			return it->second;
		}
		return nullptr;
	}

	void Game::OnEvent(Event event)
	{
		if (m_EventLog == nullptr)
		{
			return;
		}

		std::visit([this](auto& visitedEvent) { m_EventLog->log(m_Tick, visitedEvent); }, event);
	}

	uint32_t Game::GetRandom(uint32_t min, uint32_t max)
	{
		if (max < min)
		{
			throw std::invalid_argument("Game::GetRandom: max < min");
		}

		std::uniform_int_distribution<uint32_t> dist(min, max);
		return dist(m_RandomEngine);
	}

	bool Game::HasQueuedCommands() const
	{
		return !m_CommandQueue.empty();
	}

	void Game::RunQueuedCommands()
	{
		while (!m_CommandQueue.empty())
		{
			const auto& command = m_CommandQueue.front();
			std::visit([this](const auto& commandVariant) { ExecuteInputCommand(commandVariant, *this); }, command);
			m_CommandQueue.pop();
		}
	}

	void Game::MarkObjectsToRemove()
	{
		for (const auto& id : m_TurnOrderObjects)
		{
			auto hpComp = GetMapObject(id)->GetComponent<HealthComponent>();
			if (hpComp && hpComp->IsDead())
			{
				m_MarkedToRemove.push_back(id);
			}
		}
	}

	void Game::RemoveMarkedObjects()
	{
		if (m_MarkedToRemove.empty())
		{
			return;
		}

		for (uint32_t Id : m_MarkedToRemove)
		{
			auto Obj = GetMapObject(Id);
			m_HashIdObj.erase(Id);
			if (m_Map)
			{
				m_Map->RemoveObject(Obj->GetPos(), Obj->GetId());
			}
		}

		auto IsMarked = [this](const auto& id)
		{
			return std::ranges::find(m_MarkedToRemove, id) != m_MarkedToRemove.end();
		};
		std::erase_if(m_TurnOrderObjects, IsMarked);
		m_MarkedToRemove.clear();
	}

}
