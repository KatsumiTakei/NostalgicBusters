/**
 *	@file	ReturnFirstStageButton.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "PauseButton.h"
#include "../stdafx.h"

class SceneManager;

class ReturnFirstStageButton : public PauseButton
{

public:
	ReturnFirstStageButton(SceneManager *argSceneManager);
	~ReturnFirstStageButton();

private:
	virtual bool Execute() override;
	
public:

	virtual bool Enter() override;
	virtual void Draw();

};