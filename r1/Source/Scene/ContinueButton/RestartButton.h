/**
 *	@file	RestartButton.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */

#pragma once

#include "ContinueButton.h"

class Player;

class RestartButton : public ContinueButton
{
private:
	Player *player_;

public:
	RestartButton(Player *argPlayer);
	~RestartButton();

private:
	virtual bool Execute() override;

public:
	virtual bool Enter() override;
	virtual void Draw() override;

};