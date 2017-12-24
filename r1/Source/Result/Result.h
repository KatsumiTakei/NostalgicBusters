/**
 *	@file	Result.h
 *	@date	2017 / 7 / 20
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../GameObject/GameObject2D.h"

class Result final : public GameObject2D
{
private:
	class Impl;
	std::unique_ptr<Impl>pImpl;

public:

	Result();
	~Result();
public:
	Result *Create();

public:

	bool Update()override;
	void Draw()override;
};