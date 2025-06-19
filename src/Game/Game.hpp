#pragma once

#include "Event.hpp"
#include "ICommandExecutionTarget.hpp"
#include "InputCommands/InputCommand.hpp"

#include <memory>
#include <queue>
#include <random>
#include <unordered_map>
#include <vector>

namespace sw
{
	struct ITask;
	class Map;
	class EventLog;
	class MapObject;

	class Game : public ICommandExecutionTarget
	{
	public:
		Game();

		~Game();

		void SetEventLog(std::shared_ptr<EventLog> eventLog);

		EventLog* GetEventLog();

		uint64_t GetTick() const;

		void AddInputCommand(const InputCommand& command);

		bool IsEnded() const;

		void Update();

		bool HasMapObject(uint32_t id) const;

	public:	 // ICommandExecutionTarget
		void CreateMap(uint32_t w, uint32_t h) override;

		void AddMapObject(std::shared_ptr<MapObject> object) override;

		Map* GetMap() override;

		std::shared_ptr<MapObject> GetMapObject(uint32_t id) const override;

		void OnEvent(Event event) override;

		uint32_t GetRandom(uint32_t min, uint32_t max) override;

	private:
		bool HasQueuedCommands() const;
		void RunQueuedCommands();

		void MarkObjectsToRemove();
		void RemoveMarkedObjects();

	private:
		uint64_t m_Tick = 0;
		std::mt19937 m_RandomEngine{0};
		std::shared_ptr<EventLog> m_EventLog;
		std::queue<InputCommand> m_CommandQueue;
		std::unique_ptr<Map> m_Map;
		std::vector<uint32_t> m_TurnOrderObjects;
		std::unordered_map<uint32_t, std::shared_ptr<MapObject>> m_HashIdObj;
		std::vector<uint32_t> m_MarkedToRemove;
		bool m_UpdatedLastTick{false};
	};

}
