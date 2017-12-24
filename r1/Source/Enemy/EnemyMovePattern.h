/**
 *	@file	EnemyMovePattern.h
 *	@date	2017 /11 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../stdafx.h"

class Enemy;

class EnemyMovePattern
{
private:
	static std::vector<std::function<void(Enemy*)>> movePatterns_;
	static std::once_flag	isInit_;

private:
	static void Create();
private:
	EnemyMovePattern();

public:
	~EnemyMovePattern();

public:

	static std::vector<std::function<void(Enemy*)>> MovePatterns();

};