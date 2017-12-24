/**
 *	@file	TitleButton.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */

#pragma once

#include "../../stdafx.h"

class TitleButton abstract : public CMenuButton
{
private:
	class Impl;
	std::unique_ptr<Impl> pImpl;

public:
	/**
	 *  @enum		eButtonType
	 *  @brief		ボタンの種類
	 */
	enum eButtonType
	{
		Story,
		Battle,
		Config,
		Quit,

		Max,
	};

protected:
	std::wstring introduction_;
	Math::Vector4 buttonColor_[eButtonType::Max];
	bool isEnter_;
public:
	TitleButton();
	~TitleButton();

public:
	virtual bool Execute() override = 0;
	virtual bool Enter() override;
	virtual void Draw() override;

};