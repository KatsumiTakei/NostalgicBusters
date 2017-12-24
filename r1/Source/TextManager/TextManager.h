/**
 *	@file	TextManager.h
 *	@date	2017 / 10 / 20
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../stdafx.h"

class TextManager final
{
private:
	class Impl;
	std::unique_ptr<Impl>pImpl;

public:
	TextManager();
	~TextManager();

public:
	void Initialize();
	bool Update();
	void Draw();
public:

	void IsFinished(bool argFlag);
	bool IsFinished()const;

	/**
	 *  @fn			TextPath
	 *  @brief		テキストのパスを指定
	 *  @param[in]	argPath	!<	パス
	 */
	void TextPath(std::string argPath);
	/**
	 *  @fn			TextPath
	 *  @brief		テキストのパスを取得
	 *  @return		パス
	 */
	std::string TextPath()const;
};