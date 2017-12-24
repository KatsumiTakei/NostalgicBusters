/**
 *	@file	Leika.cpp
 *	@date	2017 / 11 / 02
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "Leika.h"
#include "PlayerOption.h"
#include "PlayerBullet/PlayerWeapon.h"
#include "PlayerBullet/Bullet.h"
#include "PlayerState/PlayerState.h"
#include "PlayerBullet/LeikaChargeBullet.h"

namespace
{
	const float StartSpd = 15.f;
}

class Leika::Impl
{
public:
	Impl(Leika *argLeika, const int argPlayerIndex)
		:leika_(argLeika)
	{
		leika_->chargeBullet_ = std::make_unique<LeikaChargeBullet>();

		leika_->playerWeapon_.resize(MaxBullet);
		for (int lBulletIndex = 0; lBulletIndex < MaxBullet; ++lBulletIndex)
		{
			leika_->playerWeapon_[lBulletIndex] = std::make_unique<Bullet>();
		}
		for (int lShotIndex = 0, lShotMax = OptionMax_ + 1; lShotIndex < lShotMax; lShotIndex++)
		{
			leika_->shotPos_.push_back(ShotPos_[lShotIndex]);
		}
	}
	~Impl() = default;
public:

	void ShotBullet()
	{
		if (++leika_->count_ % 4 != 0)
			return;
		
		Singleton<SoundManager>::Get()->PlaySounds("PlayerShot", eSoundType::Se);

		const int ShotNum_[static_cast<int>((OptionMax_ * 0.5) + 1)] = { 1,3,5,7,9 };
		for (int lShotIndex = 0; lShotIndex < ShotNum_[leika_->ShotIndex()]; lShotIndex++)
		{
			for (const auto &lWeapon : leika_->playerWeapon_)
			{
				if (lWeapon->IsLiving())
					continue;

				int moveType = BulletMoveNomal;

				if (leika_->GetCurrentState() == eState::Low)
				{
					if (lShotIndex % 2 == 1)
					{
						moveType = BulletMoveHoming;
					}
				}
				Singleton<TaskManager>::Get()->Add(lWeapon->Create(leika_, leika_->pos_ + ShotPos_[lShotIndex], moveType));
				break;
			}
		}
	}

	void Initialize()
	{

	}

	void Update()
	{
	}

	void Draw()
	{
	}


public:

private:
	const Math::Vector2 ShotPos_[OptionMax_ + 1] = {
		Math::Vector2(0.f, 15.f), Math::Vector2(-10.f, 20.f), Math::Vector2(10.f, 20.f),
		Math::Vector2(-30.f, 30.f), Math::Vector2(30.f, 30.f), Math::Vector2(-50.f, 10.f),
		Math::Vector2(50.f, 10.f),  Math::Vector2(-20.f, 15.f), Math::Vector2(20.f, 15.f)
	};
	Leika *leika_;
};

Leika::Leika(const int argPlayerIndex)
	:pImpl(std::make_unique<Impl>(this, argPlayerIndex)), Player(argPlayerIndex, StartSpd, L"Player")
{
	option_ = std::make_unique<PlayerOption>(OptionMax_);
	radius_ = graph_->ImageSize().x / (3 * 8);
}

Leika::~Leika() = default;

void Leika::Initialize()
{
	Player::Initialize();
}


void Leika::CollisionEnter()
{
	Player::CollisionEnter();
}


bool Leika::Update()
{
	return Player::Update();
}

void Leika::Draw()
{
	Player::Draw();
}

void Leika::ShotBullet()
{
	pImpl->ShotBullet();
}


