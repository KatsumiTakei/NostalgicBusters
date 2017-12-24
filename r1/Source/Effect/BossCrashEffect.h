/**
 *	@file	BossCrashEffect.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../GameObject/GameObject2D.h"

class BossCrashEffect final : public GameObject2D
{
private:
	int startMoveCnt_;

public:

	BossCrashEffect();
	~BossCrashEffect();
	BossCrashEffect *Create(Math::Vector2 argPos, int count);
public:
	
	bool Update()override;
	void Draw()override;
};