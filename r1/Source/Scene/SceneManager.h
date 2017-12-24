/**
 *	@file	SceneManager.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../stdafx.h"

/**
 *	@enum		eSceneID
 *	@brief		シーンのID
 */
enum class eSceneID
{
	None,
	Load,
	Title,
	Play,
};

class SceneManager final
{
private:
	class Impl;
	std::unique_ptr<Impl> pImpl;

public:
	/**
	 *	@constructor	SceneManager
	 *	@brief			シーンマネージャー
	 *	@param[in]		argStartSceneID		!<	スタートシーンのID
	 *	@param[in]		argDeviceResources	!<	デバイスリソース
	 */
	SceneManager(eSceneID argStartSceneID, const std::shared_ptr<DeviceResources> &argDeviceResources);
	~SceneManager();

public:

	/**
	 *	@fn			FadeChangeScene
	 *	@brief		シーンの変更
	 *	@param[in]	argNextSceneID	!<	次のシーンのID
	 */
	void FadeChangeScene(eSceneID argNextSceneID);
	/**
	 *	@fn			ChangeScene
	 *	@brief		シーンの変更
	 *	@param[in]	argNextSceneID	!<	次のシーンのID
	 */
	void ChangeScene(eSceneID argNextSceneID);
	
	/**
	 *	@fn			Update
	 *	@brief		更新
	 *	@retval		true	!<	シーンの更新、又はフェード中
	 *	@retval		false	!<	フェード終了
	 */
	bool Update();
	/**
	 *	@fn			Draw
	 *	@brief		描画
	 */
	void Draw();

	eSceneID CurrentScene()const;
};
