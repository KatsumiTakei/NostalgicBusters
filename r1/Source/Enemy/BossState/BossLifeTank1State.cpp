/**
 *	@file	BossLifeTank1State.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "BossLifeTank1State.h"
#include "../Boss.h"
#include "../EnemyBullet/EnemyBullet.h"
#include "../../Config.h"
#include "../../Input/Input.h"

BossLifeTank1State::BossLifeTank1State(Boss *argBoss)
	:CBossLifeState(argBoss)
{
}

BossLifeTank1State::~BossLifeTank1State() = default;

void BossLifeTank1State::Initialize(Player *argPlayer)
{
	bossPos_ = boss_->Pos();
	Math::Vector2 destPos = Math::Vector2(PlayScreenRight * 0.5f, PlayScreenBottom * 0.5f);
	tween_ = std::make_unique<Tween<Math::Vector2>>(bossPos_, destPos, static_cast<float>(boss_->Data().bltime) * 0.5f, Ease::EaseIn);
	CBossLifeState::Initialize(argPlayer);
}

bool BossLifeTank1State::Update()
{
	if (tween_->Update())
	{
		boss_->Pos(bossPos_);
		return true;
	}
	EnemyData data = boss_->Data();
	if (boss_->Count() % 10 == 0)
	{
		{// メインショット
			data.brknd = 7;
			data.blknd = static_cast<eBulletColor>(Math::XorShift() % 4 + 2);
			boss_->Data(data);
			//boss_->ShotBullet();

		}

		{// 両サイドの弾
			data.brknd = 13;
			data.blknd = eBulletColor::BulletCircleBule;
			boss_->Data(data);
			//boss_->ShotBullet();

			data.brknd++;
			boss_->Data(data);
			//boss_->ShotBullet();

		}
	}

	return true;
}
