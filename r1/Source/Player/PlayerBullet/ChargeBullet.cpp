/**
 *	@file	ChargeBullet.cpp
 *	@date	2017 / 11 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "ChargeBullet.h"

#include "../Player.h"
#include "../Graze.h"
#include "../../Collision/CollisionManager2D.h"

ChargeBullet::ChargeBullet()
	:PlayerWeapon(eShapeType::Circle)
{
	isLiving_ = false;
}

ChargeBullet::~ChargeBullet() = default;

ChargeBullet *ChargeBullet::Create(Player *argPlayer, int argLevel)
{
	count_ = 0;
	isLiving_ = true;
	pos_ = argPlayer->Pos();
	OnCreate();
	return this;
}

void ChargeBullet::CollisionEnter()
{
	collider_->Clear();
}

bool ChargeBullet::Update()
{
	Singleton<CollisionManager2D>::Get()->Entry(this);
	return PlayerWeapon::Update();
}

void ChargeBullet::Draw()
{
	GameObject2D::Draw();
}