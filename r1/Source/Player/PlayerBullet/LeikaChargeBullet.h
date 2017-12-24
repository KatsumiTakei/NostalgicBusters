/**
 *	@file	LeikaChargeBullet.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "ChargeBullet.h"

class Player;

class LeikaChargeBullet : public ChargeBullet
{
public:

	LeikaChargeBullet();
	~LeikaChargeBullet();
public:
	virtual void OnCreate();

public:
	bool Update()override;
	void Draw()override;
};