/**
 *	@file	LowSpdState.h
 *	@date	2017 / 11 / 01
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../../stdafx.h"
#include "PlayerState.h"

class Player;

class LowSpdState : public PlayerState
{
private:
	class Impl;
	std::unique_ptr<Impl> pImpl;

private:
	virtual void OnChangeState()override;

public:
	LowSpdState(Player *argPlayer);
	~LowSpdState();

public:

	virtual bool Update() override;
	virtual void Draw() override;

};