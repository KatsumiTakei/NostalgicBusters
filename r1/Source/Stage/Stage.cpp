/**
 *	@file	Stage.cpp
 *	@date	2017 / 7 / 26
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "Stage.h"

#include "../Player/Player.h"
#include "../Enemy/EnemyFactory/EnemyManager.h"
#include "../Enemy/Boss.h"
#include "../Enemy/SmallEnemy.h"
#include "../Config.h"
#include "../Input/Input.h"


class Stage::Impl
{
public:
	Impl(Stage *argNextStage, Player *argPlayer)
		:player_(argPlayer) , nextStage_(argNextStage), count_((-5) * FlameTime)
	{
	}

	~Impl() = default;
public:
	bool LoadEnemy(const std::wstring argFilename)
	{	
		 bool res = Singleton<EnemyManager>::Get()->LoadEnemy(argFilename, player_);
		 enemyList_ = Singleton<EnemyManager>::Get()->EnemyList(player_);
		 enemyDataList_ = Singleton<EnemyManager>::Get()->EnemyDataList(player_);
		 return res;
	}

	void GenerationEnemy()
	{
		count_++;
		for (int lEnemyIndex = 0, lEnemyMax = enemyDataList_.size() - 1; lEnemyIndex < lEnemyMax; ++lEnemyIndex)
		{
			if (count_ == enemyDataList_[lEnemyIndex].count)
			{
				Singleton<TaskManager>::Get()->Add(enemyList_[lEnemyIndex]->Create());
				if (lEnemyIndex == lEnemyMax - 1)
				{//	仮ループ
					count_ = enemyDataList_[5].count - 1;
					InitializeEnemyList();
				}
			}
		}
	}

	void InitializeEnemyList()
	{
		for (int lEnemyIndex = 0, lEnemyMax = enemyDataList_.size() - 1; lEnemyIndex < lEnemyMax; ++lEnemyIndex)
		{
			enemyList_[lEnemyIndex]->SetEnemy(enemyDataList_[lEnemyIndex].pattern, enemyDataList_[lEnemyIndex].brknd,
				Math::Vector2(enemyDataList_[lEnemyIndex].posX, enemyDataList_[lEnemyIndex].posY), Math::Vector2(enemyDataList_[lEnemyIndex].spdX, enemyDataList_[lEnemyIndex].spdY),
				enemyDataList_[lEnemyIndex].bltime, enemyDataList_[lEnemyIndex].blknd, enemyDataList_[lEnemyIndex].life, enemyDataList_[lEnemyIndex].wait);
		}
	}

	void Finalize()
	{
		isFinished_ = false;
		if (nextStage_)
		{
		}
		else
		{//	TODO	:	エンディングシーンに遷移
		}

	}
private:
	enum class EnemyId : int
	{
		Small,
		Boss,
	};

public:
	int count_;
	std::vector<std::shared_ptr<Enemy>> enemyList_;
	std::vector<EnemyData>	enemyDataList_;
	Player *player_;
	Stage *nextStage_;
	static bool isFinished_;
};

bool Stage::Impl::isFinished_ = false;

Stage::Stage(Player *argPlayer, Stage *argNextStage, std::wstring argEnemyFileName, std::string argSoundFileName, std::string argViewPortName)
	:Task(Singleton<TaskManager>::Get()->IsDelay()), pImpl(std::make_unique<Impl>(argNextStage, argPlayer)), enemyFileName_(argEnemyFileName), camera_(std::make_unique<Camera>()),
	viewPortName_(argViewPortName)
{
	Impl::isFinished_ = false;
	if (!argSoundFileName.empty() && argSoundFileName != Singleton<SoundManager>::Get()->CurrentPlayBgm())
	{
		Singleton<SoundManager>::Get()->PlaySounds(argSoundFileName, eSoundType::Loop);
	}

	drawMode_ = eDrawMode::Solid;
	priority_ = ePriority::BackGround;
	bool res = pImpl->LoadEnemy(enemyFileName_);
	assert(res && "EnemyData Loading Failed");

	Initialize();
}

Stage::~Stage() = default;

void Stage::Initialize()
{
	pImpl->InitializeEnemyList();
}

bool Stage::Update()
{
	camera_->Update();

	ScrollBack();
	pImpl->GenerationEnemy();

#if defined(DEBUG) || defined(_DEBUG)
	if (Singleton<InputManager>::Get()->IsTouched(Input::Debug))
	{
		pImpl->count_ = 2580;
	}
#endif

	return true;
}

void Stage::Finalize()
{
	pImpl->Finalize();
}

std::vector<std::shared_ptr<Enemy>> Stage::EnemyList()const
{
	return pImpl->enemyList_;
}

void Stage::Count(int argCount)
{
	pImpl->count_ = argCount;
}

int Stage::Count()const
{
	return pImpl->count_;
}

bool Stage::IsFinished()
{
	return Stage::Impl::isFinished_;
}

void Stage::IsFinished(bool argIsFinished)
{
	Stage::Impl::isFinished_ = argIsFinished;
}
