/**
 *	@file	PauseButton.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../stdafx.h"

class SceneManager;
enum class eSceneID;

class PauseButton : public CMenuButton
{
private:
	class Impl;
	std::unique_ptr<Impl> pImpl;

public:
	enum ButtonType
	{
		Resume,
		ReturnTitle,
		ReturnFirstStage,

		Num,
	};
protected:
	Math::Vector4 buttonColor_[Num];
	eSceneID nextSceneId_;
	bool isEnter_;

public:

	PauseButton(SceneManager *argSceneManager);
	~PauseButton();

protected:
	virtual bool Execute() override;

public:
	virtual bool Enter() override;
	virtual void Draw() override;

};