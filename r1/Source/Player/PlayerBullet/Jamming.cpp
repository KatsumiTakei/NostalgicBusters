/**
 *	@file	Jamming.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "Jamming.h"

#include "../../Enemy/EnemyFactory/EnemyManager.h"
#include "../../Enemy/Enemy.h"
#include "../../Enemy/EnemyBullet/EnemyBullet.h"
#include "../../Collision/CollisionManager2D.h"
#include "../../Config.h"

class Jamming::Impl
{
public:
	Impl()
	{
		
	}
	~Impl() = default;


public:

};

Jamming::Jamming()
	:pImpl(std::make_unique<Impl>()), GameObject2D(eShapeType::Circle)
{
	pos_ = Math::Vector2();
	angle_ = 0.f;
	graph_ = Singleton<GraphManager>::Get()->FindIndexTexture(L"screweffect");
	drawMode_ = eDrawMode::Add;
	priority_ = ePriority::Efect;
}

Jamming::~Jamming() = default;


Jamming *Jamming::Create()
{
	return this;
}

bool Jamming::Update()
{
	//Singleton<CollisionManager2D>::Get()->Entry(this);
	
	return isLiving_;
}

void Jamming::Draw()
{
	DrawRotaGraph(pos_ + Math::Vector2(), Math::Vector2(), Math::Vector2(), Math::Vector2(), angle_, graph_, Math::Vector4());
}