/**
 *	@file	CBossLifeState.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

class Boss;
class Player;

class CBossLifeState abstract
{
protected:
	Boss *boss_;
public:

	CBossLifeState(Boss *argBoss);
	virtual ~CBossLifeState();

public:

	virtual void Initialize(Player *argPlayer);
	virtual bool Update() = 0;
};