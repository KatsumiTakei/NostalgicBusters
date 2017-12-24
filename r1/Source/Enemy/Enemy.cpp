/**
 *	@file	Enemy.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "Enemy.h"
#include "EnemyMovePattern.h"

#include "../Player/Player.h"
#include "../Collision/CollisionManager2D.h"


Enemy::Enemy()
	: GameObject2D(eShapeType::Circle)
{
	isLiving_ = false;
}

Enemy::~Enemy() = default;

Enemy * Enemy::Create()
{
	move_ = EnemyMovePattern::MovePatterns()[data_.pattern];
	isLiving_ = true;

	return this;
}

void Enemy::CollisionEnter()
{
	Player *player = collider_->PlayerInfo;
	if (player)
		player->HitDamage();
}

