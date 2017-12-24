/**
 *	@file	Player.cpp
 *	@date	2017 / 7 / 26
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "Player.h"
#include "Ai/PlayerAi.h"
#include "../Collision/CollisionManager2D.h"
#include "../Stage/Stage.h"

#include "PlayerBullet/ChargeBullet.h"
#include "PlayerBullet/Bullet.h"
#include "PlayerBullet/Jamming.h"
#include "../Item/Item.h"
#include "Graze.h"
#include "PlayerState/PlayerStateManager.h"

#include "PlayerOption.h"

#include "../Effect/PlayerCrashEffect.h"
#include "../ScoreManager/ScoreManager.h"
#include "../TextManager/TextManager.h"
#include "../Config.h"
#include "../Input/Input.h"

namespace
{
	const int MaxChargeLevel = 4;
	const Math::Vector2 StartPos(PlayScreenRight * 0.5f, 300.f);
}

class Player::Impl
{
public:

	Impl(Player *argPlayer, const int argPlayerIndex)
		: state_(std::make_unique<PlayerStateManager>(player_)), chargeShotLevel_(0), maxPowerCnt_(0), isInvisible_(false), isUnrivaled_(false), isOperation_((argPlayerIndex >= 0)),
		player_(argPlayer), graze_(std::make_unique<Graze>(argPlayer)), isBonusGet_(true), cutPos_(Math::Vector2(0.f, 0)), invisibleCnt_(0), shotIndex_(0), chargeLevelUpCnt_(0), victoryPoint_(0)
	{
		playerGraphSize_ = player_->graph_->ImageSize() / SplitGraphNum_;

		if (isOperation_)
		{
			IsPressed = [=](eKey argKeyCode) { return Input::IsPressed(argKeyCode, argPlayerIndex); };
			IsTouched = [=](eKey argKeyCode) { return Input::IsTouched(argKeyCode, argPlayerIndex); };
		}
		else
		{
			ai_ = std::make_unique<PlayerAi>(player_);
		}
	}

	~Impl() = default;

private:

	void Shot()
	{
		if (!Singleton<TextManager>::Get()->IsFinished())
			return;

		if (IsPressed(Input::Shot))
		{
			player_->ShotBullet();
		}

		if (IsPressed(Input::Charge))
		{
			const int ChargeLevelUp = 60;
			if (++chargeLevelUpCnt_ > ChargeLevelUp)
			{
				chargeLevelUpCnt_ = 0;
				chargeShotLevel_++;
			}

			if (chargeShotLevel_ > 0)
			{
				chargeShotLevel_ = 0;
				Singleton<TaskManager>::Get()->Add(player_->chargeBullet_->Create(player_, chargeShotLevel_));
			}
		}

		if (!IsPressed(Input::Charge))
		{
			chargeShotLevel_ = 0;
			chargeLevelUpCnt_ = 0;
		}
	}

	void Move()
	{
		cutPos_.y = 0;

		if (IsPressed(Input::MoveUp))
		{
			player_->pos_.y -= player_->spd_.y;
		}
		if (IsPressed(Input::MoveDown))
		{
			player_->pos_.y += player_->spd_.y;
		}
		if (IsPressed(Input::MoveRight))
		{
			cutPos_.y = playerGraphSize_.y;
			player_->pos_.x += player_->spd_.x;
		}
		if (IsPressed(Input::MoveLeft))
		{
			cutPos_.y = playerGraphSize_.y * 2;
			player_->pos_.x -= player_->spd_.x;
		}

		//	移動制限
		if (player_->pos_.x > PlayArea.right - playerGraphSize_.x * 0.5f) player_->pos_.x -= player_->spd_.x;
		if (player_->pos_.x < PlayArea.left + playerGraphSize_.x * 0.5f) player_->pos_.x += player_->spd_.x;
		if (player_->pos_.y > PlayArea.bottom - playerGraphSize_.y * 0.5f) player_->pos_.y -= player_->spd_.y;
		if (player_->pos_.y < PlayArea.top + playerGraphSize_.y) player_->pos_.y += player_->spd_.y;
	}

	void KeyAction()
	{
		if (!isOperation_)
		{
			ai_->Update();
			return;
		}

		const eState CurrentState = (IsPressed(Input::Slow)) ? eState::Low : eState::Default;
		state_->ChangeState(CurrentState);

		Shot();
		Move();
	}

	void Animation()
	{
		animCount_++;
		if (animCount_ % 15 == 0)
		{
			cutPos_.x = 0.f;
		}
		else if (animCount_ % 10 == 0)
		{
			cutPos_.x = playerGraphSize_.x;
		}
		else if (animCount_ % 5 == 0)
		{
			cutPos_.x = playerGraphSize_.x * 2;
		}
	}

public:
	bool HitDamage()
	{
		if (isInvisible_ || isUnrivaled_)
			return false;

		isInvisible_ = true;
		isBonusGet_ = false;

		--player_->life_;

		if (player_->life_ != 0)
		{
			Singleton<SoundManager>::Get()->PlaySounds("PlayerDamage", eSoundType::Se);
		}
		else
		{
			player_->isLiving_ = false;
			Stage::IsFinished(true);
		}

		return true;

	}

	void CollisionEnter()
	{
	}

	bool Update()
	{
		state_->Update();
		player_->option_->Update(player_->pos_, player_->shotPos_.data());

		shotIndex_ = static_cast<int>(power_ - 1.);

		KeyAction();
		Animation();

		if (isInvisible_)
		{
			const int InvisibleTime = 180;
			if (++invisibleCnt_ >= InvisibleTime)
			{
				isInvisible_ = false;
				invisibleCnt_ = 0;
			}
		}

		const double MaxPower = 5.;
		if (power_ >= MaxPower)
		{
			power_ = MaxPower;
			maxPowerCnt_--;
		}

		graze_->Update();

		return true;
	}

	void Draw()
	{
		if (!player_->isLiving_)
			return;

#if defined(_DEBUG) || defined(DEBUG)
		if (player_->IsDelay() && !isOperation_)
		{
			ai_->Draw();
		}
#endif

		state_->Draw();
		player_->option_->Draw((shotIndex_ - 1) * 2);

		const float Alpha = (isInvisible_) ? 0.5f : 1.f;
		const Math::Vector4 Color = Math::Vector4(1.f, 1.f, 1.f, Alpha);

		DrawRectRotaGraph(player_->pos_, cutPos_, playerGraphSize_, 1.f, 0.f, player_->graph_, Color);

		graze_->Draw();
	}

private:
	Player										*player_;
	std::unique_ptr<PlayerAi>					ai_;
private:
	const Math::Vector2 SplitGraphNum_ = Math::Vector2(3);

public:
	std::unique_ptr<Graze>						graze_;
	int											chargeShotLevel_;
	int											chargeLevelUpCnt_;
	double										power_;
	bool										isBonusGet_;
	int											shotIndex_;
	std::unique_ptr<PlayerStateManager>			state_;

	std::function<bool(eKey)>					IsPressed;
	std::function<bool(eKey)>					IsTouched;
	int											invisibleCnt_;
	int											animCount_;
	int											maxPowerCnt_;
	bool										isInvisible_;
	bool										isUnrivaled_;
	bool										isOperation_;
	Math::Vector2								cutPos_;
	Math::Vector2								playerGraphSize_;
	int											victoryPoint_;
};

Player::Player(const int argPlayerIndex, const float argSpd, std::wstring argImageName)
	:GameObject2D(eShapeType::Circle)
{
	spd_ = Math::Vector2(argSpd) / Math::Vector2(3.f, 4.f);
	count_ = 0;
	graph_ = Singleton<GraphManager>::Get()->FindIndexTexture(argImageName);
	pImpl = std::make_unique<Impl>(this, argPlayerIndex);
	Initialize();
}

Player::~Player() = default;

bool Player::HitDamage()
{
	return pImpl->HitDamage();
}

void Player::CollisionEnter()
{
	pImpl->CollisionEnter();
	collider_->Clear();
}

void Player::Initialize()
{
	life_ = 5;
	maxLife_ = life_;
	pos_ = StartPos;
	pImpl->power_ = 1.;
	pImpl->isBonusGet_ = true;
	isLiving_ = true;
}

bool Player::Update()
{
	pImpl->Update();

	Singleton<CollisionManager2D>::Get()->Entry(this);

	return true;
}

void Player::Draw()
{
	pImpl->Draw();
}

int Player::ShotIndex() const
{
	return pImpl->shotIndex_;
}

eState Player::GetCurrentState() const
{
	return pImpl->state_->GetCurrentState();
}

#pragma region SetterAndGetter

double Player::Power()const
{
	return pImpl->power_;
}
void Player::Power(double argPower)
{
	pImpl->power_ = argPower;
}
bool Player::IsBonusGet() const
{
	return pImpl->isBonusGet_;
}
void Player::IsBonusGet(bool argFlag)
{
	pImpl->isBonusGet_ = argFlag;
}

bool Player::IsOperation()
{
	return pImpl->isOperation_;
}

int Player::VictoryPoint() const
{
	return pImpl->victoryPoint_;
}

#pragma endregion