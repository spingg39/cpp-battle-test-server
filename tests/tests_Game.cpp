
#include "Game/Component/AgilityComponent.hpp"
#include "Game/Component/HealthComponent.hpp"
#include "Game/Component/RangeComponent.hpp"
#include "Game/Component/StrengthComponent.hpp"
#include "Game/Game.hpp"
#include "Game/Map.hpp"
#include "Game/MapObjects/MapObject.hpp"
#include "Game/MapObjects/MapObjectBuilder.hpp"

#include <IO/System/EventLog.hpp>
#include <gtest/gtest.h>

using namespace sw;

TEST(GameTest, InputCommand_CreateMap)
{
	Game game;
	game.AddInputCommand(io::CreateMap(10, 100));
	EXPECT_TRUE(game.GetMap() == nullptr);
	game.Update();
	EXPECT_EQ(game.GetMap()->GetWidth(), 10);
	EXPECT_EQ(game.GetMap()->GetHeight(), 100);
}

TEST(GameTest, InputCommand_SpawnUnit_Swordsman)
{
	Game game;
	game.AddInputCommand(io::CreateMap(10, 100));
	uint32_t x = 0;
	uint32_t y = 2;
	game.AddInputCommand(io::SpawnSwordsman(1, x, y, 5, 2));
	game.Update();
	auto obj = game.GetMapObject(1);
	EXPECT_EQ(obj->GetId(), 1);
	EXPECT_EQ(obj->GetX(), x);
	EXPECT_EQ(obj->GetY(), y);
	EXPECT_EQ(obj->GetComponent<HealthComponent>()->GetHP(), 5);
	EXPECT_EQ(obj->GetComponent<StrengthComponent>()->GetStrength(), 2);
}

TEST(GameTest, InputCommand_SpawnUnit_Hunter)
{
	Game game;
	game.AddInputCommand(io::CreateMap(10, 100));
	uint32_t x = 9;
	uint32_t y = 0;
	game.AddInputCommand(io::SpawnHunter(2, x, y, 10, 4, 1, 4));
	game.Update();
	auto obj = game.GetMapObject(2);
	EXPECT_EQ(obj->GetId(), 2);
	EXPECT_EQ(obj->GetX(), x);
	EXPECT_EQ(obj->GetY(), y);
	EXPECT_EQ(obj->GetComponent<HealthComponent>()->GetHP(), 10);
	EXPECT_EQ(obj->GetComponent<AgilityComponent>()->GetAgility(), 4);
	EXPECT_EQ(obj->GetComponent<StrengthComponent>()->GetStrength(), 1);
	EXPECT_EQ(obj->GetComponent<RangeComponent>()->GetRange(), 4);
}

TEST(GameTest, InputCommand_SpawnUnit_BeforeMapCreated)
{
	Game game;
	game.AddInputCommand(io::SpawnSwordsman(1, 0, 2, 5, 2));
	EXPECT_THROW(game.Update(), std::runtime_error);
}

TEST(GameTest, InputCommand_March)
{
	Game game;
	game.AddInputCommand(io::CreateMap(10, 100));
	uint32_t x = 0;
	uint32_t y = 2;
	game.AddInputCommand(io::SpawnSwordsman(1, x, y, 5, 2));
	game.AddInputCommand(io::March(1, 9, 0));
	game.Update();
	auto obj = game.GetMapObject(1);
	EXPECT_EQ(obj->GetId(), 1);
	EXPECT_EQ(obj->GetX(), x + 1);
	EXPECT_EQ(obj->GetY(), y - 1);
}

TEST(GameTest, InputCommand_March_ForAbsentUnit)
{
	Game game;
	game.AddInputCommand(io::CreateMap(10, 100));
	game.AddInputCommand(io::March(1, 9, 0));
	EXPECT_THROW(game.Update(), std::runtime_error);
}

TEST(GameTest, InputCommand_March_BeforeMapCreated_1)
{
	Game game;
	game.AddInputCommand(io::March(1, 9, 0));
	EXPECT_THROW(game.Update(), std::runtime_error);
}

TEST(GameTest, InputCommand_March_BeforeMapCreated_2)
{
	Game game;
	uint32_t x = 0;
	uint32_t y = 2;
	game.AddInputCommand(io::SpawnSwordsman(1, x, y, 5, 2));
	game.AddInputCommand(io::March(1, 9, 0));
	EXPECT_THROW(game.Update(), std::runtime_error);
}

TEST(GameTest, TestScenario_1)
{
	std::stringstream log;
	auto eventLog = std::make_shared<EventLog>(log);

	std::string EXPECTED
		= "[1] MAP_CREATED width=10 height=10 \n"
		  "[1] UNIT_SPAWNED unitId=1 unitType=Swordsman x=0 y=0 \n"
		  "[1] UNIT_SPAWNED unitId=2 unitType=Hunter x=9 y=0 \n"
		  "[1] MARCH_STARTED unitId=1 x=0 y=0 targetX=9 targetY=0 \n"
		  "[1] MARCH_STARTED unitId=2 x=9 y=0 targetX=0 targetY=0 \n"
		  "[1] UNIT_SPAWNED unitId=3 unitType=Swordsman x=0 y=9 \n"
		  "[1] MARCH_STARTED unitId=3 x=0 y=9 targetX=0 targetY=0 \n"
		  "[1] UNIT_MOVED unitId=1 x=1 y=0 \n"
		  "[1] UNIT_MOVED unitId=2 x=8 y=0 \n"
		  "[1] UNIT_MOVED unitId=3 x=0 y=8 \n"

		  "[2] UNIT_MOVED unitId=1 x=2 y=0 \n"
		  "[2] UNIT_MOVED unitId=2 x=7 y=0 \n"
		  "[2] UNIT_MOVED unitId=3 x=0 y=7 \n"

		  "[3] UNIT_MOVED unitId=1 x=3 y=0 \n"
		  "[3] UNIT_ATTACKED attackerUnitId=2 targetUnitId=1 damage=5 targetHp=0 \n"
		  "[3] UNIT_DIED unitId=1 \n"
		  "[3] UNIT_MOVED unitId=3 x=0 y=6 \n"

		  "[4] UNIT_MOVED unitId=2 x=6 y=0 \n"
		  "[4] UNIT_MOVED unitId=3 x=0 y=5 \n"

		  "[5] UNIT_MOVED unitId=2 x=5 y=0 \n"
		  "[5] UNIT_MOVED unitId=3 x=0 y=4 \n"

		  "[6] UNIT_MOVED unitId=2 x=4 y=0 \n"
		  "[6] UNIT_MOVED unitId=3 x=0 y=3 \n"

		  "[7] UNIT_ATTACKED attackerUnitId=2 targetUnitId=3 damage=5 targetHp=5 \n"
		  "[7] UNIT_MOVED unitId=3 x=0 y=2 \n"

		  "[8] UNIT_ATTACKED attackerUnitId=2 targetUnitId=3 damage=5 targetHp=0 \n"
		  "[8] UNIT_DIED unitId=3 \n";

	Game game;
	game.SetEventLog(eventLog);

	game.AddInputCommand(io::CreateMap(10, 10));

	game.AddInputCommand(io::SpawnSwordsman(1, 0, 0, 5, 2));
	game.AddInputCommand(io::SpawnHunter(2, 9, 0, 10, 5, 1, 4));

	game.AddInputCommand(io::March(1, 9, 0));
	game.AddInputCommand(io::March(2, 0, 0));

	game.AddInputCommand(io::SpawnSwordsman(3, 0, 9, 10, 2));
	game.AddInputCommand(io::March(3, 0, 0));

	while (!game.IsEnded())
	{
		game.Update();
	}
	auto res = log.str();
	EXPECT_EQ(EXPECTED, res);
}

TEST(GameTest, TestScenario_2)
{
	std::stringstream log;
	auto eventLog = std::make_shared<EventLog>(log);

	std::string EXPECTED
		= "[1] MAP_CREATED width=2 height=2 \n"
		  "[1] UNIT_SPAWNED unitId=1 unitType=Swordsman x=0 y=0 \n"
		  "[1] UNIT_SPAWNED unitId=2 unitType=Swordsman x=1 y=1 \n"
		  "[1] UNIT_SPAWNED unitId=3 unitType=Swordsman x=0 y=1 \n"
		  "[1] UNIT_SPAWNED unitId=4 unitType=Swordsman x=1 y=0 \n"
		  "[1] UNIT_ATTACKED attackerUnitId=1 targetUnitId=4 damage=5 targetHp=0 \n"
		  "[1] UNIT_DIED unitId=4 \n"
		  "[1] UNIT_ATTACKED attackerUnitId=2 targetUnitId=3 damage=5 targetHp=0 \n"
		  "[1] UNIT_DIED unitId=3 \n"

		  "[2] UNIT_ATTACKED attackerUnitId=1 targetUnitId=2 damage=5 targetHp=0 \n"
		  "[2] UNIT_DIED unitId=2 \n";

	Game game;
	game.SetEventLog(eventLog);

	game.AddInputCommand(io::CreateMap(2, 2));

	game.AddInputCommand(io::SpawnSwordsman(1, 0, 0, 5, 5));
	game.AddInputCommand(io::SpawnSwordsman(2, 1, 1, 5, 5));
	game.AddInputCommand(io::SpawnSwordsman(3, 0, 1, 5, 5));
	game.AddInputCommand(io::SpawnSwordsman(4, 1, 0, 5, 5));

	while (!game.IsEnded())
	{
		game.Update();
	}

	EXPECT_EQ(EXPECTED, log.str());
}

TEST(GameTest, TestScenario_3)
{
	std::stringstream log;
	auto eventLog = std::make_shared<EventLog>(log);

	std::string EXPECTED
		= "[1] MAP_CREATED width=10 height=10 \n"
		  "[1] UNIT_SPAWNED unitId=1 unitType=Swordsman x=0 y=0 \n"
		  "[1] UNIT_SPAWNED unitId=2 unitType=Swordsman x=9 y=9 \n"
		  "[1] MARCH_STARTED unitId=1 x=0 y=0 targetX=2 targetY=0 \n"
		  "[1] MARCH_STARTED unitId=2 x=9 y=9 targetX=6 targetY=9 \n"
		  "[1] UNIT_MOVED unitId=1 x=1 y=0 \n"
		  "[1] UNIT_MOVED unitId=2 x=8 y=9 \n"

		  "[2] UNIT_MOVED unitId=1 x=2 y=0 \n"
		  "[2] MARCH_ENDED unitId=1 x=2 y=0 \n"
		  "[2] UNIT_MOVED unitId=2 x=7 y=9 \n"

		  "[3] UNIT_MOVED unitId=2 x=6 y=9 \n"
		  "[3] MARCH_ENDED unitId=2 x=6 y=9 \n";

	Game game;
	game.SetEventLog(eventLog);

	game.AddInputCommand(io::CreateMap(10, 10));

	game.AddInputCommand(io::SpawnSwordsman(1, 0, 0, 5, 5));
	game.AddInputCommand(io::SpawnSwordsman(2, 9, 9, 5, 5));
	game.AddInputCommand(io::March(1, 2, 0));
	game.AddInputCommand(io::March(2, 6, 9));

	while (!game.IsEnded())
	{
		game.Update();
	}

	EXPECT_EQ(EXPECTED, log.str());
}

TEST(GameTest, TestScenario_4)
{
	std::stringstream log;
	auto eventLog = std::make_shared<EventLog>(log);

	std::string EXPECTED
		= "[1] MAP_CREATED width=10 height=10 \n"
		  "[1] UNIT_SPAWNED unitId=1 unitType=Swordsman x=0 y=0 \n"
		  "[1] UNIT_SPAWNED unitId=2 unitType=Hunter x=4 y=0 \n"
		  "[1] UNIT_ATTACKED attackerUnitId=2 targetUnitId=1 damage=5 targetHp=0 \n"
		  "[1] UNIT_DIED unitId=1 \n";

	Game game;
	game.SetEventLog(eventLog);

	game.AddInputCommand(io::CreateMap(10, 10));

	game.AddInputCommand(io::SpawnSwordsman(1, 0, 0, 5, 5));
	game.AddInputCommand(io::SpawnHunter(2, 4, 0, 10, 5, 1, 4));

	while (!game.IsEnded())
	{
		game.Update();
	}

	EXPECT_EQ(EXPECTED, log.str());
}

TEST(GameTest, TestScenario_5)
{
	std::stringstream log;
	auto eventLog = std::make_shared<EventLog>(log);

	std::string EXPECTED
		= "[1] MAP_CREATED width=10 height=10 \n"
		  "[1] UNIT_SPAWNED unitId=1 unitType=Swordsman x=0 y=0 \n"
		  "[1] UNIT_SPAWNED unitId=2 unitType=Hunter x=4 y=0 \n"
		  "[1] UNIT_ATTACKED attackerUnitId=2 targetUnitId=1 damage=3 targetHp=2 \n"

		  "[2] UNIT_ATTACKED attackerUnitId=2 targetUnitId=1 damage=3 targetHp=0 \n"
		  "[2] UNIT_DIED unitId=1 \n";

	Game game;
	game.SetEventLog(eventLog);

	game.AddInputCommand(io::CreateMap(10, 10));

	game.AddInputCommand(io::SpawnSwordsman(1, 0, 0, 5, 5));
	game.AddInputCommand(io::SpawnHunter(2, 4, 0, 10, 3, 1, 4));

	while (!game.IsEnded())
	{
		game.Update();
	}

	EXPECT_EQ(EXPECTED, log.str());
}

TEST(GameTest, TestScenario_6)
{
	std::stringstream log;
	auto eventLog = std::make_shared<EventLog>(log);

	std::string EXPECTED
		= "[1] MAP_CREATED width=10 height=10 \n"
		  "[1] UNIT_SPAWNED unitId=1 unitType=Hunter x=0 y=0 \n"
		  "[1] UNIT_SPAWNED unitId=2 unitType=Hunter x=1 y=0 \n"
		  "[1] UNIT_ATTACKED attackerUnitId=1 targetUnitId=2 damage=6 targetHp=0 \n"
		  "[1] UNIT_DIED unitId=2 \n";

	Game game;
	game.SetEventLog(eventLog);

	game.AddInputCommand(io::CreateMap(10, 10));

	game.AddInputCommand(io::SpawnHunter(1, 0, 0, 6, 3, 6, 4));
	game.AddInputCommand(io::SpawnHunter(2, 1, 0, 6, 3, 5, 4));

	while (!game.IsEnded())
	{
		game.Update();
	}

	EXPECT_EQ(EXPECTED, log.str());
}

TEST(GameTest, TestScenario_7)
{
	std::stringstream log;
	auto eventLog = std::make_shared<EventLog>(log);

	std::string EXPECTED
		= "[1] MAP_CREATED width=10 height=10 \n"
		  "[1] UNIT_SPAWNED unitId=1 unitType=Swordsman x=0 y=0 \n"
		  "[1] UNIT_SPAWNED unitId=2 unitType=Swordsman x=0 y=2 \n"
		  "[1] UNIT_SPAWNED unitId=3 unitType=Hunter x=4 y=0 \n"
		  "[1] UNIT_ATTACKED attackerUnitId=3 targetUnitId=2 damage=5 targetHp=0 \n"
		  "[1] UNIT_DIED unitId=2 \n"

		  "[2] UNIT_ATTACKED attackerUnitId=3 targetUnitId=1 damage=5 targetHp=0 \n"
		  "[2] UNIT_DIED unitId=1 \n";

	Game game;
	game.SetEventLog(eventLog);

	game.AddInputCommand(io::CreateMap(10, 10));

	game.AddInputCommand(io::SpawnSwordsman(1, 0, 0, 5, 5));
	game.AddInputCommand(io::SpawnSwordsman(2, 0, 2, 5, 5));
	game.AddInputCommand(io::SpawnHunter(3, 4, 0, 6, 5, 1, 4));

	while (!game.IsEnded())
	{
		game.Update();
	}

	EXPECT_EQ(EXPECTED, log.str());
}
