/**
 *	@file	ReturnFirstStageButton.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "ReturnFirstStageButton.h"
#include "Pause.h"
#include "../Scene/SceneManager.h"

ReturnFirstStageButton::ReturnFirstStageButton(SceneManager * argSceneManager)
	:PauseButton(argSceneManager)
{
	buttonColor_[PauseButton::ReturnFirstStage] = Math::Vector4(1.f, 1.f, 1.f, 1.f);
	nextSceneId_ = eSceneID::Play;
}

ReturnFirstStageButton::~ReturnFirstStageButton() = default;

bool ReturnFirstStageButton::Execute()
{
	return PauseButton::Execute();
}
bool ReturnFirstStageButton::Enter()
{
	return PauseButton::Enter();
}
void ReturnFirstStageButton::Draw()
{
	PauseButton::Draw();
}