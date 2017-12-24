/**
 *	@file	ContinueButton.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */

#pragma once

#include "../../stdafx.h"

class ContinueButton : public CMenuButton
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
		Restart,
		ReturnTitle,

		Max,
	};

protected:
	Math::Vector4 buttonColor_[eButtonType::Max];

public:
	ContinueButton();
	~ContinueButton();

protected:
	virtual bool Execute() override = 0;

public:

	virtual bool Enter() override;
	virtual void Draw() override;

};