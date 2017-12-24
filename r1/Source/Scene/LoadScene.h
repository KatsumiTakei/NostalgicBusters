/**
*	@file	LoadScene.h
*	@date	2017 / 12 / 25
*	@author	Katsumi Takei
*	Copyright (c) Kastumi Takei. All rights reserved.
*/
#pragma once

#include "Scene.h"
#include "../stdafx.h"

class SceneManager;

class LoadScene final : public Scene
{
private:
	class Impl;
	std::unique_ptr<Impl>pImpl;

public:
	/**
	*  @constructor	PlayScene
	*  @brief			プレイシーン
	*	@param[in]		argSceneManager		!<	シーンマネージャー
	*	@param[in]		argDeviceResources	!<	デバイスリソース
	*/
	LoadScene(SceneManager *argSceneManager, const std::shared_ptr<DeviceResources> &argDeviceResources);
	~LoadScene();

public:
	bool Update() override;
	void Draw() override;
};
