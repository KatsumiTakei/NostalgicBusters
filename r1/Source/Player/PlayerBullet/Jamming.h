/**
 *	@file	Jamming.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../../GameObject/GameObject2D.h"

class Jamming : public GameObject2D
{
private:
	class Impl;
	std::unique_ptr<Impl>pImpl;

public:
	Jamming();
	virtual ~Jamming();

public:
	Jamming *Create();
	bool Update()override;
	void Draw()override;
};