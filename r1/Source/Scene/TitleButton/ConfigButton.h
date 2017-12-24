/**
 *	@file	QuitButton.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */

#pragma once

#include "TitleButton.h"

class ConfigButton final : public TitleButton
{
private:
	class Impl;
	std::unique_ptr<Impl> pImpl;

public:
	ConfigButton();
	~ConfigButton();

private:
	virtual bool Execute() override;

public:
	virtual bool Enter() override;
	virtual void Draw() override;

};