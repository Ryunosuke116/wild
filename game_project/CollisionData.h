#pragma once
#include "common.h"
#include <string>

struct CollisionResult
{
	VECTOR position_new;
	bool isHitGround;
	bool isHitWall;
	VECTOR isHitWall_normal;
	std::string objectTag;
};

struct PositionData
{
	VECTOR position_top_new;
	VECTOR position_bottom_new;
	VECTOR position_top;
	VECTOR position_bottom;
};