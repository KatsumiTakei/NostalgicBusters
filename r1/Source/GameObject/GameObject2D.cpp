/**
 *	@file	GameObject2D.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "GameObject2D.h"
#include "../Collision/CollisionManager2D.h"

GameObject2D::GameObject2D()
	:collider_(std::make_unique<ColliderInfo>())
{//	当たり判定なし
}

GameObject2D::GameObject2D(eShapeType argShapeType)
	: collider_(std::make_unique<ColliderInfo>())
{//	当たり判定あり

	if (argShapeType == eShapeType::Circle)
	{
		collisionShape_ = std::make_unique<CircleShape>(&pos_, &radius_);
	}
	else if (argShapeType == eShapeType::Rect)
	{
	}
}

GameObject2D::~GameObject2D() = default;

void GameObject2D::CollisionEnter() 
{
}

bool GameObject2D::Update()
{
	return true;
}

void GameObject2D::Draw()
{
#if defined(DEBUG) || defined(_DEBUG)
#if 0
	if (collisionShape_->Type() == eShapeType::Circle)
	{
		CircleGauge gauge;
		gauge.pos_ = pos_;
		gauge.Radius(radius_);
		gauge.property_.Value = 1;
		gauge.GaugeType(eGaugeType::GaugeFill);

		gauge.Draw(Singleton<GraphManager>::Get()->RenderTarget());
	}
	else if (collisionShape_->Type() == eShapeType::Rect)
	{
	}
#endif 
#endif 
}