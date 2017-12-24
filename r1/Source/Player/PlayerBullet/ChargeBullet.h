/**
 *	@file	ChargeBullet.h
 *	@date	2017 / 11 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "PlayerWeapon.h"

class Player;

class ChargeBullet : public PlayerWeapon
{
public:
	ChargeBullet();
	virtual ~ChargeBullet();
public:
	ChargeBullet *Create(Player *argPlayer, int argLevel);
	virtual void OnCreate() = 0;

private:
	void CollisionEnter() override;

public:
	bool Update()override;
	void Draw()override;
};