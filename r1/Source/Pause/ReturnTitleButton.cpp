/**
 *	@file	ReturnTitleButton.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "ReturnTitleButton.h"
#include "../Scene/SceneManager.h"


ReturnTitleButton::ReturnTitleButton(SceneManager *argSceneManager)
	:PauseButton(argSceneManager)
{
	buttonColor_[PauseButton::ReturnTitle] = Math::Vector4(1.f, 1.f, 1.f, 1.f);
	nextSceneId_ = eSceneID::Title;
}

ReturnTitleButton::~ReturnTitleButton() = default;

bool ReturnTitleButton::Execute()
{
	return PauseButton::Execute();
}
bool ReturnTitleButton::Enter()
{
	return PauseButton::Enter();
}
void ReturnTitleButton::Draw()
{
	PauseButton::Draw();
}