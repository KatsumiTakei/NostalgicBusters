/**
 *	@file	Boss.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../stdafx.h"
#include "Enemy.h"

class BossState;
class Player;

class Boss : public Enemy
{
private:
	class Impl;
	std::unique_ptr<Impl> pImpl;
private:
	void CollisionEnter();

public:
	Boss(Player *argPlayer);
	~Boss();

public:
	virtual Enemy *Create();

	void SetEnemy(int argPattern, int argBrknd,	Math::Vector2 argPos, Math::Vector2 argSpd, 
		int argBltime, eBulletColor argBlknd, int argLife, int argWait)override;
	void SetEnemy(EnemyData argEnemyData)override;

public:
	bool Update()override;
	void Draw()override;
	void Finalize()override;

};