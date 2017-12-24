/**
 *	@file	BossLifeTank2State.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../../stdafx.h"
#include "CBossLifeState.h"

class Boss;
class Player;

class BossLifeTank2State final : public CBossLifeState
{
private:

	std::unique_ptr<Tween<Math::Vector2, Math::Vector2>> tween_;

private:
	void Move();

public:

	BossLifeTank2State(Boss *argBoss);
	~BossLifeTank2State();

public:

	void Initialize(Player *argPlayer) override;
	bool Update() override;
};