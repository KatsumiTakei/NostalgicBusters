/**
 *	@file	TitleScene.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "Scene.h"
#include "../stdafx.h"

class SceneManager;


class TitleScene final : public Scene
{
private:
	class Impl;
	std::unique_ptr<Impl>pImpl;

public:
	/**
	 *  @constructor	TitleScene
	 *  @brief			タイトルシーン
	 *	@param[in]		argSceneManager		!<	シーンマネージャー
	 *	@param[in]		argDeviceResources	!<	デバイスリソース
	 */
	TitleScene(SceneManager* argSceneManager, const std::shared_ptr<DeviceResources> &argDeviceResources);
	~TitleScene();

public:
	bool Update() override;
	void Draw() override;
};
