/**
 *	@file	Stage01.h
 *	@date	2017 / 7 / 26
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "Stage.h"
#include "../stdafx.h"

class Player;

class Stage01 : public Stage
{

private:
	class Impl;
	std::unique_ptr<Impl>pImpl;

private:

	/**
	 *  @fn		ScrollBack
	 *  @brief	背景のスクロール
	 */
	void ScrollBack() override;

public:

	/**
	 *  @constructor		Stage01
	 *  @brief				ステージ01
	 *  @param[in]			argPlayer			!<	プレイヤー
	 *  @param[in]			argDeviceResources	!<	リソース
	 *  @param[in]			argNextStage		!<	次のステージ
	 *  @param[in]			argSoundFileName	!<	ステージのサウンドパス
	 */
	Stage01(Player *argPlayer, const std::shared_ptr<DeviceResources> &argDeviceResources, Stage *argNextStage, std::string argSoundFileName, std::string argViewPortName);
	~Stage01();

public:
	bool Update() override;
	void Draw() override;
};