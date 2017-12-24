/**
 *	@file	PlayerAi.cpp
 *	@date	2017 / 11 / 10
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */

#include "PlayerAi.h"
#include "Obstacle.h"
#include "RiskGrid.h"
#include "RiskMap.h"
#include "../Player.h"
#include "../../Enemy/Enemy.h"
#include "../../Enemy/EnemyFactory/EnemyManager.h"
#include "../../Enemy/EnemyBullet/EnemyBulletManager.h"

class PlayerAi::Impl
{
public:
	struct RiskDate
	{
		int RiskPoint;
		Math::Vector2 Dirction;
		static Math::Vector2 *PlayerPos;
		static Math::Vector2 *PlayerSpd;

		Math::Vector2 NextPos()const
		{
			return (*PlayerPos) + (*PlayerSpd) * Dirction;
		}
	};

public:
	Impl(Player *argPlayer)
		: riskMap_(new RiskMap()), player_(argPlayer), isHitting_(false),
		obstacleManager(Singleton<ObstacleManager>::Get())
	{
		nextEnemisPos_.resize(1000);
		nextEnemisRadius_.resize(1000);

		for (int lIndex = 0; lIndex < DataMax; lIndex++)
		{
			data_[lIndex] = new RiskDate();
		}
		data_[0]->Dirction = Math::Vector2(0, 0);
		data_[1]->Dirction = Math::Vector2(0, 1);
		data_[2]->Dirction = Math::Vector2(1, 0);
		data_[3]->Dirction = Math::Vector2(1, 1);
		
		data_[4]->Dirction = Math::Vector2(0, -1);
		data_[5]->Dirction = Math::Vector2(-1, 0);
		data_[6]->Dirction = Math::Vector2(-1, -1);
		
		data_[7]->Dirction = Math::Vector2(-1, 1);
		data_[8]->Dirction = Math::Vector2(1, -1);

		RiskDate::PlayerPos = player_->PosRef();
		RiskDate::PlayerSpd = player_->SpdRef();
	}

	~Impl()
	{
		SafeDelete(riskMap_);
		for (int lIndex = 0; lIndex < DataMax; lIndex++)
		{
			SafeDelete(data_[lIndex]);
		}
	}

private:
	/**
	 *	@fn				CalcHitting
	 *	@brief			nフレーム先の座標の当たり判定でリスクを計算する
	 *	@param[in]		argRisks			!<	リスクデータ
	 *	@param[in]		argEnemyCollision	!<	敵の当たり判定
	 */
	void CalcHitting(RiskDate **argRisks, CircleShape argEnemyCollision, float argRadius, int argFrame)
	{
		for (int lRiskIndex = 0; lRiskIndex < DataMax; ++lRiskIndex)
		{
			const CircleShape PlayerCollision(&argRadius, argRisks[lRiskIndex]->NextPos());
			if (CircleToCircle(PlayerCollision, argEnemyCollision))
			{
				argRisks[lRiskIndex]->RiskPoint += 100 * argFrame;
			}
		}
	}
	/**
	 *	@fn			ChooseSafeRisk
	 *	@brief		安全な危険度データを選択
	 *	@return		安全な危険度データ
	 */
	RiskDate *ChooseSafeRisk()
	{
		int minRisk = 999999;
		int min = -1;

		for (int lIndex = 0; lIndex < DataMax; lIndex++)
		{
			if (data_[lIndex]->RiskPoint < minRisk)
			{
				minRisk = data_[lIndex]->RiskPoint;
				min = lIndex;
			}
		}

		return data_[min];
	}

	/**
	 *	@fn			CalcRiskMap
	 *	@brief		危険度データの計算
	 */
	void CalcRiskMap()
	{
		const Math::Vector2 PlayerPos = player_->Pos();
		playerGrid_ = riskMap_->CurrentGrid(PlayerPos);

		auto obstacles = obstacleManager->Obstacles(player_->IsDelay());
		for (const auto &lObstacle : obstacles)
		{
			float nextRadius = lObstacle->Radius() * 2;

			const float Angle = lObstacle->Angle();
			const Math::Vector2 Spd = lObstacle->Spd();
			Math::Vector2 nextPos = lObstacle->Pos();
			
			for (int lFollowingFlameIndex = 1; 0 < lFollowingFlameIndex; lFollowingFlameIndex--)
			{//	角度の変化に非対応
				nextPos.x += std::cosf(Angle) * Spd.x * lFollowingFlameIndex;
				nextPos.y += std::sinf(Angle) * Spd.y * lFollowingFlameIndex;

				RiskGrid *obstacleGrid = riskMap_->InObstacle(nextPos);
				if (!obstacleGrid)
					continue;

				if (riskMap_->IsInAroundOrSame(playerGrid_, obstacleGrid))
				{
					nextEnemisPos_.push_back(nextPos);
					nextEnemisRadius_.push_back(nextRadius);
					ChackHitCollision(PlayerPos, nextPos, nextRadius, lFollowingFlameIndex);
				}
			}

		}
	}
	/**
	 *	@fn				ChackHitCollision
	 *	@brief			
	 */
	void ChackHitCollision(Math::Vector2 argPlayerPos, Math::Vector2 argEnemyPos, float argEnmyRadius, int argFrame)
	{
		float radius = player_->Radius();
		const CircleShape PlayerCollision(&radius, argPlayerPos);
		const CircleShape EnemyCollision(&argEnmyRadius, argEnemyPos);
		if (CircleToCircle(PlayerCollision, EnemyCollision))
		{
			isHitting_ = true;
			CalcHitting(data_, EnemyCollision, radius, argFrame);
		}
	}
	/**
	 *	@fn				CalcCandidates
	 *	@brief			移動候補の計算
	 */
	void CalcCandidates()
	{
		for (int lIndexPosX = 0; lIndexPosX < MovePattern; lIndexPosX++)
		{
			for (int lIndexPosY = 0; lIndexPosY < MovePattern; lIndexPosY++)
			{
				const int DataIndex = lIndexPosX * MovePattern + lIndexPosY;

				RiskGrid* nextGrid = riskMap_->CurrentGrid(data_[DataIndex]->NextPos());
				data_[DataIndex]->RiskPoint += nextGrid->Risk();
			}
		}
	}

	/**
	 *	@fn				MoveSafeGrid
	 *	@brief			安全なグリッドに移動
	 *	@return			目標座標
	 */
	Math::Vector2 MoveSafeGrid()
	{
		Math::Vector2 pos = player_->Pos();
		const RiskGrid* SafeGrid = riskMap_->SearchSafeGrid(pos);
		
		if (isHitting_)
		{//	移動先で障害物とぶつかるなら修正

		 //	移動候補を算出
			CalcCandidates();
			pos = CheckHitNextPos(pos);
		}
		else
		{
			const Math::Vector2 Index = playerGrid_->Index() - SafeGrid->Index();
			pos -= (player_->Spd() * Index);

			//	移動制限
			pos.x = Math::Clamp(pos.x, static_cast<float>(Area.left), static_cast<float>(Area.right));
			pos.y = Math::Clamp(pos.y, static_cast<float>(Area.top), static_cast<float>(Area.bottom));
		}

		return pos;
	}
	/**
	 *	@fn				CheckHitNextPos
	 *	@brief			次の座標で当たり判定する
	 *	@param[in]		argTarPos	!<	目標座標
	 *	@return			計算した目標座標
	 */
	Math::Vector2 CheckHitNextPos(Math::Vector2 argTarPos)
	{
		RiskDate* risk = ChooseSafeRisk();
		assert(risk && "not found risk data...");
		argTarPos = risk->NextPos();
		debugAiPlayerPos_ = argTarPos;
		return argTarPos;
	}

	void Move()
	{
		//	比較的安全なグリッドへ移動
		Math::Vector2 tarPos = MoveSafeGrid();
		player_->Pos(tarPos);
	}

	void Attack()
	{
		player_->ShotBullet();
	}

public:
	void Update()
	{
		Singleton<EnemyBulletManager>::Get()->CanShoot(false);
		for (int lIndex = 0; lIndex < DataMax; lIndex++)
		{
			data_[lIndex]->RiskPoint = 0;
		}
		nextEnemisPos_.clear();
		nextEnemisRadius_.clear();
		isHitting_ = false;

		riskMap_->RefreshRiskPoint();

		//	マップの危険度の計算
		CalcRiskMap();

		Move();
		Attack();

		Singleton<ObstacleManager>::Get()->Clear(player_->IsDelay());
		Singleton<EnemyBulletManager>::Get()->CanShoot(true);
	}

	void Draw()
	{
		riskMap_->Draw();

		for (auto lPos : nextEnemisPos_)
		{
			auto test = Singleton<GraphManager>::Get()->FindIndexTexture(L"AiTest1");
			DrawRectRotaGraph(lPos, Math::Vector2(0, 0), test->ImageSize(), 1.0, 1.f, test);
		}

		if (isHitting_)
		{
			auto test = Singleton<GraphManager>::Get()->FindIndexTexture(L"AiTest2");
			DrawRectRotaGraph(debugAiPlayerPos_, Math::Vector2(0, 0), test->ImageSize(), 1.0, 1.f, test);
		}
	}

private:
	static const int DataMax = MovePattern * MovePattern;
	Player *player_;
	ObstacleManager* obstacleManager;
public:
	//	!<	@attention	unique_ptrはoperatorが重いので生ポインタ
	RiskMap						*riskMap_;
	RiskDate					*data_[DataMax];

	std::vector<Math::Vector2>	nextEnemisPos_;
	std::vector<float>			nextEnemisRadius_;

	bool isHitting_;
	RiskGrid					*playerGrid_;
	Math::Vector2				debugAiPlayerPos_;

};

Math::Vector2 * PlayerAi::Impl::RiskDate::PlayerPos = nullptr;
Math::Vector2 * PlayerAi::Impl::RiskDate::PlayerSpd = nullptr;


PlayerAi::PlayerAi(Player *argPlayer)
	:pImpl(std::make_unique<Impl>(argPlayer))
{
}

PlayerAi::~PlayerAi() = default;

void PlayerAi::Update()
{
	pImpl->Update();
}

void PlayerAi::Draw()
{
	pImpl->Draw();
}
