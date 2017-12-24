/**
 *	@file	UiDrawer.h
 *	@date	2017 / 10 / 20
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../stdafx.h"
#include "../GameObject/GameObject2D.h"

class Player;

class UiDrawer final : public GameObject2D
{
private:
	class Impl;
	std::unique_ptr<Impl>pImpl;
public:
	/**
	 *	@constructor	UiDrawer
	 *	@brief			UIの表示
	 *	@param[in]		argPlayer	!<	プレイヤー
	 */
	UiDrawer(Player *argPlayer);
	~UiDrawer();
public:

	bool Update() override;
	void Draw() override;
};