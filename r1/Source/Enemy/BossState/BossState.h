/**
 *	@file	BossState.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../../stdafx.h"

class Boss;
class Player;

class BossState final
{
private:
	class Impl;
	std::unique_ptr<Impl> pImpl;

protected:
	Boss *boss_;
public:
	BossState(Boss *argBoss, Player *argPlayer);
	virtual ~BossState();

public:
	virtual bool Update();
	virtual void Draw();
	virtual void Finalize();
};