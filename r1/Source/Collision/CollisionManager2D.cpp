/**
 *	@file	CollisionManager2D.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "CollisionManager2D.h"

#include "../Player/Player.h"
#include "../Player/Graze.h"
#include "../Effect/EnemyCrashEffect.h"
#include "../Player/PlayerBullet/PlayerWeapon.h"
#include "../Player/PlayerBullet/ChargeBullet.h"
#include "../Item/Item.h"
#include "../Enemy/Enemy.h"
#include "../Enemy/EnemyBullet/EnemyBullet.h"

class CollisionManager2D::Impl
{
public:
	Impl() = default;
	~Impl() = default;

public:

	void PlayerToOthers()
	{
		for (auto player : playerColliderList_)
		{
			for (auto enemy : enemyColliderList_)
			{
				if (player->IsDelay() != enemy->IsDelay())
					continue;

				if (IsHitJudgment(*player->CollisionShape().get(), *enemy->CollisionShape().get()))
				{
					enemy->Collider()->PlayerInfo = player;
					enemy->CollisionEnter();
					break;
				}
			}

			for (auto bullet : enemyBulletColliderList_)
			{
				if (player->IsDelay() != bullet->IsDelay())
					continue;

				if (IsHitJudgment(*player->CollisionShape().get(), *bullet->CollisionShape().get()))
				{
					bullet->Collider()->PlayerInfo = player;
					bullet->CollisionEnter();
					break;
				}
			}

			for (auto item : itemColliderList_)
			{
				if (player->IsDelay() != item->IsDelay())
					continue;

				if (IsHitJudgment(*player->CollisionShape().get(), *item->CollisionShape().get()))
				{
					item->Collider()->PlayerInfo = player;
					item->CollisionEnter();
					break;
				}
			}
		}
	}

	void EnemyToOhtres()
	{
		for (auto enemy : enemyColliderList_)
		{
			for (auto bullet : playerWeaponColliderList_)
			{
				if (enemy->IsDelay() != bullet->IsDelay())
					continue;

				if (IsHitJudgment(*enemy->CollisionShape().get(), *bullet->CollisionShape().get()))
				{
					bullet->Collider()->EnemyInfo = enemy;
					bullet->CollisionEnter();
					break;
				}
			}	
			for (auto graze : grazeColliderList_)
			{
				if (graze->IsDelay() != enemy->IsDelay())
					continue;

				if (IsHitJudgment(*graze->CollisionShape().get(), *enemy->CollisionShape().get()))
				{
					graze->Collider()->EnemyInfo = enemy;
					graze->CollisionEnter();
					break;
				}
			}
		}
	}

	void ChargeBulletToOthers()
	{
		for (auto charge : chargeBulletColliderList_)
		{
			for (auto bullet : enemyBulletColliderList_)
			{
				if (charge->IsDelay() != bullet->IsDelay())
					continue;

				if (IsHitJudgment(*charge->CollisionShape().get(), *bullet->CollisionShape().get()))
				{
					bullet->Collider()->ChargeBulletInfo = charge;
				}
			}
			for (auto enemy : enemyColliderList_)
			{
				if (charge->IsDelay() != enemy->IsDelay())
					continue;

				if (IsHitJudgment(*charge->CollisionShape().get(), *enemy->CollisionShape().get()))
				{
					enemy->Collider()->ChargeBulletInfo = charge;
				}
			}
		}
	}

	void EnemyCrashEffectToOthers()
	{
		for (auto lEnemyCrashEffect : eraseRippleColliderList_)
		{
			for (auto enemy : enemyColliderList_)
			{
				if (lEnemyCrashEffect->IsDelay() != enemy->IsDelay())
					continue;

				if (IsHitJudgment(*enemy->CollisionShape().get(), *lEnemyCrashEffect->CollisionShape().get()))
				{
					enemy->Collider()->EraseRippleInfo = lEnemyCrashEffect;
					enemy->CollisionEnter();
					break;
				}
			}
			for (auto bullet : enemyBulletColliderList_)
			{
				if (lEnemyCrashEffect->IsDelay() != bullet->IsDelay())
					continue;

				if (IsHitJudgment(*lEnemyCrashEffect->CollisionShape().get(), *bullet->CollisionShape().get()))
				{
					bullet->Collider()->EraseRippleInfo = lEnemyCrashEffect;
					bullet->CollisionEnter();
					break;
				}
			}
		}
	}

	bool Update()
	{
		PlayerToOthers();
		EnemyToOhtres();
		ChargeBulletToOthers();
		EnemyCrashEffectToOthers();

		return true;
	}
public:
	std::list<Player*>				playerColliderList_;
	std::list<Graze*>				grazeColliderList_;
	std::list<EraseRipple*>			eraseRippleColliderList_;
	std::list<ChargeBullet*>		chargeBulletColliderList_;
	std::list<PlayerWeapon*>		playerWeaponColliderList_;
	std::list<Item*>				itemColliderList_;
	std::list<Enemy*>				enemyColliderList_;
	std::list<EnemyBullet*>			enemyBulletColliderList_;

};

CollisionManager2D::CollisionManager2D()
	:pImpl(std::make_unique<Impl>())
{
}

CollisionManager2D::~CollisionManager2D() = default;

void CollisionManager2D::Entry(Player *argPlayer)
{
	pImpl->playerColliderList_.push_back(argPlayer);
}
void CollisionManager2D::Entry(Graze *argGraze)
{
	pImpl->grazeColliderList_.push_back(argGraze);
}
void CollisionManager2D::Entry(Enemy *argEnemy)
{
	pImpl->enemyColliderList_.push_back(argEnemy);
}
void CollisionManager2D::Entry(Item *argItem)
{
	pImpl->itemColliderList_.push_back(argItem);
}
void CollisionManager2D::Entry(EnemyBullet *argEnemyBullet)
{
	pImpl->enemyBulletColliderList_.push_back(argEnemyBullet);
}
void CollisionManager2D::Entry(PlayerWeapon *argPlayerWeapon)
{
	pImpl->playerWeaponColliderList_.push_back(argPlayerWeapon);
}
void CollisionManager2D::Entry(ChargeBullet *argChargeBullet)
{
	pImpl->chargeBulletColliderList_.push_back(argChargeBullet);
}
void CollisionManager2D::Entry(EraseRipple *argEraseRipple)
{
	pImpl->eraseRippleColliderList_.push_back(argEraseRipple);
}

void CollisionManager2D::Clear()
{
	pImpl->playerColliderList_.clear();
	pImpl->playerWeaponColliderList_.clear();
	pImpl->chargeBulletColliderList_.clear();
	pImpl->grazeColliderList_.clear();
	pImpl->eraseRippleColliderList_.clear();
	pImpl->itemColliderList_.clear();
	pImpl->enemyColliderList_.clear();
	pImpl->enemyBulletColliderList_.clear();
}

bool CollisionManager2D::Update()
{
	pImpl->Update();
	Clear();
	return true;
}