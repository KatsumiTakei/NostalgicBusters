/**
 *	@file	PlayerAi.h
 *	@date	2017 / 11 / 10
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */

#pragma once

#include "../../stdafx.h"

class Enemy;
class Player;

class PlayerAi
{
private:
	class Impl;
	std::unique_ptr<Impl> pImpl;

public:
	PlayerAi(Player *argPlayer);
	~PlayerAi();
public:

	void Update();
	void Draw();
};