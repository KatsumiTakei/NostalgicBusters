/**
 *	@file	EnemyCrashEffect.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../Player/PlayerBullet/EraseRipple.h"

class Enemy;

class EnemyCrashEffect final : public EraseRipple
{

public:
	EnemyCrashEffect();
	~EnemyCrashEffect();
public:
	EnemyCrashEffect *Create(Enemy *argEnemy);

public:
	bool Update()override;
	void Draw()override;
};