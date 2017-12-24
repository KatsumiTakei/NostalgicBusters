/**
 *	@file	ResumeButton.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "PauseButton.h"
#include "../stdafx.h"

class SceneManager;
class Pause;

class ResumeButton : public PauseButton
{
private:
	class Impl;
	std::unique_ptr<Impl> pImpl;

public:
	ResumeButton(Pause *argPause, SceneManager *argSceneManager);
	~ResumeButton();

private:
	virtual bool Execute() override;

public:
	virtual bool Enter() override;
	virtual void Draw();

};