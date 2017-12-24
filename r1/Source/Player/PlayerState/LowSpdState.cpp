/**
 *	@file	LowSpdState.cpp
 *	@date	2017 / 11 / 01
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "LowSpdState.h"
#include "../Player.h"

class LowSpdState::Impl
{
public:
	Impl(LowSpdState *argState)
		:collisionMark_(std::make_unique<CircleGauge>()), state_(argState)
	{
		lowSpd_ = state_->player_->Spd() * 0.25f;
		collisionMark_->property_.MaxValue = 1.f;
		collisionMark_->property_.Value = 1.f;
		collisionMark_->Radius(state_->player_->Radius());
		collisionMark_->GaugeType(eGaugeType::GaugeFill);
		collisionMark_->Color(D2D1::ColorF(1.f, 0.5, 0.f, 0.5f));
		graph_ = Singleton<GraphManager>::Get()->FindIndexTexture(L"Vital");
	}
	~Impl() = default;

public:

	void Update()
	{
		collisionMark_->pos_ = state_->player_->Pos();
		state_->player_->Spd(lowSpd_);
	}

	void Draw()
	{// あたり判定の表示
		const Math::Vector2 Size = Math::Vector2(50.f, 50.f);
		DrawRotaGraph(state_->player_->Pos(), Size, Size * 0.5f, 1.0, state_->player_->Angle(), graph_, Math::Vector4(1.f, 1.f, 1.f, 0.4f));
		collisionMark_->Draw(Singleton<GraphManager>::Get()->RenderTarget());
	}

public:
	Math::Vector2 lowSpd_;

private:

	std::unique_ptr<CircleGauge>	collisionMark_;
	LowSpdState						*state_;
	std::shared_ptr<Texture> graph_;

};

LowSpdState::LowSpdState(Player * argPlayer)
	:PlayerState(argPlayer), pImpl(std::make_unique<Impl>(this))
{
}

LowSpdState::~LowSpdState() = default;


bool LowSpdState::Update()
{
	pImpl->Update();

	return true;
}

void LowSpdState::Draw()
{
	pImpl->Draw();
}

void LowSpdState::OnChangeState()
{
	player_->Spd(pImpl->lowSpd_);
}
