/**
 * @file   GameObject.h
 * @brief  GameObject�̐錾
 * @author Katsumi Takei
 * @date   2017 / 01 / 20
 */
#pragma once

#include "GameObject.h"

class GameObject3D : public GameObject
{

protected:

	eDrawMode mode_;
	Math::Vector3 spd_;
	Math::Vector3 pos_;
	
public:
	GameObject3D();
	virtual ~GameObject3D() {}

public:

	inline void Pos(Math::Vector3 argPos) { pos_ = argPos; }
	inline Math::Vector3 Pos()const { return pos_; }

	inline void Spd(Math::Vector3 argSpd) { spd_ = argSpd; }
	inline Math::Vector3 Spd()const { return spd_; }

public:
	virtual bool Update()override;
	virtual void Draw()override;

};