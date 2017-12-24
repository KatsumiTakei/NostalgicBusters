/**
 *	@file	BallBullet.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "EnemyBullet.h"

class Enemy;
class Player;

class BallBullet final : public EnemyBullet
{

public:
	BallBullet(Math::Vector2 *argPlayerPos);
	~BallBullet();

public:
	bool Update()override;
	void Draw()override;

};
