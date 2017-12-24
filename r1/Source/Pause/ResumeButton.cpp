/**
 *	@file	ResumeButton.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "ResumeButton.h"
#include "Pause.h"

class ResumeButton::Impl
{
public:

	Impl(Pause *argPause)
		:pause_(argPause)
	{
	}
	~Impl() = default;

public:

	bool Execute()
	{
		pause_->IsPause(false);
		return true;
	}

public:
	Pause *pause_;
};

ResumeButton::ResumeButton(Pause *argPause, SceneManager * argSceneManager)
	:PauseButton(argSceneManager), pImpl(std::make_unique<Impl>(argPause))
{
	buttonColor_[PauseButton::Resume] = Math::Vector4(1.f, 1.f, 1.f, 1.f);
}

ResumeButton::~ResumeButton() = default;

bool ResumeButton::Execute()
{
	isEnter_ = false;
	return pImpl->Execute();
}
bool ResumeButton::Enter()
{
	return PauseButton::Enter() ? Execute() : false;
}
void ResumeButton::Draw()
{
	PauseButton::Draw();
}