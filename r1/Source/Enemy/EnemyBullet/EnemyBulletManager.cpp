/**
 *	@file	EnemyBulletManager.cpp
 *	@date	2017 / 12 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "EnemyBulletManager.h"
#include "../../Player/Ai/Obstacle.h"

#include "BallBullet.h"
#include "../Enemy.h"
#include "../../Player/Player.h"

class EnemyBulletManager::Impl 
{
public:
	Impl() = default;
	~Impl()
	{
		for (int lIndex = 0; lIndex < static_cast<int>(eSide::Max); lIndex++)
		{
			for (int lBulletIndex = 0; lBulletIndex < BulletMax; lBulletIndex++)
			{
				delete bullets_[lBulletIndex][lIndex];
			}
		}
	}

public:
	EnemyBullet *Create(Enemy* argEnemy, Math::Vector2 argGeneratePos, float argAccel, float argInterval)
	{
		EnemyBullet *bullet = nullptr;
		const int Side = (argEnemy->IsDelay()) ? 1 : 0;

		for (int lBulletIndex = 0; lBulletIndex < BulletMax; lBulletIndex++)
		{
			if (bullets_[lBulletIndex][Side]->IsLiving())
				continue;

			bullet = bullets_[lBulletIndex][Side];
		}
		
		Singleton<ObstacleManager>::Get()->Entry(bullet, bullet->IsDelay());
		return bullet->Create(argEnemy, argGeneratePos, argAccel, argInterval);
	}

public:
	static const int BulletMax = 255;
	EnemyBullet* bullets_[BulletMax][static_cast<int>(eSide::Max)];
};

EnemyBulletManager::EnemyBulletManager()
	:pImpl(std::make_unique<Impl>())
{
}

EnemyBulletManager::~EnemyBulletManager() = default;

void EnemyBulletManager::SetBullets(Player *argPlayer)
{
	for (int lBulletIndex = 0; lBulletIndex < pImpl->BulletMax; lBulletIndex++)
	{ 
		const int GameSide = (argPlayer->IsDelay()) ? 1 : 0;
		pImpl->bullets_[lBulletIndex][GameSide] = new BallBullet(argPlayer->PosRef());
	}
}

EnemyBullet * EnemyBulletManager::Create(Enemy* argEnemy, Math::Vector2 argGeneratePos, float argAccel, float argInterval)
{
	if (!canShoot_)
		return nullptr;

	return pImpl->Create(argEnemy, argGeneratePos, argAccel, argInterval);
}

void EnemyBulletManager::CreateAndRegist(Enemy * argEnemy, Math::Vector2 argGeneratePos, float argAccel, float argInterval)
{
	if (!canShoot_)
		return;

	const auto Bullet = pImpl->Create(argEnemy, argGeneratePos, argAccel, argInterval);
	Singleton<TaskManager>::Get()->Add(Bullet);
}

int EnemyBulletManager::BulletMax() const
{
	return pImpl->BulletMax;
}

EnemyBullet ** EnemyBulletManager::Bullets(bool argIsDelay) const
{
	const int GameSide = (argIsDelay) ? 1 : 0;
	return pImpl->bullets_[GameSide];
}

EnemyBullet **EnemyBulletManager::Bullets(int argGameSide) const
{
	return pImpl->bullets_[argGameSide];
}

void EnemyBulletManager::Clear(bool argIsDelay)
{
	const int GameSide = (argIsDelay) ? 1 : 0;
	for (int lBulletIndex = 0; lBulletIndex < Impl::BulletMax; lBulletIndex++)
	{
		pImpl->bullets_[lBulletIndex][GameSide]->IsLiving(false);
	}
}

void EnemyBulletManager::Clear(int argGameSide)
{
	for (int lBulletIndex = 0; lBulletIndex < Impl::BulletMax; lBulletIndex++)
	{
		pImpl->bullets_[lBulletIndex][argGameSide]->IsLiving(false);
	}
}

void EnemyBulletManager::Clear()
{
	for (int lIndex = 0; lIndex < static_cast<int>(eSide::Max); lIndex++)
	{
		for (int lBulletIndex = 0; lBulletIndex < Impl::BulletMax; lBulletIndex++)
		{
			pImpl->bullets_[lBulletIndex][lIndex]->IsLiving(false);
		}
	}
}
