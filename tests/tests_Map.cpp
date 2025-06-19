
#include "Game/InputCommands/InputCommand.hpp"
#include "Game/Map.hpp"
#include "Game/MapObjects/MapObject.hpp"
#include "Game/MapObjects/MapObjectBuilder.hpp"

#include <gtest/gtest.h>
#include <set>

using namespace sw;

static const io::SpawnSwordsman DefaultSpawnSwordsman{.unitId = 1, .x = 0, .y = 0, .hp = 10, .strength = 5};

TEST(MapTest, GetMapWidthHeight)
{
	uint32_t w = 10;
	uint32_t h = 5;
	Map map(w, h);
	EXPECT_EQ(map.GetWidth(), w);
	EXPECT_EQ(map.GetHeight(), h);
}

TEST(MapTest, IsValidPos)
{
	uint32_t w = 10;
	uint32_t h = 5;
	Map map(w, h);
	EXPECT_TRUE(map.IsValidPos({0, 0}));
	EXPECT_TRUE(map.IsValidPos({9, 4}));
	EXPECT_FALSE(map.IsValidPos({10, 5}));
	EXPECT_FALSE(map.IsValidPos({100, 50}));
}

TEST(MapTest, AddMapObject)
{
	Map map(10, 10);
	auto swordsMan = MapObjectBuilder().Build(DefaultSpawnSwordsman);
	EXPECT_TRUE(swordsMan != nullptr);
	map.AddObject(swordsMan->GetPos(), swordsMan->GetId());
	auto idsAtPos = map.GetIdsAt({.x = DefaultSpawnSwordsman.x, .y = DefaultSpawnSwordsman.y});
	EXPECT_EQ(idsAtPos.size(), 1);
	EXPECT_EQ(DefaultSpawnSwordsman.unitId, idsAtPos[0]);
}

TEST(MapTest, AddNotValidPos)
{
	Map map(10, 10);
	auto SwordsManWithInvalidPos = DefaultSpawnSwordsman;
	SwordsManWithInvalidPos.x = 100;
	auto swordsMan = MapObjectBuilder().Build(SwordsManWithInvalidPos);
	EXPECT_THROW(map.AddObject(swordsMan->GetPos(), swordsMan->GetId()), std::invalid_argument);
}

TEST(MapTest, GetWithPos)
{
	Map map(10, 10);
	auto nullIds = map.GetIdsAt({0, 0});
	EXPECT_EQ(nullIds.size(), 0);
	auto SpawnConfig = DefaultSpawnSwordsman;
	auto unit = MapObjectBuilder().Build(SpawnConfig);
	map.AddObject(unit->GetPos(), unit->GetId());
	auto ids = map.GetIdsAt({0, 0});
	EXPECT_EQ(ids.size(), 1);
	EXPECT_EQ(ids[0], unit->GetId());
	auto ids2 = map.GetIdsAt({1, 0});
	EXPECT_EQ(ids2.size(), 0);
	auto ids3 = map.GetIdsAt({0, 1});
	EXPECT_EQ(ids3.size(), 0);
}

TEST(MapTest, GetWithArea)
{
	Map map(10, 10);
	auto nullIds = map.GetIdsInArea({0, 0}, 0, 0);
	EXPECT_EQ(nullIds.size(), 0);
	auto SpawnConfig = DefaultSpawnSwordsman;
	auto unit = MapObjectBuilder().Build(SpawnConfig);
	map.AddObject(unit->GetPos(), unit->GetId());
	SpawnConfig.x = 2;
	SpawnConfig.y = 2;
	++SpawnConfig.unitId;
	auto unit2 = MapObjectBuilder().Build(SpawnConfig);
	map.AddObject(unit2->GetPos(), unit2->GetId());

	auto ids1 = map.GetIdsInArea({0, 0}, 0, 0);
	EXPECT_EQ(ids1.size(), 1);
	EXPECT_EQ(ids1[0], 1);

	auto ids2 = map.GetIdsInArea({0, 0}, 0, 2);
	std::set<uint32_t> set2(ids2.begin(), ids2.end());
	std::set<uint32_t> exp2{1, 2};
	EXPECT_EQ(set2, exp2);

	auto ids3 = map.GetIdsInArea({1, 1}, 1, 1);
	std::set<uint32_t> set3(ids3.begin(), ids3.end());
	std::set<uint32_t> exp3{1, 2};
	EXPECT_EQ(set3, exp3);
}
