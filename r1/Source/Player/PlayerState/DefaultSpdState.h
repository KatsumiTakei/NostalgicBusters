/**
 *	@file	DefaultSpdState.h
 *	@date	2017 / 11 / 01
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../../stdafx.h"
#include "PlayerState.h"

class Player;

class DefaultSpdState : public PlayerState
{
private:
	class Impl;
	std::unique_ptr<Impl> pImpl;

private:
	virtual void OnChangeState()override;

public:
	DefaultSpdState(Player *argPlayer);
	~DefaultSpdState();

public:

	virtual bool Update() override;
	virtual void Draw() override;
};