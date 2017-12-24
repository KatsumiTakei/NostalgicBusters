/**
 *	@file	ReplayRecorder.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "CReplay.h"
#include "../stdafx.h"

class Player;

class ReplayRecorder final : public CReplay
{
private:
	Player *player_;

public:
	/**
	 *  @constructor	ReplayRecorder
	 *  @brief			リプレイの記録クラス
	 *  @param[in]		argPlayer	!<	プレイヤー
	 */
	ReplayRecorder(Player *argPlayer);
	~ReplayRecorder();

public:
	void Reset(int argCount, double argPower);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
};