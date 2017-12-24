/**
 *	@file	Pause.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../stdafx.h"

class SceneManager;

class Pause
{
private:
	class Impl;
	std::unique_ptr<Impl>pImpl;

public:

	Pause(SceneManager *argSceneManager);
	~Pause();

public:
	bool Update();
	void Draw();

public:
	bool IsPause()const;
	void IsPause(bool argFlag);
};