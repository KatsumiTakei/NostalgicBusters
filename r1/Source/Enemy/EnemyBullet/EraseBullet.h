/**
 *	@file	EraseBullet.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "EnemyBullet.h"

class Enemy;
class Player;

class EraseBullet final : public EnemyBullet
{

public:
	EraseBullet(Enemy *argEnemy, Player *argPlayer);
	EraseBullet();
	~EraseBullet();

public:
	void CollisionEnter() override;
	bool Update()override;
	void Draw()override;

};
