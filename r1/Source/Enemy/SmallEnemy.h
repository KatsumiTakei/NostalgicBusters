/**
 *	@file	SmallEnemy.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "Enemy.h"
#include "../stdafx.h"

enum class eItemId;
struct EnemyData;
class Player;

class SmallEnemy final : public Enemy
{
private:
	class Impl;
	std::unique_ptr<Impl>pImpl;

private:
	virtual void CollisionEnter();

public:
	SmallEnemy(Player *argPlayer, eItemId argItemId);
	~SmallEnemy();

	void SetEnemy(int argPattern, int argBrknd, Math::Vector2 argPos, Math::Vector2 argSpd,
		int argBltime, eBulletColor argBlknd, int argLife, int argWait)override;


	void SetEnemy(EnemyData argEnemyData) override;

public:

	bool Update() override;
	void Draw() override;
	void Finalize() override;
};