/**
 *	@file	EnemyBullet.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "EnemyBullet.h"

#include "../../Player/Player.h"
#include "../../Player/PlayerBullet/ChargeBullet.h"
#include "../Enemy.h"
#include "EnemyBulletMovePattern.h"
#include "../../Collision/CollisionManager2D.h"
#include "../../Config.h"

#include "../../Player/Ai/Obstacle.h"

EnemyBullet::EnemyBullet(Math::Vector2 *argPlayerPos, eShapeType argShapeType)
	: GameObject2D(argShapeType), target_(argPlayerPos), rate_(eBulletColor::BulletCircleBule),
	color_(Math::Vector4(1.f, 1.f, 1.f, 1.f)), accel_(0.f), interval_(0.f)
{
	graph_ = Singleton<GraphManager>::Get()->FindIndexTexture(L"Bullets");
	isLiving_ = false;
}
EnemyBullet::~EnemyBullet() = default;

EnemyBullet *EnemyBullet::Create(Enemy* argEnemy, Math::Vector2 argGeneratePos, float argAccel, float argInterval)
{
	enemy_ = argEnemy;
	isLiving_ = true;
	pos_ = argGeneratePos;
	rate_ = enemy_->Data().blknd;
	move_ = EnemyBulletMovePattern::MovePatterns()[enemy_->Data().brknd];
	accel_ = argAccel;
	interval_ = argInterval;
	count_ = 0;

	return this;
}

void EnemyBullet::CollisionEnter()
{
	Player *player = collider_->PlayerInfo;
	bool shouldErase = collider_->EraseRippleInfo;
	if (player || shouldErase)
	{
		if (player->HitDamage() || shouldErase)
			isLiving_ = false;
	}
	collider_->Clear();
}

bool EnemyBullet::Update()
{
	Singleton<ObstacleManager>::Get()->Entry(this, IsDelay());

	move_(this);
	pos_.x += std::cosf(angle_) * spd_.x;
	pos_.y += std::sinf(angle_) * spd_.y;
	count_++;

	isLiving_ = IsInArea(pos_, PlayArea);

	Singleton<CollisionManager2D>::Get()->Entry(this);

	return isLiving_;
}

void EnemyBullet::Draw()
{
	GameObject2D::Draw();
}
