/**
 *	@file	PlayerOption.h
 *	@date	2017 / 11 / 01
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../stdafx.h"

class PlayerOption
{
private:
	class Impl;
	std::unique_ptr<Impl> pImpl;

public:
	PlayerOption(const int argMax);
	~PlayerOption();

public:
	void Update(const Math::Vector2 argPlayerPos, const Math::Vector2 *argShotPos);
	void Draw(const int argShotMax);


};
