/**
 *	@file	IReplay.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

/**
 *  @class	IReplay
 *  @brief	リプレイのインターフェイス
 */
class IReplay
{
public:
	virtual ~IReplay() = default;
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Finalize() = 0;
	virtual bool IsPlaying() const = 0;
};
