/**
 *	@file	EnemyCrashEffect.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "EnemyCrashEffect.h"

#include"../Enemy/Enemy.h"
#include "../Enemy/EnemyBullet/EnemyBullet.h"

namespace
{
	const int MaxCount = 30;
}

EnemyCrashEffect::EnemyCrashEffect()
{
	drawMode_ = eDrawMode::Alpha;
	priority_ = ePriority::Efect;
	graph_ = Singleton<GraphManager>::Get()->FindIndexTexture(L"EnemyCrashEffect");
	radius_ = graph_->ImageSize().x * 0.5f;
	addValue_ = graph_->ImageSize().x / MaxCount * 0.1f;
	size_ = addValue_;
}

EnemyCrashEffect *EnemyCrashEffect::Create(Enemy *argEnemy)
{
	count_ = 0;
	pos_ = argEnemy->Pos();
	size_ = addValue_;
	return this;
}

EnemyCrashEffect::~EnemyCrashEffect() = default;

bool EnemyCrashEffect::Update()
{	
	size_ += addValue_;
	return (++count_ < MaxCount);
}

void EnemyCrashEffect::Draw()
{
	const Math::Vector4 Color = Math::Vector4(1, 1, 1, 0.5f);
	DrawRotaGraph(Math::Vector2(pos_.x, pos_.y), Math::Vector2(radius_ * 2.f), Math::Vector2(radius_), size_, 0.f, graph_, Color);
}