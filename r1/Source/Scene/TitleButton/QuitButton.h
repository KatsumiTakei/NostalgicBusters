/**
 *	@file	QuitButton.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */

#pragma once

#include "TitleButton.h"

class QuitButton final : public TitleButton
{
public:
	QuitButton();
	~QuitButton();

private:
	virtual bool Execute() override;

public:
	virtual bool Enter() override;
	virtual void Draw() override;

};