/**
 *	@file	BossLifeTank3State.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "CBossLifeState.h"

class Boss;

class BossLifeTank3State final : public CBossLifeState
{
public:
	BossLifeTank3State(Boss *argBoss);
	~BossLifeTank3State();

public:

	bool Update() override;
};