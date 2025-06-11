#pragma once

#include "Event.hpp"
#include "InputCommand.hpp"

#include <memory>
#include <queue>
#include <random>

namespace sw
{
	struct ITask;
	class Map;
	class EventLog;

	class Game
	{
	public:
		Game();

		~Game();

		void SetEventLog(std::shared_ptr<EventLog> eventLog);

		void AddInputCommand(const InputCommand& command);

		bool IsEnded() const;

		void Update();

		void CreateMap(uint32_t w, uint32_t h);

		Map* GetMap();

		void OnEvent(Event event);

		uint32_t GetRandom(uint32_t min, uint32_t max);

	private:
		bool HasQueuedCommands() const;
		void RunQueuedCommands();

	private:
		uint64_t m_Tick = 0;
		std::mt19937 m_RandomEngine{0};
		std::shared_ptr<EventLog> m_EventLog;
		std::queue<InputCommand> m_CommandQueue;
		std::unique_ptr<Map> m_Map;
	};

}
