/**
 *	@file	Scene.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../stdafx.h"

class SceneManager;

class Scene abstract
{
protected:
	SceneManager *sceneManager_;
	std::shared_ptr<DeviceResources> deviceResources_;
	std::unique_ptr<CFade>		fadeIn_;
	std::unique_ptr<CFade>		fadeOut_;
	CFade		*fadeCurrent_;
public:
	/**
	 *	@constructor	Scene
	 *	@brief			シーンの抽象クラス
	 *	@param[in]		argSceneManager		!<	シーンマネージャー
	 *	@param[in]		argDeviceResources	!<	デバイスリソース
	 */
	Scene(SceneManager *argSceneManager, const std::shared_ptr<DeviceResources> &argDeviceResources) 
		: sceneManager_(argSceneManager), deviceResources_(argDeviceResources)
	{
	}
	virtual ~Scene() = default;

public:
	/**
	 *	@fn			Update
	 *	@brief		更新
	 *	@retval		true	!<	更新成功
	 *	@retval		false	!<	更新失敗
	 */
	virtual bool Update() = 0;
	/**
	 *	@fn			Draw
	 *	@brief		描画
	 */
	virtual void Draw() = 0;

	inline CFade *Fade() const { return fadeCurrent_; }
	inline void ChangeFade() 
	{ 
		(fadeCurrent_ == fadeIn_.get()) ?
			fadeCurrent_ = fadeOut_.get() :
			fadeCurrent_ = fadeIn_.get();
	}
};


