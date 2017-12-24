/**
 *	@file	GameObject.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../stdafx.h"

class GameObject abstract : public Task
{

protected:
	bool isLiving_;
	int life_, maxLife_;
	int count_;
	float angle_;
	float radius_;

public:
	GameObject()
		: Task(Singleton<TaskManager>::Get()->IsDelay())
	{
	}
	//GameObject() = default;
	virtual ~GameObject() = default;

public:
	inline void IsLiving(bool argState) { isLiving_ = argState; }
	inline bool IsLiving()const { return isLiving_; }

	inline void Count(int value) { count_ = value; }
	inline int Count()const { return count_; }
	
	inline void Life(int value) { life_ = value; }
	inline int Life()const { return life_; }

	inline void Angle(float angle) { angle_ = angle; }
	inline float Angle()const { return angle_; }

	inline void Radius(float radius) { radius_ = radius; }
	inline float Radius()const { return radius_; }

	inline int MaxLife()const { return maxLife_; }

public:
	virtual bool Update()override = 0;
	virtual void Draw()override = 0;

};