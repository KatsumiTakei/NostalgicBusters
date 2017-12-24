/**
 *	@file	EnemyBulletMovePattern.cpp
 *	@date	2017 /11 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "EnemyBulletMovePattern.h"
#include "EnemyBullet.h"
#include "../Enemy.h"

#include <LuaBind.h>

std::vector<std::function<void(EnemyBullet*)>> EnemyBulletMovePattern::movePatterns_;
std::once_flag	EnemyBulletMovePattern::isInit_;

void EnemyBulletMovePattern::Create()
{
	auto lua = Singleton<LuaBindLib::LuaBind>::Get()->LuaState();

	lua->new_usertype<EnemyBullet>(
		"EnemyBullet",
		"count", &EnemyBullet::count_,
		"angle", &EnemyBullet::angle_,
		"pos", &EnemyBullet::pos_,
		"spd", &EnemyBullet::spd_,
		"enemy", &EnemyBullet::enemy_,
		"interval", &EnemyBullet::interval_,
		"accel", &EnemyBullet::accel_,

		"Target", [](EnemyBullet *argBullet) ->Math::Vector2 { return (*argBullet->target_); }
	);

	Singleton<LuaBindLib::LuaBind>::Get()->LoadScript("ShotMove.lua");

	const int PatternMax = (*lua)["ShotPatternMax"].get<int>();

	(*lua)["PlayScreenLeft"] = PlayScreenLeft;
	(*lua)["PlayScreenRight"] = PlayScreenRight;

	movePatterns_.resize(PatternMax);
	for (int lIndex = 0; lIndex < PatternMax; lIndex++)
	{
		std::stringstream ss;
		ss << "ShotPattern" << lIndex;

		std::string arg = ss.str();
		movePatterns_[lIndex] = [=](EnemyBullet* argEnemy) { (*lua)[arg.c_str()](argEnemy); };
	}

}

EnemyBulletMovePattern::~EnemyBulletMovePattern() = default;

std::vector<std::function<void(EnemyBullet*)>> EnemyBulletMovePattern::MovePatterns()
{
	std::call_once(isInit_, Create);

	return movePatterns_;
}


