/**
 *	@file	BallBullet.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "BallBullet.h"

//BallBullet::BallBullet(Enemy *argEnemy, Player *argPlayer)
//	:EnemyBullet(argEnemy, argPlayer, eShapeType::Circle)
//{
//	spd_ = Math::Vector2(2.f, 2.f) * 0.001f;
//	radius_ = 5.f;
//	isLiving_ = false;
//}

BallBullet::BallBullet(Math::Vector2 *argPlayerPos)
	:EnemyBullet(argPlayerPos, eShapeType::Circle)
{
	spd_ = Math::Vector2(2.f, 2.f) * 0.001f;
	radius_ = 5.f;
	isLiving_ = false;
}
BallBullet::~BallBullet() = default;

bool BallBullet::Update()
{
	return EnemyBullet::Update();
}

void BallBullet::Draw()
{
	const float Size = graph_->ImageSize().y;
	EnemyBullet::Draw();
	DrawRectRotaGraph(pos_, Math::Vector2(Size * static_cast<int>(rate_), 0.f), Math::Vector2(Size), 1.0, angle_, graph_, color_);
}