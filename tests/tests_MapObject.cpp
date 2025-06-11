#include "Game/Game.hpp"  //for io
#include "Game/MapObject.hpp"
#include "Game/MapObjectBuilder.hpp"
#include "Game/MapObjectTasks/MapObjectTask.hpp"
#include "Game/MapObjectTasks/MarchTask.hpp"

#include <gtest/gtest.h>

using namespace sw;

static const io::SpawnSwordsman DefaultSpawnSwordsman{.unitId = 1, .x = 0, .y = 0, .hp = 10, .strength = 5};
static const io::SpawnHunter DefaultSpawnHunter{
	.unitId = 2, .x = 5, .y = 5, .hp = 5, .agility = 5, .strength = 1, .range = 5};

TEST(MapObjectBuilderTest, CreateSwordsman)
{
	auto swordsMan = MapObjectBuilder().Build(DefaultSpawnSwordsman);
	EXPECT_TRUE(swordsMan != nullptr);
	EXPECT_EQ(swordsMan->GetId(), DefaultSpawnSwordsman.unitId);
	EXPECT_EQ(swordsMan->GetType(), "Swordsman");
	EXPECT_EQ(swordsMan->GetX(), DefaultSpawnSwordsman.x);
	EXPECT_EQ(swordsMan->GetY(), DefaultSpawnSwordsman.y);
	EXPECT_EQ(swordsMan->GetHP(), DefaultSpawnSwordsman.hp);
	EXPECT_EQ(swordsMan->GetStrength(), DefaultSpawnSwordsman.strength);
	EXPECT_FALSE(swordsMan->GetAgility().has_value());
	EXPECT_FALSE(swordsMan->GetRange().has_value());
	EXPECT_TRUE(swordsMan->IsOccupyingCell());
	EXPECT_EQ(swordsMan->GetMoveSpeed(), 1);
}

TEST(MapObjectBuilderTest, CreateHunter)
{
	auto hunter = MapObjectBuilder().Build(DefaultSpawnHunter);
	EXPECT_TRUE(hunter != nullptr);
	EXPECT_EQ(hunter->GetId(), DefaultSpawnHunter.unitId);
	EXPECT_EQ(hunter->GetType(), "Hunter");
	EXPECT_EQ(hunter->GetX(), DefaultSpawnHunter.x);
	EXPECT_EQ(hunter->GetY(), DefaultSpawnHunter.y);
	EXPECT_EQ(hunter->GetHP(), DefaultSpawnHunter.hp);
	EXPECT_EQ(hunter->GetStrength(), DefaultSpawnHunter.strength);
	EXPECT_EQ(hunter->GetAgility(), DefaultSpawnHunter.agility);
	EXPECT_EQ(hunter->GetRange(), DefaultSpawnHunter.range);
	EXPECT_TRUE(hunter->IsOccupyingCell());
	EXPECT_EQ(hunter->GetMoveSpeed(), 1);
}

TEST(MapObjectBuilderTest, InvalidCustomConfig_RangeAttackForSwordsman)
{
	MapObjectConfig withRangeAttack{.RangeAttack = TaskConfig{.Priority = 1}};
	EXPECT_THROW(MapObjectBuilder().WithCustomConfig(withRangeAttack).Build(DefaultSpawnSwordsman), std::runtime_error);
}

TEST(MapObjectTest, IsDead)
{
	auto swordsMan = MapObjectBuilder().Build(DefaultSpawnSwordsman);
	EXPECT_TRUE(swordsMan->CanReceiveDamage());
	EXPECT_TRUE(swordsMan->GetHP() > 0U);
	EXPECT_FALSE(swordsMan->IsDead());
	swordsMan->SetHP(0);
	EXPECT_TRUE(swordsMan->IsDead());
}

TEST(MapObjectTest, NotAddedObjectGetMap)
{
	auto swordsMan = MapObjectBuilder().Build(DefaultSpawnSwordsman);
	EXPECT_TRUE(swordsMan->GetMap() == nullptr);
}

TEST(MapObjectTest, TaskOrder)
{
	auto hunter = MapObjectBuilder().Build(DefaultSpawnHunter);
	uint32_t count = 0;
	uint32_t prevPriority = 0;
	hunter->VisitTasks(
		[&](const auto& task)
		{
			++count;
			EXPECT_TRUE(task.GetPriority() >= prevPriority);
			prevPriority = task.GetPriority();
		});
	EXPECT_EQ(count, 2);
}

class TestTask : public MapObjectTask
{
	using MapObjectTask::MapObjectTask;

	bool Update() override
	{
		return false;
	}

	bool IsComplete() const override
	{
		return false;
	}
};

TEST(MapObjectTest, AddTaskOrder_AddLowerPriority)
{
	auto hunter = MapObjectBuilder().Build(DefaultSpawnHunter);
	hunter->AddTask(std::make_unique<TestTask>(*hunter, 0));
	uint32_t count = 0;
	uint32_t prevPriority = 0;
	hunter->VisitTasks(
		[&](const auto& task)
		{
			++count;
			EXPECT_TRUE(task.GetPriority() >= prevPriority);
			prevPriority = task.GetPriority();
		});
	EXPECT_EQ(count, 3);
}

TEST(MapObjectTest, AddTaskOrder_AddHigherPriority)
{
	auto hunter = MapObjectBuilder().Build(DefaultSpawnHunter);
	hunter->AddTask(std::make_unique<TestTask>(*hunter, 10));
	uint32_t count = 0;
	uint32_t prevPriority = 0;
	hunter->VisitTasks(
		[&](const auto& task)
		{
			++count;
			EXPECT_TRUE(task.GetPriority() >= prevPriority);
			prevPriority = task.GetPriority();
		});
	EXPECT_EQ(count, 3);
}

TEST(MapObjectTest, TryMarchNonMovable)
{
	auto nonmovableConfig = MapObjectConfigs.at("Swordsman");
	nonmovableConfig.MoveSpeed = 0;
	auto nonMovableUnit = MapObjectBuilder().WithCustomConfig(nonmovableConfig).Build(DefaultSpawnSwordsman);
	EXPECT_THROW(
		nonMovableUnit->AddTask(std::make_unique<MarchTask>(*nonMovableUnit, 3, Point(1, 1))), std::runtime_error);
}

TEST(MapObjectTest, TryUpdateWithoutMap)
{
	auto swordsMan = MapObjectBuilder().Build(DefaultSpawnSwordsman);
	EXPECT_THROW(swordsMan->Update(), std::runtime_error);
}
