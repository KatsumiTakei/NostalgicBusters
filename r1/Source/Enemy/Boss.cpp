/**
 *	@file	Boss.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "Boss.h"
#include "../Result/Result.h"
#include "../Item/Item.h"
#include "../Player/PlayerBullet/ChargeBullet.h"

#include "EnemyBullet/BallBullet.h"
#include "BossState/BossState.h"
#include "../Effect/BossCrashEffect.h"

#include "../Collision/CollisionManager2D.h"
#include "EnemyFactory/EnemyManager.h"

#include "../TextManager/TextManager.h"
#include "../Config.h"

namespace
{
	const int EffectMax = 60;
}

class Boss::Impl
{

public:

	Impl(Boss *argBoss, Player *argPlayer)
		: boss_(argBoss), textManager_(Singleton<TextManager>::Get()), graphManager_(Singleton<GraphManager>::Get()),
		lifeGauge_(nullptr), lifeBer_(nullptr), state_(nullptr), isTalking_(false),player_(argPlayer),
		effectNum_(EffectMax - 1), result_(std::make_unique<Result>())
	{
		for (int i = 0; i < EffectMax; ++i)
		{
			bossCrashEffect_[i] = std::make_unique<BossCrashEffect>();
		}

		image_[eImage::BackEffect] = graphManager_->FindIndexTexture(L"magiceffect");
		image_[eImage::Number] = graphManager_->FindIndexTexture(L"number");

	}
	~Impl() = default;

public:
	void Initialize()
	{
		lifeGauge_ = std::make_unique<CircleGauge>();
		lifeGauge_->property_.MaxValue = static_cast<float>(boss_->maxLife_);
		lifeGauge_->property_.Value = static_cast<float>(boss_->life_);
		lifeGauge_->Radius(50.f);
		lifeGauge_->LineWight(3.f);
		lifeGauge_->GaugeType(eGaugeType::GaugeLine);

		lifeBer_ = std::make_unique<RectGauge>();
		lifeBer_->property_.MaxValue = static_cast<float>(boss_->maxLife_);
		lifeBer_->property_.MaxPos= PlayScreenRight - PlayScreenLeft - 120.f;
		lifeBer_->property_.Value = static_cast<float>(boss_->life_);
		lifeBer_->Color(D2D1::ColorF(1, 0, 0));
		lifeBer_->Wight(5.f);

		effectNum_ = EffectMax - 1;

		isTalking_ = false;

		const int PrevMoveCnt = FlameTime * 3;
		const Math::Vector2 destPos = Math::Vector2(PlayScreenRight * 0.5f, PlayScreenBottom * 0.25f);
		tween_ = std::make_unique<Tween<Math::Vector2>>(boss_->pos_, destPos, static_cast<float>(PrevMoveCnt), Ease::Linear);

		state_ = std::make_unique<BossState>(boss_, player_);
	}

	bool Update()
	{
		if (!textManager_->IsFinished())
		{
			if (!isTalking_)
			{
				isTalking_ = !tween_->Update();
				boss_->life_ = boss_->maxLife_;
			}
			else
			{
				textManager_->Update();
			}
		}
		else
		{
			Singleton<CollisionManager2D>::Get()->Entry(boss_);

			state_->Update();
			boss_->count_++;
		}

		lifeGauge_->property_.Value = static_cast<float>(boss_->life_);
		lifeBer_->property_.Value = static_cast<float>(boss_->life_);
		lifeGauge_->pos_ = boss_->pos_;

		lifeBer_->pos_ = Math::Vector2(PlayScreenLeft + 70.f, PlayScreenTop + 12.f);
		return true;
	}

	void Draw()
	{
		if (!textManager_->IsFinished())
			return;

		DrawBackEffect();
		DrawLifeGauge();

		state_->Draw();

		graphManager_->ConfigTextColor(D2D1::ColorF::White);
		graphManager_->DrawAndWriteText(L"Boss", Math::Vector2(static_cast<float>(PlayScreenLeft + 6.f), static_cast<float>(PlayScreenTop)));
		graphManager_->ConfigTextColor(D2D1::ColorF::Black);

	}

	void Finalize()
	{

		Singleton<TaskManager>::Get()->Add(bossCrashEffect_[effectNum_]->Create(boss_->pos_, effectNum_));
		
		const auto &enemyList = Singleton<EnemyManager>::Get()->EnemyList(player_);
		for (const auto &enemy : enemyList)
		{
			if (enemy.get() != boss_)
			{
				enemy->IsLiving(false);
				//const auto &bulletList = enemy->BulletList();
				//for (const auto &task : bulletList)
				//{
				//	task->IsLiving(false);
				//}
			}

		}

		if (--effectNum_ <= 0)
		{
			boss_->isLiving_ = false;
			Singleton<TaskManager>::Get()->Add(result_->Create());
			//const auto &bulletList = boss_->BulletList();
			//for (const auto &task : bulletList)
			//{
			//	task->IsLiving(false);
			//}
		}
	}

private:
	void DrawBackEffect()
	{// 後ろに出てくるエフェクト
		static float localeAngle = 0.f;
		localeAngle += Math::ToRadian(18.f);
		const float R = static_cast<float>(Math::XorShift() % 10 + 1) * 0.1f;
		const float G = static_cast<float>(Math::XorShift() % 10 + 1) * 0.1f;
		const float B = static_cast<float>(Math::XorShift() % 10 + 1) * 0.1f;
		const Math::Vector4 Color = Math::Vector4(R, G, B, 0.25f);
		DrawRotaGraph(boss_->pos_, Math::Vector2(64.f, 64.f), Math::Vector2(32.f, 32.f), 3.f, localeAngle, image_[eImage::BackEffect], Color);
	}
	void DrawLifeGauge()
	{
		if (boss_->life_ > 0)
		{// 円型のライフゲージ
			if (boss_->maxLife_ * 0.3f < boss_->life_ &&
				boss_->life_ < boss_->maxLife_ * 0.3f * 2.f)
			{
				lifeGauge_->Color(D2D1::ColorF(1, 1, 0, 0.5f));
			}
			else if (boss_->maxLife_ * 0.3f * 2.f < boss_->life_)
			{
				lifeGauge_->Color(D2D1::ColorF(0, 1, 0, 0.5f));
			}
			else
			{
				lifeGauge_->Color(D2D1::ColorF(1, 0, 0, 0.5f));
			}
			lifeGauge_->Draw(Singleton<GraphManager>::Get()->RenderTarget());

			// 棒状のライフゲージ
			lifeBer_->Draw(Singleton<GraphManager>::Get()->RenderTarget());
		}
	}

private:
	enum eImage
	{
		BackEffect,
		Number,
		Max,
	};

private:
	int						effectNum_;
	bool					isTalking_;
	std::unique_ptr<CircleGauge>		lifeGauge_;
	std::unique_ptr<RectGauge>			lifeBer_;
	std::unique_ptr<BossState>			state_;
	std::unique_ptr<Result>			result_;
	std::unique_ptr<BossCrashEffect>	bossCrashEffect_[EffectMax];
	GraphManager			*graphManager_;
	TextManager				*textManager_;
	Boss					*boss_;
	std::shared_ptr<Texture>image_[eImage::Max];
	std::unique_ptr<Tween<Math::Vector2>>	tween_;
public:

	Player							*player_;
};

Boss::Boss(Player *argPlayer)
	:Enemy(), pImpl(std::make_unique<Impl>(this, argPlayer))
{
	isLiving_ = false;
	
	graph_ = Singleton<GraphManager>::Get()->FindIndexTexture(L"boss");
}

Boss::~Boss() = default;

void Boss::SetEnemy(int argPattern, int argBrknd, Math::Vector2 argPos, Math::Vector2 argSpd,
	int argBltime, eBulletColor argBlknd, int argHp, int argWait)
{
	data_.pattern = argPattern;
	data_.blknd = argBlknd;
	data_.brknd = argBrknd;
	data_.bltime = argBltime;
	data_.wait = argWait;
	life_ = argHp;
	maxLife_ = life_;
	pos_.x = argPos.x + 12.f;
	pos_.y = argPos.y;
	spd_.x = 1.f;
	spd_.y = 2.f;
	radius_ = 16.f;
	isLiving_ = false;
	priority_ = ePriority::GameObjectFoword;

	pImpl->Initialize();

}

void Boss::SetEnemy(EnemyData argEnemyData)
{
	data_ = argEnemyData;
	maxLife_ = life_;
	isLiving_ = false;

	spd_.x = 1.f;
	spd_.y = 2.f;
	radius_ = 16.f;
	pos_.x += 12.f;
	priority_ = ePriority::GameObjectFoword;

	pImpl->Initialize();

}
Enemy *Boss::Create()
{
	//	ボスが出現する時、敵と敵の弾を全滅させる（アイテムは落とさない）
	//const auto &enemyList = Singleton<EnemyManager>::Get()->EnemyList(pImpl->player_);
	//for (const auto &enemy : enemyList)
	//{
	//	enemy->IsLiving(false);

	//	const auto &bulletList = enemy->BulletList();
	//	for (const auto &bullet : bulletList)
	//	{
	//		bullet->IsLiving(false);
	//	}
	//}
	Singleton<TextManager>::Get()->IsFinished(false);	//	!<	会話開始
	
	return 	Enemy::Create();
}

void Boss::CollisionEnter()
{
	Enemy::CollisionEnter();
}

bool Boss::Update()
{
	pImpl->Update();
	return isLiving_;
}

void Boss::Draw()
{	
	pImpl->Draw();
	DrawRotaGraph(pos_, Math::Vector2(64, 64), Math::Vector2(32.f, 32.f), 1.f, 0.f, graph_);

	GameObject2D::Draw();
}

void Boss::Finalize()
{
	pImpl->Finalize();
}

