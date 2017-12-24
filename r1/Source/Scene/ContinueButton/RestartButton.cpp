/**
 *	@file	RestartButton.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "RestartButton.h"
#include "../../Player/Player.h"

RestartButton::RestartButton(Player *argPlayer)
	:ContinueButton(),player_(argPlayer)
{
	buttonColor_[ContinueButton::Restart] = Math::Vector4(1.f, 1.f, 1.f, 1.f);;
}

RestartButton::~RestartButton() = default;

bool RestartButton::Execute()
{
	player_->Initialize();
	return true;
}

bool RestartButton::Enter()
{
	return ContinueButton::Enter() ? Execute() : false;
}

void RestartButton::Draw()
{
	ContinueButton::Draw();
}
