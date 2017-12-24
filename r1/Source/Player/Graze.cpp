/**
 *	@file	Graze.cpp
 *	@date	2017 / 7 / 27
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "Graze.h"
#include "../Player/Player.h"
#include "../Collision/CollisionManager2D.h"

#include "../Enemy/Enemy.h"
#include "../Enemy/Boss.h"
#include "../Enemy/EnemyBullet/EnemyBullet.h"

class Graze::Impl
{
public:

	Impl(Player *argPlayer) : player_(argPlayer), grazePoint_(0),
		isHit_(false)
	{
	}

	~Impl() = default;

public:
	bool isHit_;
	int grazePoint_;
	Player *player_;
};

Graze::Graze(Player *argPlayer)
	:GameObject2D(eShapeType::Circle), pImpl(std::make_unique<Impl>(argPlayer))
{
	pos_ = argPlayer->Pos();
	radius_ = argPlayer->Radius() * 5.f;
	angle_ = 0.f;
	isLiving_ = true;
	priority_ = ePriority::GameObjectFoword;
	count_ = 0;
}

Graze::~Graze() = default;


void Graze::CollisionEnter()
{
	if (collider_->EnemyInfo)
	{
		pImpl->grazePoint_++;
		Singleton<SoundManager>::Get()->PlaySounds("Graze", eSoundType::Se);
		pImpl->isHit_ = true;
	}
	//for (auto bullet : Collider()->EnemyBulletListInfo)
	//{
	//	pImpl->grazePoint_++;
	//	Singleton<SoundManager>::Get()->PlaySounds("Graze", eSoundType::Se);
	//	pImpl->isHit_ = true;
	//}
	collider_->Clear();

}

bool Graze::Update()
{
	pImpl->isHit_ = false;
	pos_ = pImpl->player_->Pos();
	radius_ = pImpl->player_->Radius() * 5.f;

	angle_ = (++count_ % 2 == 0) ? 0.f : 180.f;
	

	Singleton<CollisionManager2D>::Get()->Entry(this);

	return true;
}

void Graze::Draw()
{
	if (!pImpl->isHit_)
		return;

#if defined (DEBUG) || defined (_DEBUG)
	//GameObject::draw();
#endif
}

int Graze::GrazePoint()const 
{ 
	return pImpl->grazePoint_; 
}

void Graze::GrazePoint(int argPonit)
{
	pImpl->grazePoint_ = argPonit;
}