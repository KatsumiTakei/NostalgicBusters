/**
 *	@file	LeikaShooting.h
 *	@date	2017 / 7 / 30
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../stdafx.h"
#include "Game.h"

class LeikaShooting : public Game 
{
private:
	class Impl;
	std::unique_ptr<Impl> pImpl;

public:

	LeikaShooting();
	~LeikaShooting();

};