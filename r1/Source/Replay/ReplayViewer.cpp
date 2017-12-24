/**
 *	@file	ReplayViewer.cpp
 *	@date	2017 / 7 / 26
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "ReplayViewer.h"
#include "../Collision/CollisionManager2D.h"
#include "../ScoreManager/ScoreManager.h"
#include "../Enemy/EnemyFactory/EnemyManager.h"

#include "../Player/Player.h"
#include "../UiDrawer/UiDrawer.h"
#include "../Stage/Stage01.h"

#include "../Config.h"

class ReplayViewer::Impl
{

public:
	Impl(const std::shared_ptr<DeviceResources> &argDeviceResources)
		: rePlayFrame_(0), graph_(Singleton<GraphManager>::Get()->FindIndexTexture(L"democall")),
		/*player_(std::make_unique<Player>(-1, 15.f)), */playerShotCnt_(0), demoGraphAlpha_(0.f), demoGraphColor_(Math::Vector4(1.f, 1.f, 1.f, 0.f))
	{
		stage_ = std::make_unique<Stage01>(player_.get(), argDeviceResources, nullptr, "", "LeftPlayer");
		uiDrawer_ = std::make_unique<UiDrawer>(player_.get());

	}
	~Impl() = default;

public:
	void Update(ReplayViewer *argReplay)
	{
		if (argReplay->recData_[rePlayFrame_].IsShot)
		{
			player_->ShotBullet();
		}
	
		player_->Pos(Math::Vector2(argReplay->recData_[rePlayFrame_].PosX, argReplay->recData_[rePlayFrame_].PosY));
		player_->Update();
		Singleton<TaskManager>::Get()->Update();

		Singleton<ScoreManager>::Get()->Update();
		Singleton<CollisionManager2D>::Get()->Update();

		rePlayFrame_++;

		if (rePlayFrame_ == argReplay->recFrame_)
		{
			argReplay->Finalize();
		}
	}

	void Draw()
	{
		player_->Draw();

#if defined(DEBUG) || defined(_DEBUG)
		Singleton<GraphManager>::Get()->DrawAndWriteText(L"動作を再生中です", Math::Vector2());
#endif

		// 画面の中央にリプレイ確認文字をだす
		demoGraphColor_.w = sinf(++demoGraphAlpha_ * Math::PI2);
		DrawRectGraph(Math::Vector2(PlayScreenRight * 0.5f, PlayScreenBottom * 0.5f), Math::Vector2(0, 0), Math::Vector2(120, 40), graph_, demoGraphColor_);

		Singleton<TaskManager>::Get()->Draw();
		Singleton<GraphManager>::Get()->GetSpriteBatch()->Draw();
	}

	static void CanUseArchive(bool argCanUseArchive)
	{
		canUseArchive_ = argCanUseArchive;
	}


public:
	int								playerShotCnt_;
	int								rePlayFrame_;
	float							demoGraphAlpha_;
	std::shared_ptr<Texture>		graph_;
	std::unique_ptr<Player>			player_;
	std::unique_ptr<Stage>			stage_;
	std::unique_ptr<UiDrawer>		uiDrawer_;
	Math::Vector4					demoGraphColor_;
	static bool canUseArchive_;

};

bool ReplayViewer::Impl::canUseArchive_ = false;

ReplayViewer::ReplayViewer(const std::shared_ptr<DeviceResources> &argDeviceResources)
	:pImpl(std::make_unique<Impl>(argDeviceResources))
{
	Initialize();
}

ReplayViewer::~ReplayViewer()
{
	Singleton<TaskManager>::Get()->Clear();
	Singleton<CollisionManager2D>::Get()->Clear();
	Singleton<EnemyManager>::Get()->ClearEnemyList();
	free(recData_);
}

void ReplayViewer::Initialize()
{
	//	取り敢えず今はデモのみ再生
	const std::string Path = dataPath_ + "DemoReplay.dat";
	
	if (pImpl->canUseArchive_)
	{
		const char *Data = Singleton<Archive>::Get()->LoadText(Path.c_str());

		unsigned int readPos = 0;
		std::memcpy(&recFrame_, &Data[readPos], sizeof(int));
		readPos += sizeof(int);

		recData_ = static_cast<lpRecData>(malloc(sizeof(RecData) * recFrame_));
		assert(recData_ && "ReplayData Loading Failed");

		std::memcpy(recData_, &Data[readPos], sizeof(RecData) * recFrame_);
		readPos += sizeof(RecData) * recFrame_;

	}
	else
	{
		//	HACK:		技術アピール用、就活が終わったらfstreamに変える
		//	@section	ファイルの読み込み
		FILE *fp = nullptr;
		errno_t err = fopen_s(&fp, Path.c_str(), "rb");
		assert(fp && "File Reading Fialed");
		fread(&recFrame_, sizeof(int), 1, fp);

		//	HACK:		技術アピール用、就活が終わったらvectorに変える
		//	@section	mallocによる動的確保
		recData_ = static_cast<lpRecData>(malloc(sizeof(RecData) * recFrame_));
		assert(recData_ && "ReplayData Loading Failed");

		fread_s(recData_, sizeof(RecData) * recFrame_, sizeof(RecData) * recFrame_, 1, fp);
		fclose(fp);
	
	}
	
	pImpl->rePlayFrame_ = 0;
	pImpl->player_->Initialize();
	pImpl->player_->Power(recData_->Power);
	pImpl->stage_->Count(recData_->Count);
	pImpl->uiDrawer_->Count(recData_->Count);

	Singleton<TaskManager>::Get()->Add(pImpl->stage_.get());
	Singleton<TaskManager>::Get()->Add(pImpl->uiDrawer_.get());
	Singleton<ScoreManager>::Get()->Score(recData_->Score);
	Singleton<ScoreManager>::Get()->HiScore(recData_->HiScore);
}

void ReplayViewer::Update()
{
	isPlaying_ = true;
	pImpl->Update(this);
}

void ReplayViewer::Draw()
{
	pImpl->Draw();
}

void ReplayViewer::Finalize()
{
	isPlaying_ = false;

	Singleton<TaskManager>::Get()->Clear();
	Singleton<CollisionManager2D>::Get()->Clear();
	pImpl->stage_->Initialize();
	free(recData_);
}

void ReplayViewer::CanUseArchive(bool argCanUseArchive)
{
	Impl::CanUseArchive(argCanUseArchive);
}
