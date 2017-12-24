/**
 *	@file	PromptlyReturnTitleButton.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "PromptlyReturnTitleButton.h"

#include "../../Scene/SceneManager.h"

PromptlyReturnTitleButton::PromptlyReturnTitleButton(SceneManager *argSceneManager)
	:ContinueButton(), sceneManager_(argSceneManager)
{
	buttonColor_[ContinueButton::ReturnTitle] = Math::Vector4(1.f, 1.f, 1.f, 1.f);
}

PromptlyReturnTitleButton::~PromptlyReturnTitleButton() = default;

bool PromptlyReturnTitleButton::Execute()
{
	sceneManager_->ChangeScene(eSceneID::Title);
	return true;
}

bool PromptlyReturnTitleButton::Enter()
{
	return ContinueButton::Enter() ? Execute() : false;
}

void PromptlyReturnTitleButton::Draw()
{
	ContinueButton::Draw();
}
