/**
 *	@file	LeikaChargeBullet.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "LeikaChargeBullet.h"

#include "../Player.h"
#include "../Graze.h"
#include "../../Collision/CollisionManager2D.h"

namespace
{
	const Math::Vector2 GraphSize = Math::Vector2(30.f, 30.f);
}

LeikaChargeBullet::LeikaChargeBullet()
{
	isLiving_ = false;
}

LeikaChargeBullet::~LeikaChargeBullet() = default;

void LeikaChargeBullet::OnCreate()
{

}

bool LeikaChargeBullet::Update()
{
	Singleton<CollisionManager2D>::Get()->Entry(this);
	return ChargeBullet::Update();
}

void LeikaChargeBullet::Draw()
{
	ChargeBullet::Draw();

	//(count_ % 2 == 0) ?	DrawRectRotaGraph(pos_, Math::Vector2(0.f, 0.f), GraphSize, radius_ / 30.f, Math::ToRadian(0.f), graph_, Math::Vector4(1, 1, 1, 1)) :
	//	DrawRectRotaGraph(pos_, Math::Vector2(GraphSize.x, 0.f), GraphSize, radius_ / 30.f, Math::ToRadian(0.f), graph_, Math::Vector4(1, 1, 1, 1));
}