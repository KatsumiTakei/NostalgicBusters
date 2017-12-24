/**
 *	@file	PlayerAi.h
 *	@date	2017 / 11 / 10
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */

#pragma once

#include "../../stdafx.h"

class GameObject2D;
class Player;

struct ObstacleData
{
	Math::Vector2 Pos, Spd;
	int Interval, Count;
	float Angle, Accel;
};

class ObstacleManager
{

private:
	class Impl;
	std::unique_ptr<Impl> pImpl;

public:
	ObstacleManager();
	~ObstacleManager();

public:

	void Entry(GameObject2D *argObstacle, bool argIsDelay);
	void Entry(GameObject2D *argObstacle, int argGameSide);
	void Update();
	void Clear(bool argIsDelay);
	void Clear();
	std::vector<GameObject2D*> Obstacles(bool argIsDeray)const;

};