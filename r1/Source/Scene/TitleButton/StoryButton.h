/**
 *	@file	BattleButton.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../../stdafx.h"
#include "TitleButton.h"

class SceneManager;

class StoryButton final : public TitleButton
{
private:
	class Impl;
	std::unique_ptr<Impl> pImpl;

public:
	StoryButton(SceneManager *argSceneManager);
	~StoryButton();

private:
	virtual bool Execute() override;

public:
	virtual bool Enter() override;
	virtual void Draw() override;

};