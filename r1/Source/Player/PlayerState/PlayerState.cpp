/**
 *	@file	PlayerState.cpp
 *	@date	2017 / 11 / 01
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "PlayerState.h"
#include "../Player.h"

PlayerState::PlayerState(Player *argPlayer)
	:player_(argPlayer) 
{
}

PlayerState::~PlayerState() = default;
