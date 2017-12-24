/**
 *	@file	GameObject2D.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../stdafx.h"
#include "GameObject.h"

struct ColliderInfo;

class GameObject2D abstract : public GameObject
{
protected:
	Math::Vector2 spd_;
	Math::Vector2 pos_;
	std::shared_ptr<Texture> graph_;
	std::unique_ptr<CShape> collisionShape_;
	std::unique_ptr<ColliderInfo> collider_;

protected:
	virtual void CollisionEnter();

public:
	/**
	 *	@constructor	GameObject2D
	 *	@brief			当たり判定なしのオブジェクト用
	 */
	GameObject2D();
	/**
	 *	@constructor	GameObject2D
	 *	@brief			当たり判定ありのオブジェクト用
	 *	@param[in]		argShapeType	!<	当たり判定の形状
	 */
	GameObject2D(eShapeType argShapeType);
	virtual ~GameObject2D();

public:

	inline void Pos(Math::Vector2 argPos) { pos_ = argPos; }
	inline Math::Vector2 Pos()const { return pos_; }
	inline Math::Vector2 *PosRef() { return &pos_; }

	inline void Spd(Math::Vector2 argSpd) { spd_ = argSpd; }
	inline Math::Vector2 Spd()const { return spd_; }
	inline Math::Vector2 *SpdRef() { return &spd_; }

	inline const std::unique_ptr<CShape> &CollisionShape()const { return collisionShape_; }
	inline const std::unique_ptr<ColliderInfo> &Collider()const { return collider_; }

public:
	virtual bool Update()override;
	virtual void Draw()override;

};