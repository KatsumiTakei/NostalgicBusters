/**
 *	@file	EnemyBullet.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../../stdafx.h"
#include "../../GameObject/GameObject2D.h"

class Enemy;
class EnemyBulletMovePattern;

enum class eBulletColor : int
{
	BulletFeatherPink,
	BulletFeatherRed,
	BulletThinRed,
	BulletThinGreen,
	BulletThinBlue,
	BulletThinYellow,
	BulletCircleBule,
	BulletCircleRed,
	BulletCircleYellow,
	BulletCirclePink,
};

class EnemyBullet abstract : public GameObject2D
{
	friend EnemyBulletMovePattern;
private:
	float accel_;
	float interval_;
	std::function <void(EnemyBullet*) > move_;

protected:

	Math::Vector2 *target_;
	Enemy *enemy_;
	eBulletColor rate_;
	Math::Vector4 color_;

public:
	EnemyBullet(Math::Vector2 *argPlayerPos, eShapeType argShapeType);
	virtual ~EnemyBullet();
	EnemyBullet *Create(Enemy* argEnemy, Math::Vector2 argGeneratePos, float argAccel, float argInterval);

public:

	virtual void CollisionEnter() override;
	virtual bool Update() override;
	virtual void Draw() override;

};