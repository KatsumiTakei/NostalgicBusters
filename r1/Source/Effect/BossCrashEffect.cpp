/**
 *	@file	BossCrashEffect.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "BossCrashEffect.h"

BossCrashEffect::BossCrashEffect()
	:startMoveCnt_(0)
{
	priority_ = ePriority::Efect;
	drawMode_ = eDrawMode::Add;
	graph_ = Singleton<GraphManager>::Get()->FindIndexTexture(L"BossCrashEffect");
	radius_ = graph_->ImageSize().x * 0.5f;
}

BossCrashEffect::~BossCrashEffect() = default;

BossCrashEffect *BossCrashEffect::Create(Math::Vector2 argPos, int argCount)
{
	pos_ = argPos + Math::Vector2(static_cast<float>(15 - Math::XorShift() % 30), static_cast<float>(15 - Math::XorShift() % 30));
	spd_ = Math::Vector2(static_cast<float>(15 - Math::XorShift() % 30), static_cast<float>(15 - Math::XorShift() % 30));
	angle_ = Math::ToRadian(static_cast<float>(Math::XorShift() % 360));
	count_ = argCount * 2;
	startMoveCnt_ = static_cast<int>(count_ * 0.5);
	Singleton<SoundManager>::Get()->PlaySounds("BossCrash", eSoundType::Se);

	return this;
}

bool BossCrashEffect::Update()
{
	if (--count_ < startMoveCnt_)
	{
		pos_.x += cos(angle_) * spd_.x;
		pos_.y += sin(angle_) * spd_.y;
	}
	return (count_ > 0);
}

void BossCrashEffect::Draw()
{
	const float R = static_cast<float>(Math::XorShift() % 10 + 1) * 0.1f;
	const float G = static_cast<float>(Math::XorShift() % 10 + 1) * 0.1f;
	const float B = static_cast<float>(Math::XorShift() % 10 + 1) * 0.1f;
	DrawRotaGraph(pos_, graph_->ImageSize(), Math::Vector2(radius_, radius_), Math::Vector2(1.0), angle_, graph_, Math::Vector4(R, G, B, 1.f));
}