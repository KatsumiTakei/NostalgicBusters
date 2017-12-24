/**
 *	@file	Bullet.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "Bullet.h"
#include "../../Enemy/Enemy.h"

#include "../../Collision/CollisionManager2D.h"

Bullet::Bullet()
	:PlayerWeapon(eShapeType::Circle)
{
	spd_.y = BulletSpeed;
	angle_ = 1.f;
	radius_ = 9.f;
	graph_ = Singleton<GraphManager>::Get()->FindIndexTexture(L"PlayerWeapon");
	isLiving_ = false;
}

Bullet::~Bullet() = default;

void Bullet::CollisionEnter()
{
	Enemy *enemy = collider_->EnemyInfo;
	if (enemy)
	{
		enemy->Life(enemy->Life() - 1);
		Singleton<SoundManager>::Get()->PlaySounds("EnemyDamage", eSoundType::Se);
		isLiving_ = false;
	}
	collider_->Clear();
}

bool Bullet::Update()
{
	Singleton<CollisionManager2D>::Get()->Entry(this);

	return PlayerWeapon::Update();
}

void Bullet::Draw()
{
	DrawRotaGraph(pos_, Math::Vector2(18, 18), Math::Vector2(radius_, radius_), 1.0, angle_, graph_);
	GameObject2D::Draw();
}