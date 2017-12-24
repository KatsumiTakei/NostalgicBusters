/**
 *	@file	Game.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../stdafx.h"

class SceneManager;

class Game abstract
{
private:
	class Impl;
	std::unique_ptr<Impl> pImpl;

protected:
	std::shared_ptr<DeviceResources> deviceResources_;
	std::unique_ptr<SceneManager> sceneManager_;
	std::unique_ptr<QueryPerformanceTimer> fps_;
	Archive							*archive_;

public:

	Game();
	~Game();
public:
	MSG Run();
};