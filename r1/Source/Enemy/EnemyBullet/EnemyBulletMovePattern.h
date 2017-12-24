/**
 *	@file	EnemyBulletMovePattern.h
 *	@date	2017 /11 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../../stdafx.h"

class EnemyBullet;

class EnemyBulletMovePattern
{
private:
	static std::vector<std::function<void(EnemyBullet*)>> movePatterns_;
	static std::once_flag	isInit_;

private:
	static void Create();
private:
	EnemyBulletMovePattern();

public:
	~EnemyBulletMovePattern();

public:

	static std::vector<std::function<void(EnemyBullet*)>> MovePatterns();

};