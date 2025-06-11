#include "Game.hpp"

#include "InputCommandImpl.hpp"
#include "Map.hpp"

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

		return m_Map->IsEnded();
	}

	void Game::Update()
	{
		++m_Tick;
		RunQueuedCommands();

		if (m_Map)
		{
			m_Map->Update();
		}
	}

	void Game::CreateMap(uint32_t w, uint32_t h)
	{
		if (m_Map == nullptr)
		{
			m_Map = std::make_unique<Map>(*this, w, h);
			OnEvent(io::MapCreated{.width = w, .height = h});
		}
		else
		{
			// Message?
		}
	}

	Map* Game::GetMap()
	{
		return m_Map.get();
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

}
