#include "Game/Component/AgilityComponent.hpp"
#include "Game/Component/HealthComponent.hpp"
#include "Game/Component/MovableComponent.hpp"
#include "Game/Component/OccupyingComponent.hpp"
#include "Game/Component/RangeComponent.hpp"
#include "Game/Component/StrengthComponent.hpp"
#include "Game/Game.hpp"  //for io
#include "Game/MapObjects/MapObject.hpp"
#include "Game/MapObjects/MapObjectBuilder.hpp"

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
	EXPECT_EQ(swordsMan->GetComponent<HealthComponent>()->GetHP(), DefaultSpawnSwordsman.hp);
	EXPECT_EQ(swordsMan->GetComponent<StrengthComponent>()->GetStrength(), DefaultSpawnSwordsman.strength);
	EXPECT_EQ(swordsMan->GetComponent<AgilityComponent>(), nullptr);
	EXPECT_EQ(swordsMan->GetComponent<RangeComponent>(), nullptr);
	EXPECT_TRUE(swordsMan->GetComponent<OccupyingComponent>() != nullptr);
	EXPECT_EQ(swordsMan->GetComponent<MovableComponent>()->GetMoveSpeed(), 1);
}

TEST(MapObjectBuilderTest, CreateHunter)
{
	auto hunter = MapObjectBuilder().Build(DefaultSpawnHunter);
	EXPECT_TRUE(hunter != nullptr);
	EXPECT_EQ(hunter->GetId(), DefaultSpawnHunter.unitId);
	EXPECT_EQ(hunter->GetType(), "Hunter");
	EXPECT_EQ(hunter->GetX(), DefaultSpawnHunter.x);
	EXPECT_EQ(hunter->GetY(), DefaultSpawnHunter.y);
	EXPECT_EQ(hunter->GetComponent<HealthComponent>()->GetHP(), DefaultSpawnHunter.hp);
	EXPECT_EQ(hunter->GetComponent<StrengthComponent>()->GetStrength(), DefaultSpawnHunter.strength);
	EXPECT_EQ(hunter->GetComponent<AgilityComponent>()->GetAgility(), DefaultSpawnHunter.agility);
	EXPECT_EQ(hunter->GetComponent<RangeComponent>()->GetRange(), DefaultSpawnHunter.range);
	EXPECT_TRUE(hunter->GetComponent<OccupyingComponent>() != nullptr);
	EXPECT_EQ(hunter->GetComponent<MovableComponent>()->GetMoveSpeed(), 1);
}

TEST(MapObjectTest, HealthComponentTest)
{
	auto swordsMan = MapObjectBuilder().Build(DefaultSpawnSwordsman);
	auto comp = swordsMan->GetComponent<HealthComponent>();
	EXPECT_TRUE(comp != nullptr);
	EXPECT_TRUE(comp->GetHP() > 0U);
	EXPECT_FALSE(comp->IsDead());
	comp->SetHP(0);
	EXPECT_TRUE(comp->IsDead());
}
