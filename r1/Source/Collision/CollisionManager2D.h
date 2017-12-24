/**
 *	@file	CollisionManager2D.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../stdafx.h"

class Player;
class Graze;
class Enemy;
class Item;
class PlayerWeapon;
class ChargeBullet;
class EnemyBullet;
class EraseRipple;

struct ColliderInfo
{
	Player							*PlayerInfo;
	Graze							*GrazeInfo;
	ChargeBullet					*ChargeBulletInfo;
	Enemy							*EnemyInfo;
	Item							*ItemInfo;
	EraseRipple						*EraseRippleInfo;

	ColliderInfo() 
	{
		Clear();
	};
	~ColliderInfo() = default;

	inline void Clear()
	{
		PlayerInfo = nullptr;
		GrazeInfo = nullptr;
		EnemyInfo = nullptr;
		ChargeBulletInfo = nullptr;
		ItemInfo = nullptr;
		EraseRippleInfo = nullptr;
	}
};

class CollisionManager2D final
{
private:

	class Impl;
	std::unique_ptr<Impl> pImpl;

public:

	CollisionManager2D();
	~CollisionManager2D();

	void Entry(Player *argPlayer);
	void Entry(Graze *argGraze);
	void Entry(Enemy *argEnemy);
	void Entry(Item *argItem);
	void Entry(EnemyBullet *argEnemyBullet);
	void Entry(PlayerWeapon *argPlayerWeapon);
	void Entry(ChargeBullet *argChargeBullet);
	void Entry(EraseRipple *argEraseRipple);

	void Clear();
	bool Update();

};