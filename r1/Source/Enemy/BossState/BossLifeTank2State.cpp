/**
 *	@file	BossLifeTank2State.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "BossLifeTank2State.h"
#include "../Boss.h"
#include "../EnemyBullet/EnemyBullet.h"
#include "../../Config.h"

BossLifeTank2State::BossLifeTank2State(Boss *argBoss)
:CBossLifeState(argBoss)
{
}

BossLifeTank2State::~BossLifeTank2State() = default;

void BossLifeTank2State::Initialize(Player *argPlayer)
{
	CBossLifeState::Initialize(argPlayer);
}

void BossLifeTank2State::Move()
{
	static bool isActive = false;
	static int localeCount = 0, moveCnt = 0;
	const int MaxTarget = 3;
	const int MaxBullet = 8;
	Math::Vector2 pos = boss_->Pos();	//	!<	参照で扱うためConstをつけない

	EnemyData data = boss_->Data();
	if (data.bltime + MaxBullet * 5.f <= moveCnt)
	{
		const Math::Vector2 TargetPos[MaxTarget] = {
			Math::Vector2(PlayScreenLeft * 4.f, PlayScreenBottom * 0.5f),
			Math::Vector2(PlayScreenRight - PlayScreenLeft * 2.f, PlayScreenBottom * 0.5f),
			Math::Vector2(PlayScreenRight * 0.5f, PlayScreenBottom * 0.25f) };

		Math::Vector2 nextPos = Math::Vector2();
		for (int i = 0; i < MaxTarget; i++)
		{
			if (localeCount == i)
			{
				nextPos = TargetPos[i];
				break;
			}
		}

		if (!isActive)
		{
			tween_ = std::make_unique<Tween<Math::Vector2>>(pos, nextPos, static_cast<float>(FlameTime), Ease::EaseIn);
			isActive = true;
		}

		moveCnt -= MaxBullet * 5 + 100;
	}

	if (isActive)
	{
		isActive = tween_->Update();
		boss_->Pos(pos);
		if (!isActive)
		{
			localeCount++;
			if (localeCount >= MaxTarget)
			{
				localeCount = 0;
			}
		}
	}
	moveCnt++;
}

bool BossLifeTank2State::Update()
{
	EnemyData data = boss_->Data();
	if (boss_->Count() % 100 == 0 ||
		boss_->Count() % 120 == 0 ||
		boss_->Count() % 140 == 0)
	{
		data.brknd = 11;
		data.blknd = eBulletColor::BulletCircleRed;
		boss_->Data(data);
		//boss_->ShotBullet();
	}
	
	if (boss_->Count() % 10 == 0)
	{
		data.brknd = 12;
		data.blknd = eBulletColor::BulletThinBlue;
		boss_->Data(data);
		//boss_->ShotBullet();

	}

	Move();
	return true;
}
