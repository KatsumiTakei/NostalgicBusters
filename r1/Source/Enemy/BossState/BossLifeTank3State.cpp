/**
 *	@file	BossLifeTank3State.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "BossLifeTank3State.h"
#include "../Boss.h"
#include "../EnemyBullet/EnemyBullet.h"

BossLifeTank3State::BossLifeTank3State(Boss *argBoss)
:CBossLifeState(argBoss)
{
}

BossLifeTank3State::~BossLifeTank3State() = default;

bool BossLifeTank3State::Update()
{
	EnemyData data = boss_->Data();
	if (boss_->Count() % 100 == 0)
	{
		data.brknd = 9;
		data.blknd = eBulletColor::BulletThinRed;

		boss_->Data(data);
		//boss_->ShotBullet();
	}
	else if (boss_->Count() % 100 == 5)
	{
		data.brknd = 10;
		data.blknd = eBulletColor::BulletThinGreen;

		boss_->Data(data);
		//boss_->ShotBullet();
	}

	return true;
}