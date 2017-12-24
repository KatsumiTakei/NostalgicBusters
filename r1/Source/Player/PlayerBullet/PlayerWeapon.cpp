/**
 *	@file	PlayerWeapon.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "PlayerWeapon.h"

#include "../../Enemy/Enemy.h"
#include "../../Enemy/EnemyFactory/EnemyManager.h"
#include "../../Config.h"

PlayerWeapon::PlayerWeapon(eShapeType argShapeType)
	:GameObject2D(argShapeType)
{
	priority_ = static_cast<ePriority>(static_cast<int>(ePriority::GameObjectBack) - 1);
	isLiving_ = false;
}

PlayerWeapon::~PlayerWeapon() = default;

PlayerWeapon *PlayerWeapon::Create(Player *argPlayer, Math::Vector2 argPos, int argMode)
{
	isLiving_ = true;
	pos_ = argPos;

	spd_ = Math::Vector2(0.f, BulletSpeed);
	angle_ = 1.f;

	if (argMode == BulletMoveHoming)
	{
		angle_ = HomingShot(argPlayer);
	}
	return this;
}

float PlayerWeapon::HomingShot(Player *argPlayer)
{
	float destinationLeft = 1000;
	float angleLeft = 1.f;
	const auto &enemyList = Singleton<EnemyManager>::Get()->EnemyList(argPlayer);
	for (const auto &enemy : enemyList)
	{
		if (!enemy->IsLiving()) 
			continue;

		Math::Vector2 enemyPos = enemy->Pos();

		float destinationRight = sqrt((enemyPos.x - pos_.x) * (enemyPos.x - pos_.x) + (enemyPos.y - pos_.y) * (enemyPos.y - pos_.y));
		float angleRight = atan2(enemyPos.y - pos_.y, enemyPos.x - pos_.x);

		if (destinationLeft >= destinationRight)
		{
			destinationLeft = destinationRight;
			angleLeft = angleRight;
			spd_ = Math::Vector2(18.f, 18.f);
		}
	}
	return angleLeft;
}

bool PlayerWeapon::Update()
{
	pos_.x += cos(angle_) * spd_.x;
	pos_.y += sin(angle_) * spd_.y;

	if (!IsInArea(pos_, PlayArea))
	{
		isLiving_ = false;
	}

	return isLiving_;
}
