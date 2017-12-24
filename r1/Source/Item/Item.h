/**
 *	@file	Item.h
 *	@date	2017 / 7 / 20
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../stdafx.h"
#include "../GameObject/GameObject2D.h"

class Player;

enum class eItemId : int
{
	PowerUpItemLittle,
	PowerUpItemBig,
	ScoreUpItem,
	BombUpItem,
	LifeUpItem,
};

class Item : public GameObject2D
{
private:
	class Impl;
	std::unique_ptr<Impl> pImpl;

public:
	Item(eItemId argItemId, Player *argPlayer);
	~Item();
	Item *Create(Math::Vector2 argPos);

public:
	virtual void CollisionEnter() override;
	virtual bool Update() override;
	virtual void Draw() override;

public:

	void Pick(int *argMaxPowerReport);

};