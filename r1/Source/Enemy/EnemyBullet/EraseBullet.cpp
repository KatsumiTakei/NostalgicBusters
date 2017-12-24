/**
 *	@file	EraseBullet.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "EraseBullet.h"
#include "../../Collision/CollisionManager2D.h"

//EraseBullet::EraseBullet(Enemy *argEnemy, Player *argPlayer)
//	:EnemyBullet(argEnemy, argPlayer, eShapeType::Circle)
//{
//	spd_ = Math::Vector2(2.f, 2.f) * 0.001f;
//	radius_ = 5.f;
//	isLiving_ = false;
//}


EraseBullet::EraseBullet()
	:EnemyBullet(nullptr, eShapeType::Circle)
{
}
EraseBullet::~EraseBullet() = default;

void EraseBullet::CollisionEnter()
{
	if (collider_->PlayerInfo ||
		collider_->ChargeBulletInfo)
	{
		isLiving_ = false;
	}

	collider_->Clear();
}

bool EraseBullet::Update()
{
	return EnemyBullet::Update();
}

void EraseBullet::Draw()
{
	EnemyBullet::Draw();
	DrawRectRotaGraph(pos_, Math::Vector2(15.f * static_cast<int>(rate_), 0.f), Math::Vector2(15.f, 15.f), 1.0, angle_, graph_, color_);
}