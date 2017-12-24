/**
 *	@file	PromptlyReturnTitleButton.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../../stdafx.h"
#include "ContinueButton.h"

class SceneManager;

class PromptlyReturnTitleButton : public ContinueButton
{
private:
	SceneManager *sceneManager_;
public:
	PromptlyReturnTitleButton(SceneManager *argSceneManager);
	~PromptlyReturnTitleButton();

private:
	virtual bool Execute() override;

public:
	virtual bool Enter() override;
	virtual void Draw() override;

};