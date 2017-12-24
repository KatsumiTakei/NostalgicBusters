/**
 *	@file	PlayerStateManager.h
 *	@date	2017 / 11 / 01
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../../stdafx.h"
#include "PlayerState.h"

class Player;

class PlayerStateManager
{
private:
	class Impl;
	std::unique_ptr<Impl> pImpl;

public:
	PlayerStateManager(Player *argPlayer);
	~PlayerStateManager();

public:

	bool Update();
	void Draw();

	void ChangeState(eState argNextState);
	eState GetCurrentState()const;
};