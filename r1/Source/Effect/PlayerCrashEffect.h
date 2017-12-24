/**
 *	@file	PlayerCrashEffect.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../GameObject/GameObject2D.h"

class PlayerCrashEffect : public GameObject2D
{

public:
	PlayerCrashEffect(float argAngle);
	~PlayerCrashEffect();

	PlayerCrashEffect *Create(Math::Vector2 argPos);

public:
	bool Update()override;
	void Draw()override;

};