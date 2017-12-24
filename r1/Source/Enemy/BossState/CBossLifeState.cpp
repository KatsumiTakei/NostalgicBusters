/**
 *	@file	CBossLifeState.cpp
 *	@date	2017 / 7 / 27
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "CBossLifeState.h"
#include "../Boss.h"
#include "../../Player/Player.h"

CBossLifeState::CBossLifeState(Boss *argBoss)
	:boss_(argBoss)
{
}

CBossLifeState::~CBossLifeState() = default;

void CBossLifeState::Initialize(Player *argPlayer)
{
	boss_->Count(0);
	boss_->Life(boss_->MaxLife());
	argPlayer->IsBonusGet(true);
}