/**
 *	@file	Enemy.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../stdafx.h"
#include "../GameObject/GameObject2D.h"

enum class eBulletColor;
class EnemyMovePattern;

/**
 *	@struct		EnemyData
 *	@brief		敵の情報
 *	@attention	ステージ作成ツールの書き出し順に合わせて変数を宣言する
 */
struct EnemyData
{
	eBulletColor blknd;
	int brknd, pattern, count, life,
		bltime, wait;
	float posX, posY, spdX, spdY;
};

class Enemy abstract : public GameObject2D
{
	friend EnemyMovePattern;
protected:
	std::function <void(Enemy*)>	move_;
	EnemyData						data_;

public:
	Enemy();
	virtual ~Enemy();

public:
	virtual void CollisionEnter() override;
	virtual Enemy *Create();

	virtual void SetEnemy(int argPattern, int argBrknd, Math::Vector2 argPos, Math::Vector2 argSpd,
		int argBltime, eBulletColor argBlknd, int argLife, int argWait) = 0;
	virtual void SetEnemy(EnemyData argEnemyData) = 0;

public:
	virtual bool Update() override = 0;
	virtual void Draw() override = 0;
	virtual void Finalize() = 0;

#pragma region SetterAndGetter
public:
	
	inline EnemyData Data()const { return data_; }
	inline void Data(EnemyData argData) { data_ = argData; }
#pragma endregion

};