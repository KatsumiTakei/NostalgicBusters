/**
 *	@file	PlayScene.cpp
 *	@date	2017 / 11 / 21
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "PlayScene.h"
#include "SceneManager.h"

#include "../Player/Leika.h"
#include "../Stage/Stage01.h"
#include "../Pause/Pause.h"
#include "../Replay/ReplayRecorder.h"

#include "../UiDrawer/UiDrawer.h"
#include "../ScoreManager/ScoreManager.h"
#include "../Enemy/EnemyFactory/EnemyManager.h"
#include "../Enemy/EnemyBullet/EnemyBulletManager.h"
#include "../Collision/CollisionManager2D.h"
#include "ContinueButton/RestartButton.h"
#include "ContinueButton/PromptlyReturnTitleButton.h"

#include "../TextManager/TextManager.h"
#include "../Player/Ai/Obstacle.h"

#include "../Config.h"
#include "../Input/Input.h"

class PlayScene::Impl
{
public:
	Impl(PlayScene *argPlayScene)
		:playScene_(argPlayScene), input_(Singleton<InputManager>::Get()), graphManager_(Singleton<GraphManager>::Get()),
		taskManager_(Singleton<TaskManager>::Get())
	{
		Singleton<ObstacleManager>::Get()->Clear();

		// テキストマネージャーの初期化
		Singleton<TextManager>::Get()->Initialize();

#if defined(_DEBUG) | defined(DEBUG) 
		//	スコアの初期化
		Singleton<ScoreManager>::Get()->Score(0);
		Singleton<ScoreManager>::Get()->HiScore(0);
#endif

		pause_ = std::make_unique<Pause>(argPlayScene->sceneManager_);

		auto viewportManager = Singleton<ViewportManager>::Get();
		views_[0] = viewportManager->GetViewport("Left");
		views_[1] = viewportManager->GetViewport("Right");
		views_[2] = viewportManager->GetViewport("Default");

		//	左サイドタスク初期化
		taskManager_->IsDelay(false);
		const int SideLeft = static_cast<int>(eSide::Left);
		player_[SideLeft] = std::make_unique<Leika>(0);
		stage_[SideLeft] = std::make_unique<Stage01>(player_[SideLeft].get(), argPlayScene->deviceResources_, nullptr, "Stage01", "LeftPlayer");
		uiDrawer_[SideLeft] = std::make_unique<UiDrawer>(player_[SideLeft].get());
		//replay_[eSide::Left] = std::make_unique<ReplayRecorder>(player_[eSide::Left].get());
		Singleton<EnemyBulletManager>::Get()->SetBullets(player_[SideLeft].get());

		{// タスクの登録
			taskManager_->Add(stage_[SideLeft].get());
			taskManager_->Add(player_[SideLeft].get());
			taskManager_->Add(uiDrawer_[SideLeft].get());
		}

		//	右サイドタスク初期化
		taskManager_->IsDelay(true);
		const int SideRight = static_cast<int>(eSide::Right);
		player_[SideRight] = std::make_unique<Leika>(-1);
		stage_[SideRight] = std::make_unique<Stage01>(player_[SideRight].get(), argPlayScene->deviceResources_, nullptr, "Stage01", "RightPlayer");
		uiDrawer_[SideRight] = std::make_unique<UiDrawer>(player_[SideRight].get());
		//replay_[eSide::Right] = std::make_unique<ReplayRecorder>(player_[eSide::Right].get());
		Singleton<EnemyBulletManager>::Get()->SetBullets(player_[SideRight].get());

		{
			taskManager_->Add(stage_[SideRight].get());
			taskManager_->Add(player_[SideRight].get());
			taskManager_->Add(uiDrawer_[SideRight].get());
		}

		continueMenu_ = std::make_unique<CMenu>(new RestartButton(player_[SideLeft].get()), new PromptlyReturnTitleButton(playScene_->sceneManager_));

		graph_[static_cast<int>(eImage::Result)] = graphManager_->FindIndexTexture(L"Resultbord");
		graph_[static_cast<int>(eImage::Back)] = graphManager_->FindIndexTexture(L"BackGround");
		graph_[static_cast<int>(eImage::Num)] = graphManager_->FindIndexTexture(L"Number");


		spriteBatch = graphManager_->GetSpriteBatch();
		viewportManager = Singleton<ViewportManager>::Get();
		context = playScene_->deviceResources_->ImContext().Get();

		update_ = [=]() { FightingUpdate(); };
	}
	~Impl() = default;

public:

	void Draw()
	{
#if defined(DEBUG) || defined(_DEBUG)
		//replay_->Draw();
#endif

		DrawGraph(Math::Vector2(0, 0), graph_[static_cast<int>(eImage::Back)]);

		const Math::Vector2 GraphSize = Math::Vector2(graph_[static_cast<int>(eImage::Num)]->ImageSize().x * 0.1f, graph_[static_cast<int>(eImage::Num)]->ImageSize().y);
		DrawNum(Math::Vector2(320.f, 450.f), GraphSize, QueryPerformanceTimer::sFramePerSecond(), graph_[static_cast<int>(eImage::Num)], 2);

		spriteBatch->Draw();

		//	左Side
		viewportManager->RSSetViewports(context, 1, &views_[static_cast<int>(eSide::Left)]);
		taskManager_->Draw();
		spriteBatch->Draw();

		//	右Side
		viewportManager->RSSetViewports(context, 1, &views_[static_cast<int>(eSide::Right)]);
		taskManager_->DelayDraw();
		spriteBatch->Draw();

		//	共通
		viewportManager->RSSetViewports(context, 1, &views_[static_cast<int>(eSide::Default)]);

		//if (Stage::IsFinished())
		//{//	コンてニューメニューの描画
		//	graphManager_->ConfigTextColor(D2D1::ColorF::Red);
		//	graphManager_->DrawAndWriteText(L"決定キーを押してね", Math::Vector2(260.f, PlayScreenBottom - PlayScreenTop * 5.f));
		//	graphManager_->ConfigTextColor(D2D1::ColorF::Black);
		//}

		//Singleton<TextManager>::Get()->Draw();
		if (pause_->IsPause())
		{
			pause_->Draw();
			spriteBatch->Draw();
		}
	}

private:

	void FightingUpdate()
	{
		if (!pause_->Update())
		{/// ポーズしてなかったら
			taskManager_->Update();
			Singleton<ScoreManager>::Get()->Update();
			Singleton<CollisionManager2D>::Get()->Update();

			if (Stage::IsFinished())
			{// プレイヤーが死んだら
				update_ = [=]() { ContinueSelect(); };
			}

#if defined(DEBUG) || defined(_DEBUG)
#if 0	//	!<	リプレイ記録
			if (replay_->IsPlaying())
			{
				replay_->Update();
			}
			if (input_->IsTouched(eKey::Debug))
			{
				replay_->Initialize(stage_->Count(), player_->Power());
			}
#endif	//	!<	リプレイ記録
#endif
		}

	}

	void ContinueSelect()
	{//	コンてニューメニューの更新

		Singleton<EnemyManager>::Get()->DestroyActiveEnemis();
		if (input_->IsTouched(Input::Decide))
		{
			playScene_->sceneManager_->FadeChangeScene(eSceneID::Title);
		}
	}
private:

	enum class eImage
	{
		Result,
		Back,
		Num,


		Max,
	};
private:

	SpriteBatch* spriteBatch;
	ViewportManager* viewportManager;
	ID3D11DeviceContext* context;


private:
	TaskManager							*taskManager_;
	GraphManager						*graphManager_;
	InputManager						*input_;
	std::unique_ptr<Player>				player_[static_cast<int>(eSide::Max)];
	std::unique_ptr<Stage>				stage_[static_cast<int>(eSide::Max)];
	std::unique_ptr<ReplayRecorder>		replay_[static_cast<int>(eSide::Max)];
	std::unique_ptr<Pause>				pause_;
	std::unique_ptr<UiDrawer>			uiDrawer_[static_cast<int>(eSide::Max)];
	std::shared_ptr<Texture>			graph_[static_cast<int>(eImage::Max)];
	PlayScene							*playScene_;
	std::unique_ptr<CMenu>				continueMenu_;
	D3D11_VIEWPORT						views_[static_cast<int>(eSide::Max) + 1];

public:

	std::function<void()>				update_;
};

PlayScene::PlayScene(SceneManager *argSceneManager, const std::shared_ptr<DeviceResources> &argDeviceResources)
	: Scene(argSceneManager, argDeviceResources), pImpl(std::make_unique<Impl>(this))
{
	auto graph = Singleton<GraphManager>::Get()->FindIndexTexture(L"LoadingBack");
	auto context = deviceResources_->ImContext().Get();
	fadeOut_ = std::make_unique<RippleFade>(context, eFadeType::Out, Math::Vector2(640, 480), graph);
	fadeIn_ = std::make_unique<RippleFade>(context, eFadeType::In, Math::Vector2(640, 480), graph);
	fadeCurrent_ = fadeOut_.get();
}

PlayScene::~PlayScene()
{
	Singleton<TaskManager>::Get()->Clear();
	Singleton<CollisionManager2D>::Get()->Clear();
	Singleton<EnemyManager>::Get()->ClearEnemyList();
}

bool PlayScene::Update()
{
	pImpl->update_();
	return true;
}

void PlayScene::Draw()
{
	pImpl->Draw();
}