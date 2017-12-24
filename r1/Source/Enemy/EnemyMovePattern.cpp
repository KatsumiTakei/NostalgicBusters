/**
 *	@file	EnemyMovePattern.cpp
 *	@date	2017 /11 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "EnemyMovePattern.h"
#include "Enemy.h"
#include "EnemyBullet/EnemyBullet.h"
#include "EnemyBullet/EnemyBulletManager.h"


#include <LuaBind.h>


std::vector<std::function<void(Enemy*)>> EnemyMovePattern::movePatterns_;
std::once_flag	EnemyMovePattern::isInit_;

void EnemyMovePattern::Create()
{
	auto lua = Singleton<LuaBindLib::LuaBind>::Get()->LuaState();

	lua->new_usertype<EnemyData>(
		"Data",
		"wait", &EnemyData::wait
		);

	lua->new_usertype<Enemy>(
		"Enemy",
		"count", &Enemy::count_,
		"angle", &Enemy::angle_,
		"spd", &Enemy::spd_,
		"data", &Enemy::data_,

#pragma region ShotBullet
		"ShotBullet", sol::overload([](Enemy* argEnemy) {
		
		Singleton<EnemyBulletManager>::Get()->CreateAndRegist(argEnemy, argEnemy->pos_, 0, 0);
	},
			[](Enemy* argEnemy, float argInterval, float argAccel) {

		 Singleton<EnemyBulletManager>::Get()->CreateAndRegist(argEnemy, argEnemy->pos_, argInterval, argInterval);
	},
		[](Enemy* argEnemy, float argInterval) {

		Singleton<EnemyBulletManager>::Get()->CreateAndRegist(argEnemy, argEnemy->pos_, 0, argInterval);
	},
		[](Enemy* argEnemy, Math::Vector2 argGeneratPos) {

		Singleton<EnemyBulletManager>::Get()->CreateAndRegist(argEnemy, argGeneratPos, 0, 0);
	}
		),
#pragma endregion ShotBullet

		"Pos", [](Enemy* argEnemy) ->Math::Vector2 { return argEnemy->Pos(); },
		"Count", [](Enemy* argEnemy) ->int { return argEnemy->Count(); }

	);

	Singleton<LuaBindLib::LuaBind>::Get()->LoadScript("EnemyMove.lua");

	const int PatternMax = (*lua)["MovePatternMax"].get<int>();

	movePatterns_.resize(PatternMax);
	for (int lIndex = 0; lIndex < PatternMax; lIndex++)
	{
		std::stringstream ss;
		ss << "MovePattern" << lIndex;

		std::string arg = ss.str();
		movePatterns_[lIndex] = [=](Enemy* argEnemy) { (*lua)[arg.c_str()](argEnemy); };
	}

}

EnemyMovePattern::~EnemyMovePattern() = default;

std::vector<std::function<void(Enemy*)>> EnemyMovePattern::MovePatterns()
{
	std::call_once(isInit_, Create);

	return movePatterns_;
}


