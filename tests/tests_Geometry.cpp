#include "Game/Utils/GeometryUtils.hpp"

#include <gtest/gtest.h>
#include <numeric>

using namespace sw;

TEST(GeometryTest, Dist)
{
	EXPECT_EQ(Dist(5, 12), 7);
}

TEST(GeometryTest, Dist_Negative)
{
	EXPECT_EQ(Dist(21, 13), 8);
}

TEST(GeometryTest, Dist_Point)
{
	Point a(0, 0);
	Point b(2, 3);
	Point dist = Dist(a, b);
	EXPECT_EQ(dist.x, 2);
	EXPECT_EQ(dist.y, 3);
}

TEST(GeometryTest, Dist_Point_Negative)
{
	Point a(0, 0);
	Point b(2, 3);
	Point dist = Dist(b, a);
	EXPECT_EQ(dist.x, 2);
	EXPECT_EQ(dist.y, 3);
}

TEST(GeometryTest, ShiftTo_1)
{
	Point start(0, 0);
	Point target(2, 2);
	uint32_t max_delta = 1;
	Point shifted = ShiftTo(start, target, max_delta);
	EXPECT_EQ(shifted.x, 1);
	EXPECT_EQ(shifted.y, 1);
}

TEST(GeometryTest, ShiftTo_DeltaLargerThanTarget)
{
	Point start(0, 0);
	Point target(2, 2);
	uint32_t max_delta = 3;
	Point shifted = ShiftTo(start, target, max_delta);
	EXPECT_EQ(shifted.x, 2);
	EXPECT_EQ(shifted.y, 2);
}

TEST(GeometryTest, ShiftTo_DeltaLargerThanTarget_MaxDelta)
{
	Point start(0, 0);
	Point target(2, 2);
	uint32_t max_delta = std::numeric_limits<uint32_t>::max();
	Point shifted = ShiftTo(start, target, max_delta);
	EXPECT_EQ(shifted.x, 2);
	EXPECT_EQ(shifted.y, 2);
}

TEST(GeometryTest, ShiftTo_OneDirection_x)
{
	Point start(0, 0);
	Point target(2, 0);
	uint32_t max_delta = 1;
	Point shifted = ShiftTo(start, target, max_delta);
	EXPECT_EQ(shifted.x, 1);
	EXPECT_EQ(shifted.y, 0);
}

TEST(GeometryTest, ShiftTo_OneDirection_y)
{
	Point start(0, 0);
	Point target(0, 2);
	uint32_t max_delta = 1;
	Point shifted = ShiftTo(start, target, max_delta);
	EXPECT_EQ(shifted.x, 0);
	EXPECT_EQ(shifted.y, 1);
}

TEST(GeometryTest, ShiftTo_ZeroDelta)
{
	Point start(0, 0);
	Point target(2, 2);
	uint32_t max_delta = 0;
	Point shifted = ShiftTo(start, target, max_delta);
	EXPECT_EQ(shifted.x, 0);
	EXPECT_EQ(shifted.y, 0);
}

TEST(GeometryTest, ShiftTo_StartEqualsTarget)
{
	Point start(2, 2);
	Point target(2, 2);
	uint32_t max_delta = 1;
	Point shifted = ShiftTo(start, target, max_delta);
	EXPECT_EQ(shifted.x, 2);
	EXPECT_EQ(shifted.y, 2);
}

TEST(GeometryTest, ShiftTo_Negative)
{
	Point start(10, 10);
	Point target(0, 0);
	uint32_t max_delta = 1;
	Point shifted = ShiftTo(start, target, max_delta);
	EXPECT_EQ(shifted.x, 9);
	EXPECT_EQ(shifted.y, 9);
}

TEST(GeometryTest, ShiftTo_Negative_OneDirection_x)
{
	Point start(10, 10);
	Point target(0, 10);
	uint32_t max_delta = 1;
	Point shifted = ShiftTo(start, target, max_delta);
	EXPECT_EQ(shifted.x, 9);
	EXPECT_EQ(shifted.y, 10);
}

TEST(GeometryTest, ShiftTo_Negative_OneDirection_y)
{
	Point start(10, 10);
	Point target(10, 0);
	uint32_t max_delta = 1;
	Point shifted = ShiftTo(start, target, max_delta);
	EXPECT_EQ(shifted.x, 10);
	EXPECT_EQ(shifted.y, 9);
}

TEST(GeometryTest, ShiftTo_Negative_DeltaLargerThanTarget)
{
	Point start(10, 10);
	Point target(5, 5);
	uint32_t max_delta = 100;
	Point shifted = ShiftTo(start, target, max_delta);
	EXPECT_EQ(shifted.x, 5);
	EXPECT_EQ(shifted.y, 5);
}

TEST(GeometryTest, ShiftTo_Negative_DeltaLargerThanTarget_MaxDelta)
{
	Point start(10, 10);
	Point target(5, 5);
	uint32_t max_delta = std::numeric_limits<uint32_t>::max();
	Point shifted = ShiftTo(start, target, max_delta);
	EXPECT_EQ(shifted.x, 5);
	EXPECT_EQ(shifted.y, 5);
}
