/**
 *	@file	Leika.h
 *	@date	2017 / 11 / 02
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../stdafx.h"
#include "Player.h"

class Leika : public Player
{
private:
	class Impl;
	std::unique_ptr<Impl>pImpl;

public:
	Leika(const int argPlayerIndex);
	~Leika();

public:
	void ShotBullet() override;
	void Initialize() override;

	void CollisionEnter() override;
	bool Update() override;
	void Draw() override;

};