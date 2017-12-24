/**
 *	@file	SmallEnemy.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "SmallEnemy.h"
#include "../Effect/EnemyCrashEffect.h"
#include "EnemyBullet/BallBullet.h"
#include "EnemyBullet/EnemyBulletManager.h"
#include "../Player/PlayerBullet/ChargeBullet.h"

#include "../Item/Item.h"
#include "../Player/Player.h"
#include "../Player/Ai/Obstacle.h"

#include "../Collision/CollisionManager2D.h"
#include "../ScoreManager/ScoreManager.h"
#include "../Config.h"

class SmallEnemy::Impl
{
public:
	Impl(SmallEnemy *argSmallEnemy, eItemId argItemId)
	{
		smallEnemy_ = argSmallEnemy;
		enemyCrashEffect_ = std::make_unique<EnemyCrashEffect>();
	}
	~Impl() = default;

	void Finalize()
	{
		TaskManager *taskManager = Singleton<TaskManager>::Get();
		taskManager->Add(enemyCrashEffect_->Create(smallEnemy_));
		Singleton<SoundManager>::Get()->PlaySounds("EnemyCrash", eSoundType::Se);
	}

public:
	std::unique_ptr<EnemyCrashEffect>	enemyCrashEffect_;
	SmallEnemy							*smallEnemy_;
};

SmallEnemy::SmallEnemy(Player *argPlayer, eItemId argItemId)
	:Enemy(), pImpl(std::make_unique<Impl>(this, argItemId))
{
	graph_ = Singleton<GraphManager>::Get()->FindIndexTexture(L"Enemies");
}

SmallEnemy::~SmallEnemy() = default;

void SmallEnemy::SetEnemy(int argPattern, int argBrknd, Math::Vector2 argPos, Math::Vector2 argSpd, 
	int argBltime, eBulletColor argBlknd, int argHp, int argWait)
{
	data_.pattern = argPattern;
	data_.blknd = argBlknd;
	data_.brknd = argBrknd;
	data_.bltime = argBltime;
	life_ = argHp;
	maxLife_ = life_;
	data_.wait = argWait;
	pos_ = argPos;
	spd_ = argSpd;
	count_ = 0;
	angle_ = 1.f;
	radius_ = graph_->ImageSize().y * 0.75f;
	isLiving_ = false;
}

void SmallEnemy::SetEnemy(EnemyData argEnemyData)
{
	data_ = argEnemyData;
	maxLife_ = life_;
	count_ = 0;
	angle_ = 1.f;
	radius_ = graph_->ImageSize().y * 0.75f;
	isLiving_ = false;
}

void SmallEnemy::CollisionEnter()
{
	if (collider_->PlayerInfo)
	{
		collider_->PlayerInfo->HitDamage();
	}
	if (collider_->ChargeBulletInfo || collider_->EraseRippleInfo)
	{
		life_ -= life_;
	}
	
	Enemy::CollisionEnter();
}

bool SmallEnemy::Update()
{
	Singleton<ObstacleManager>::Get()->Entry(this, IsDelay());
	
	move_(this);
	pos_.x += std::cosf(angle_) * spd_.x;
	pos_.y += std::sinf(angle_) * spd_.y;
	count_++;

	if ((count_ >= data_.bltime) && (count_ % 20 == 0))
	{
		Singleton<EnemyBulletManager>::Get()->CreateAndRegist(this, pos_, 0, 0);
	}
	if (!IsInArea(pos_, PlayArea))
	{
		isLiving_ = false;
	}

	if (life_ <= 0)
	{
		isLiving_ = false;
		Finalize();
	}
	Singleton<CollisionManager2D>::Get()->Entry(this);

	return isLiving_;
}

void SmallEnemy::Draw()
{
	Math::Vector2 Size = graph_->ImageSize();

	DrawRotaGraph(pos_, Size, Size * 0.5f, 1.0f, Math::ToRadian(count_ * 10.f), graph_);
	GameObject2D::Draw();
}

void SmallEnemy::Finalize()
{
	pImpl->Finalize();
}

