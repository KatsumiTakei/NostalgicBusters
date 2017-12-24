/**
 *	@file	EraseRipple.h
 *	@date	2017 /11 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../../GameObject/GameObject2D.h"

class EraseRipple : public GameObject2D
{
protected:
	float size_;
	float addValue_;

public:

	EraseRipple();
	virtual ~EraseRipple();

};