
#include "Game/Game.hpp"
#include "Game/Map.hpp"
#include "Game/MapObjectBuilder.hpp"

#include <gtest/gtest.h>

using namespace sw;

static const io::SpawnSwordsman DefaultSpawnSwordsman{.unitId = 1, .x = 0, .y = 0, .hp = 10, .strength = 5};

TEST(MapTest, CreateMap)
{
	Game game;
	uint32_t w = 10;
	uint32_t h = 5;
	game.CreateMap(w, h);
	EXPECT_TRUE(game.GetMap() != nullptr);
}

TEST(MapTest, GetMapWidthHeight)
{
	Game game;
	uint32_t w = 10;
	uint32_t h = 5;
	game.CreateMap(w, h);
	Map * map = game.GetMap();
	EXPECT_EQ(map->GetWidth(), w);
	EXPECT_EQ(map->GetHeight(), h);
}

TEST(MapTest, AddObject)
{
	Game game;
	game.CreateMap(10, 10);
	Map * map = game.GetMap();
	auto swordsMan = MapObjectBuilder().Build(DefaultSpawnSwordsman);
	EXPECT_TRUE(swordsMan != nullptr);
	EXPECT_TRUE(map->TryAddObject(swordsMan));
	EXPECT_EQ(swordsMan, map->GetObject(DefaultSpawnSwordsman.unitId));
	EXPECT_EQ(swordsMan->GetId(), DefaultSpawnSwordsman.unitId);
	EXPECT_EQ(swordsMan->GetX(), DefaultSpawnSwordsman.x);
	EXPECT_EQ(swordsMan->GetY(), DefaultSpawnSwordsman.y);
}

TEST(MapTest, AddNotValidObject)
{
	Game game;
	game.CreateMap(10, 10);
	Map * map = game.GetMap();
	EXPECT_FALSE(map->TryAddObject(nullptr));
	EXPECT_TRUE(map->TryAddObject(MapObjectBuilder().Build(DefaultSpawnSwordsman)));

	auto SwordsManWithSamePos = DefaultSpawnSwordsman;
	++SwordsManWithSamePos.unitId;
	EXPECT_FALSE(map->TryAddObject(MapObjectBuilder().Build(SwordsManWithSamePos)));

	auto SwordsManWithSameId = DefaultSpawnSwordsman;
	++SwordsManWithSameId.x;
	EXPECT_FALSE(map->TryAddObject(MapObjectBuilder().Build(SwordsManWithSameId)));
}

TEST(MapTest, GetMap)
{
	Game game;
	game.CreateMap(10, 10);
	Map * map = game.GetMap();

	auto swordsMan = MapObjectBuilder().Build(DefaultSpawnSwordsman);
	EXPECT_TRUE(swordsMan->GetMap() == nullptr);
	EXPECT_TRUE(map->TryAddObject(swordsMan));
	EXPECT_TRUE(swordsMan->GetMap() != nullptr);
	EXPECT_EQ(swordsMan->GetMap(), map);
}

TEST(MapTest, HasObject)
{
	Game game;
	game.CreateMap(10, 10);
	Map * map = game.GetMap();
	auto swordsMan = MapObjectBuilder().Build(DefaultSpawnSwordsman);
	EXPECT_FALSE(map->HasObject(*swordsMan));
	EXPECT_TRUE(map->TryAddObject(swordsMan));
	EXPECT_TRUE(map->HasObject(*swordsMan));
}

TEST(MapTest, IsOccupied)
{
	Game game;
	game.CreateMap(10, 10);
	Map * map = game.GetMap();
	auto swordsMan = MapObjectBuilder().Build(DefaultSpawnSwordsman);
	EXPECT_FALSE(map->IsOccupied({DefaultSpawnSwordsman.x, DefaultSpawnSwordsman.y}));
	EXPECT_TRUE(map->TryAddObject(swordsMan));
	EXPECT_TRUE(map->IsOccupied({DefaultSpawnSwordsman.x, DefaultSpawnSwordsman.y}));
}

TEST(MapTest, GetWithWrongId)
{
	Game game;
	game.CreateMap(10, 10);
	auto nullObj = game.GetMap()->GetObject(1);
	EXPECT_EQ(nullObj, nullptr);
}

TEST(MapTest, GetWithPos)
{
	Game game;
	game.CreateMap(10, 10);
	Map * map = game.GetMap();
	auto nullObj = map->GetObjectsAtPos({0, 0});
	EXPECT_EQ(nullObj.size(), 0);
	auto SpawnConfig = DefaultSpawnSwordsman;
	EXPECT_TRUE(map->TryAddObject(MapObjectBuilder().Build(SpawnConfig)));
	auto obj = map->GetObjectsAtPos({0, 0});
	EXPECT_EQ(obj.size(), 1);
	auto obj2 = map->GetObjectsAtPos({1, 0});
	EXPECT_EQ(obj2.size(), 0);
	auto obj3 = map->GetObjectsAtPos({0, 1});
	EXPECT_EQ(obj3.size(), 0);
}

TEST(MapTest, GetWithArea)
{
	Game game;
	game.CreateMap(10, 10);
	Map * map = game.GetMap();
	auto nullObj = map->GetObjectsInArea({0, 0}, 0, 0);
	EXPECT_EQ(nullObj.size(), 0);
	auto SpawnConfig = DefaultSpawnSwordsman;
	EXPECT_TRUE(map->TryAddObject(MapObjectBuilder().Build(SpawnConfig)));
	SpawnConfig.x = 2;
	SpawnConfig.y = 2;
	++SpawnConfig.unitId;
	EXPECT_TRUE(map->TryAddObject(MapObjectBuilder().Build(SpawnConfig)));

	auto obj = map->GetObjectsInArea({0, 0}, 0, 0);
	EXPECT_EQ(obj.size(), 1);
	EXPECT_EQ(obj[0]->GetX(), 0);
	EXPECT_EQ(obj[0]->GetY(), 0);

	auto obj2 = map->GetObjectsInArea({0, 0}, 0, 2);
	EXPECT_EQ(obj2.size(), 2);

	auto obj3 = map->GetObjectsInArea({1, 1}, 1, 1);
	EXPECT_EQ(obj3.size(), 2);
}

TEST(MapTest, MoveObject)
{
	Game game;
	game.CreateMap(10, 10);
	auto swordsMan = MapObjectBuilder().Build(DefaultSpawnSwordsman);
	EXPECT_TRUE(game.GetMap()->TryAddObject(swordsMan));
	EXPECT_EQ(swordsMan->GetX(), DefaultSpawnSwordsman.x);
	EXPECT_EQ(swordsMan->GetY(), DefaultSpawnSwordsman.y);
	EXPECT_TRUE(game.GetMap()->TryMoveObject(*swordsMan, {DefaultSpawnSwordsman.x + 2, DefaultSpawnSwordsman.y + 3}));
	EXPECT_EQ(swordsMan->GetX(), DefaultSpawnSwordsman.x + 2);
	EXPECT_EQ(swordsMan->GetY(), DefaultSpawnSwordsman.y + 3);
}

TEST(MapTest, MoveObjectNotFromMap)
{
	Game game;
	game.CreateMap(10, 10);
	auto swordsMan = MapObjectBuilder().Build(DefaultSpawnSwordsman);

	EXPECT_EQ(swordsMan->GetX(), DefaultSpawnSwordsman.x);
	EXPECT_EQ(swordsMan->GetY(), DefaultSpawnSwordsman.y);
	EXPECT_FALSE(game.GetMap()->TryMoveObject(*swordsMan, {DefaultSpawnSwordsman.x + 2, DefaultSpawnSwordsman.y + 3}));
	EXPECT_EQ(swordsMan->GetX(), DefaultSpawnSwordsman.x);
	EXPECT_EQ(swordsMan->GetY(), DefaultSpawnSwordsman.y);
}

TEST(MapTest, MoveObjectToOccupiedPos)
{
	Game game;
	game.CreateMap(10, 10);
	Map * map = game.GetMap();
	auto swordsMan = MapObjectBuilder().Build(DefaultSpawnSwordsman);
	EXPECT_TRUE(map->TryAddObject(swordsMan));
	auto SecondConfig = DefaultSpawnSwordsman;
	++SecondConfig.unitId;
	++SecondConfig.x;
	EXPECT_TRUE(map->TryAddObject(MapObjectBuilder().Build(SecondConfig)));
	EXPECT_FALSE(map->TryMoveObject(*swordsMan, {SecondConfig.x, SecondConfig.y}));
}

TEST(MapTest, MoveObjectToSamePos)
{
	Game game;
	game.CreateMap(10, 10);
	Map * map = game.GetMap();
	auto swordsMan = MapObjectBuilder().Build(DefaultSpawnSwordsman);
	EXPECT_TRUE(map->TryAddObject(swordsMan));
	EXPECT_FALSE(map->TryMoveObject(*swordsMan, {DefaultSpawnSwordsman.x, DefaultSpawnSwordsman.y}));
}

TEST(MapTest, KillObject)
{
	Game game;
	game.CreateMap(10, 10);
	auto map = game.GetMap();

	auto swordsMan = MapObjectBuilder().Build(DefaultSpawnSwordsman);
	EXPECT_TRUE(map->TryAddObject(swordsMan));
	swordsMan->SetHP(0);
	map->Update();
	map->Update();

	EXPECT_TRUE(swordsMan->GetMap() == nullptr);
}
