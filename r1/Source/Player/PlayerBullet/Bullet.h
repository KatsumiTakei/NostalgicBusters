/**
 *	@file	Bullet.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "PlayerWeapon.h"

class Bullet : public PlayerWeapon
{
private:
	void CollisionEnter();

public:
	Bullet();
	~Bullet();

public:

	bool Update() override;
	void Draw() override;
};