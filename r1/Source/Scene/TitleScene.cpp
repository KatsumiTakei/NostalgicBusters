/**
 *	@file	TitleScene.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "TitleScene.h"
#include "SceneManager.h"

#include "../Config.h"
#include "../Replay/ReplayViewer.h"
#include "../Collision/CollisionManager2D.h"
#include "TitleButton/QuitButton.h"
#include "TitleButton/StoryButton.h"
#include "../Input/Input.h"

class TitleScene::Impl
{

public:
	Impl(TitleScene *argTitleScene)
		: titleScene_(argTitleScene),input_(Singleton<InputManager>::Get())
		/*,
		replay_(std::make_unique<ReplayViewer>(argTitleScene->deviceResources_))*/
	{
		CReplay::DataPath("Resources/Data/");
		Singleton<SoundManager>::Get()->PlaySounds("Title", eSoundType::Loop);
		titleMenu_ = std::make_unique<CMenu>(new StoryButton(titleScene_->sceneManager_), new QuitButton());
		
		graph_[Image::Back] = Singleton<GraphManager>::Get()->FindIndexTexture(L"Titleback");
		graph_[Image::TitleName] = Singleton<GraphManager>::Get()->FindIndexTexture(L"Titlename");
		graph_[Image::Leika] = Singleton<GraphManager>::Get()->FindIndexTexture(L"Leika");

	}

	~Impl() = default;
	
private:
	void UpdateReplay(int *argTime)
	{// 一定時間たったらリプレイ開始

		replay_->Update();
		if (input_->IsTouched(Input::Decide))
		{// ショットボタンを押してリプレイを強制停止
			replay_->Finalize();
		}
		if (!replay_->IsPlaying())
		{// リプレイ終了時の初期化
			*argTime = 0;
			replay_->Initialize();
		}
	}

	void UpdateTitleScene(int *argTime) 
	{
		if (!titleMenu_->Enter())
		{
			if (input_->IsTouched(Input::MoveDown))
			{
				Singleton<SoundManager>::Get()->PlaySounds("CursolMove", eSoundType::Se);
				*argTime = 0;
				titleMenu_->Next();
			}
			else if (input_->IsTouched(Input::MoveUp))
			{
				Singleton<SoundManager>::Get()->PlaySounds("CursolMove", eSoundType::Se);
				*argTime = 0;
				titleMenu_->Prev();
			}
		}
		else
		{
			*argTime = 0;
		}
	}

	void DrawTitleScene()
	{// タイトル画面操作時

		auto rect = Window::Get()->WindowRect();
		Math::Vector2 pos = Math::Vector2(static_cast<float>(rect.right), static_cast<float>(rect.bottom));

		DrawGraph(Math::Vector2(0, 0), graph_[Image::Back]);
		DrawGraph(Math::Vector2(100, 50), graph_[Image::TitleName]);
		DrawGraph(Math::Vector2(pos.x - pos.x * 0.5f, pos.y * 0.3f), graph_[Image::Leika]);

		titleMenu_->Draw();
		Singleton<ViewportManager>::Get()->RSSetViewports(titleScene_->deviceResources_->ImContext().Get(), 1, &Singleton<ViewportManager>::Get()->GetViewport("Default"));

		Singleton<GraphManager>::Get()->GetSpriteBatch()->Draw();

	}

public:
	bool Update()
	{
		const int DemoStartTime = FlameTime * 10;
		static int time = 0;
		//(++time > DemoStartTime) ? UpdateReplay(&time) : 
		UpdateTitleScene(&time);
		return true;
	}

	void Draw()
	{
		//(replay_->IsPlaying()) ? replay_->Draw() : 
		DrawTitleScene();
	}
private:
	enum Image
	{
		Back,
		TitleName,
		Leika,

		Max,
	};

private:
	TitleScene						*titleScene_;
	std::unique_ptr<CMenu>			titleMenu_;
	std::unique_ptr<ReplayViewer>	replay_;
	std::shared_ptr<Texture>		graph_[Image::Max];
	InputManager					*input_;

};

TitleScene::TitleScene(SceneManager* argSceneManager, const std::shared_ptr<DeviceResources> &argDeviceResources)
	: Scene(argSceneManager, argDeviceResources), pImpl(std::make_unique<Impl>(this))
{
	auto graph = Singleton<GraphManager>::Get()->FindIndexTexture(L"LoadingBack");
	auto context = deviceResources_->ImContext().Get();
	fadeOut_ = std::make_unique<SlideFade>(context, eFadeType::Out, Math::Vector2(640, 480), graph);
	fadeIn_ = std::make_unique<RippleFade>(context, eFadeType::In, Math::Vector2(640, 480), graph);
	fadeCurrent_ = fadeOut_.get();
}
TitleScene::~TitleScene() = default;

bool TitleScene::Update()
{
	return pImpl->Update();
}

void TitleScene::Draw()
{
	pImpl->Draw();
}