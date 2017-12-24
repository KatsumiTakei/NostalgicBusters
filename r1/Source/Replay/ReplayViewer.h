/**
 *	@file	ReplayViewer.h
 *	@date	2017 / 7 / 26
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "CReplay.h"
#include "../stdafx.h"

class ReplayViewer final : public CReplay
{
private:
	class Impl;
	std::unique_ptr<Impl> pImpl;
public:
	/**
	 *  @constructor	ReplayViewer
	 *  @brief			リプレイの再生クラス
	 *  @param[in]		argDeviceResources	!<	デバイスリソース
	 */
	ReplayViewer(const std::shared_ptr<DeviceResources> &argDeviceResources);
	~ReplayViewer();
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
	static void CanUseArchive(bool argCanUseArchive);
};


