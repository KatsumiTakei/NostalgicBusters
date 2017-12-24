/**
 *	@file	EnemyBulletManager.h
 *	@date	2017 / 12 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../../stdafx.h"

class EnemyBullet;
class Player;
class Enemy;

class EnemyBulletManager
{
private:
	class Impl;
	std::unique_ptr<Impl> pImpl;
	bool canShoot_;
public:
	EnemyBulletManager();
	~EnemyBulletManager();

public:

	void SetBullets(Player *argPlayer);

	EnemyBullet *Create(Enemy* argEnemy, Math::Vector2 argGeneratePos, float argAccel, float argInterval);
	void CreateAndRegist(Enemy* argEnemy, Math::Vector2 argGeneratePos, float argAccel, float argInterval);

	inline void CanShoot(bool argCanShoot) { canShoot_ = argCanShoot; }
	inline bool CanShoot() const { return canShoot_; }
	int BulletMax()const;
	EnemyBullet** Bullets(bool argIsDelay)const;
	EnemyBullet** Bullets(int argGameSide)const;
	void Clear(bool argIsDelay);
	void Clear(int argGameSide);
	void Clear();
};