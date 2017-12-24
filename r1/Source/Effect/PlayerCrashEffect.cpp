/**
 *	@file	PlayerCrashEffect.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "PlayerCrashEffect.h"
#include "../Config.h"

PlayerCrashEffect::PlayerCrashEffect(float argAngle)
{
	angle_ = argAngle;
	spd_ = Math::Vector2(6.f, 6.f);
	priority_ = ePriority::Efect;
	graph_ = Singleton<GraphManager>::Get()->FindIndexTexture(L"PlayerCrashEffect");
}

PlayerCrashEffect::~PlayerCrashEffect() = default;

PlayerCrashEffect *PlayerCrashEffect::Create(Math::Vector2 argPos)
{
	pos_ = argPos;
	return this;
}

bool PlayerCrashEffect::Update()
{
	pos_.x += cos(angle_) * spd_.x;
	pos_.y += sin(angle_) * spd_.y;

	return IsInArea(pos_, Window::Get()->WindowRect());
}

void PlayerCrashEffect::Draw()
{
	DrawRotaGraph(pos_, graph_->ImageSize(), 1.f, angle_, graph_);
}