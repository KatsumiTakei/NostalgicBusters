/**
 *	@file	Graze.h
 *	@date	2017 / 7 / 27
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../GameObject/GameObject2D.h"

class Player;

namespace
{
	const int SpecifiedValue = 750;
}

class Graze : public GameObject2D
{
private:
	class Impl;
	std::unique_ptr<Impl>pImpl;

public:

	Graze(Player *argPlayer);
	~Graze();


public:
	virtual void CollisionEnter() override;
	virtual bool Update() override;
	virtual void Draw() override;

public:
	int GrazePoint()const;
	void GrazePoint(int argPonit);
};