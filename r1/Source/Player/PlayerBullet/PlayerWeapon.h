/**
 *	@file	PlayerWeapon.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../../GameObject/GameObject2D.h"

class Player;

namespace
{
	const int BulletMoveNomal = 0;
	const int BulletMoveHoming = 1;
	const float BulletSpeed = -18.f;
	const int MaxBullet = 400;
}

class PlayerWeapon : public GameObject2D
{
private:
	float HomingShot(Player *argPlayer);

public:

	PlayerWeapon(eShapeType argShapeType);
	virtual ~PlayerWeapon();

	PlayerWeapon *Create(Player *argPlayer, Math::Vector2 argPos, int argMode);
public:
	virtual void CollisionEnter() override = 0;

	virtual bool Update() override;
	virtual void Draw() override = 0;
};