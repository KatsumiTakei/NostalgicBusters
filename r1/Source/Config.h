/**
 *	@file	Config.h
 *	@date	2017 / 10 / 20
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

namespace
{//	定数の設定

	enum class eSide
	{
		Left,
		Right,
		Default,
		Max = 2,
	};

	static const int FlameTime = 60;
	static const int PlayScreenLeft = 24;
	static const int PlayScreenRight = 424;
	static const int PlayScreenTop = 12;
	static const int PlayScreenBottom = 468;

	static RECT PlayArea = { -10, -10, 650, 490 };
}


