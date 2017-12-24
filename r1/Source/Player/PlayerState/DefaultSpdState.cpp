/**
 *	@file	DefaultSpdState.cpp
 *	@date	2017 / 11 / 01
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "DefaultSpdState.h"
#include "../Player.h"

class DefaultSpdState::Impl
{
public:
	Impl(DefaultSpdState*argState)
		: state_(argState)
	{
		defaultSpd_ = state_->player_->Spd();
	}
	~Impl() = default;

public:
	void Update()
	{
	}

public:
	Math::Vector2 defaultSpd_;

private:
	DefaultSpdState	*state_;
};

DefaultSpdState::DefaultSpdState(Player * argPlayer)
	:PlayerState(argPlayer),pImpl(std::make_unique<Impl>(this))
{
}

DefaultSpdState ::~DefaultSpdState() = default;

bool DefaultSpdState::Update()
{
	return true;
}

void DefaultSpdState::Draw()
{
}

void DefaultSpdState::OnChangeState()
{
	player_->Spd(pImpl->defaultSpd_);
}
