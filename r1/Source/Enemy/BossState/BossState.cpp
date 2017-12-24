/**
 *	@file	BossState.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "BossState.h"
#include "../Boss.h"
#include "../../Item/Item.h"
#include "../../Player/Player.h"
#include "../EnemyBullet/EnemyBullet.h"

#include "BossLifeTank3State.h" 
#include "BossLifeTank2State.h" 
#include "BossLifeTank1State.h" 

#include "../../ScoreManager/ScoreManager.h"
#include "../../Config.h"

class BossState::Impl
{
public:
	Impl() = default;

	~Impl() = default;
public:

	void Initialize(BossState *argBossState, Player *argPlayer)
	{
		isBonusGet_ = false;
		player_ = argPlayer;
		bossState_ = argBossState;
		finishCnt_ = InitCnt;
		drawCnt_ = 0;
		graph_[eImage::Number] = Singleton<GraphManager>::Get()->FindIndexTexture(L"Number");
		graph_[eImage::Call] = Singleton<GraphManager>::Get()->FindIndexTexture(L"UtilityCall");
		boss_ = bossState_->boss_;
		currentState_ = eState::Life3;
		stateList_[eState::Life1] = std::make_unique<BossLifeTank1State>(boss_);
		stateList_[eState::Life2] = std::make_unique<BossLifeTank2State>(boss_);
		stateList_[eState::Life3] = std::make_unique<BossLifeTank3State>(boss_);
		stateList_[currentState_]->Initialize(player_);
	}

	bool Update()
	{
		(--finishCnt_ < 0 || boss_->Life() < 0) ? Finalize() : stateList_[currentState_]->Update();

		return true;
	}

	void Draw()
	{
		if (drawCnt_ > 0)
		{
			const float CallGraphSize = 75.f;

			(isBonusGet_) ? DrawRectGraph(Math::Vector2(PlayScreenRight * 0.2f, PlayScreenTop * 2.f), Math::Vector2(0.f, CallGraphSize), Math::Vector2(300.f, CallGraphSize), graph_[eImage::Call]) :
				DrawRectGraph(Math::Vector2(PlayScreenRight * 0.2f, PlayScreenTop * 2.f), Math::Vector2(0.f, CallGraphSize * 2.f), Math::Vector2(300.f, CallGraphSize), graph_[eImage::Call]);
		}
		DrawNum(Math::Vector2(PlayScreenRight - 30.f, static_cast<float>(PlayScreenTop)),
			Math::Vector2(12.f, 24.f), static_cast<float>(finishCnt_) / FlameTime, graph_[eImage::Number], 2, Math::Vector4(1.f, 1.f, 1.f, 1.f));
		DrawNum(Math::Vector2(PlayScreenLeft + 52.f, static_cast<float>(PlayScreenTop)),
			Math::Vector2(12.f, 24.f), eState::StateMax - static_cast<float>(currentState_), graph_[eImage::Number], 1, Math::Vector4(1.f, 1.f, 1.f, 1.f));

	}

	void Finalize()
	{
		if (drawCnt_ == 0)
		{
			drawCnt_ = 180;
			const Math::Vector2 Pos = boss_->Pos();
			//for (int i = 0; i < ItemMax; ++i)
			//{// アイテムの生成
			//	Math::Vector2 itemPos = Math::Vector2(Pos.x - 50.f + static_cast<float>(Math::XorShift() % (100)),
			//		Pos.y - 50.f + static_cast<float>(Math::XorShift() % (100)));

			//	Singleton<TaskManager>::Get()->Add(boss_->ItemList()[i]->Create(itemPos));
			//}

			//const Math::Vector2 ItemPos = Math::Vector2(Pos.x - 50.f + static_cast<float>(Math::XorShift() % (100)),
			//	Pos.y - 50.f + static_cast<float>(Math::XorShift() % (100)));

			//Singleton<TaskManager>::Get()->Add(boss_->DropItem()->Create(ItemPos));

			//const auto &bulletList = boss_->BulletList();
			//for (const auto &bullet : bulletList)
			//{
			//	bullet->IsLiving(false);
			//}

			isBonusGet_ = player_->IsBonusGet();
			if (player_->IsBonusGet())
			{
				Singleton<ScoreManager>::Get()->Score(Singleton<ScoreManager>::Get()->Score() + 10000);
			}
		}


		if (currentState_ + 1 < eState::StateMax)
		{
			if (--drawCnt_ <= 0)
			{
				currentState_ = static_cast<eState>(currentState_ + 1);
				stateList_[currentState_]->Initialize(player_);
				finishCnt_ = InitCnt;
			}
		}
		else
		{
			drawCnt_--;
			boss_->Finalize();
		}
	}


private:
	enum eImage
	{
		Number,
		Call,

		Max,
	};

	enum eState
	{
		Life3,
		Life2,
		Life1,

		StateMax,
	};

private:
	std::unique_ptr<CBossLifeState> stateList_[eState::StateMax];
	eState currentState_;
	BossState *bossState_;
	Boss *boss_;
	Player *player_;
	std::shared_ptr<Texture> graph_[eImage::Max];
	int finishCnt_;
	int drawCnt_;
	bool isBonusGet_;
	const int InitCnt = 60 * FlameTime;
};

BossState::BossState(Boss *argBoss, Player *argPlayer)
: boss_(argBoss), pImpl(std::make_unique<Impl>())
{
	pImpl->Initialize(this, argPlayer);
}

BossState::~BossState() = default;

bool BossState::Update()
{
	return pImpl->Update();
}

void BossState::Draw()
{
	pImpl->Draw();
}

void BossState::Finalize()
{
	pImpl->Finalize();
};
