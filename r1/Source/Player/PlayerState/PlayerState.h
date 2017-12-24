/**
 *	@file	PlayerState.h
 *	@date	2017 / 11 / 01
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../../stdafx.h"

class Player;

enum class eState
{
	Default,
	Low,

	Max,
};

class PlayerState
{
protected:

	Player *player_;

public:
	PlayerState(Player *argPlayer);
	virtual ~PlayerState();

public:

	virtual bool Update() = 0;
	virtual void Draw() = 0;

	virtual void OnChangeState() = 0;
};