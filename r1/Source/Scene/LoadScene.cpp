/**
*	@file	LoadScene.cpp
*	@date	2017 / 12 / 25
*	@author	Katsumi Takei
*	Copyright (c) Kastumi Takei. All rights reserved.
*/

#include "LoadScene.h"
#include "SceneManager.h"

#include "../SolidObject/SolidBody.h"
#include "../Fbx/FbxModelManager.h"
#include "../Input/Input.h"
#include "../Replay/ReplayViewer.h"
#include "../ScoreManager/ScoreManager.h"
#include "../Enemy/EnemyFactory/EnemyManager.h"
#include "../Enemy/EnemyMovePattern.h"
#include "../Enemy/EnemyBullet/EnemyBulletMovePattern.h"
#include <LuaBind.h>

#include "../Config.h"

#include <thread>
#include <process.h>	//	!<	thread用
#include <exception>	//	!<	例外用

class LoadScene::Impl
{
public:
	Impl(LoadScene* argLoadScene)
		:scene_(argLoadScene), graphManager_(Singleton<GraphManager>::Get()), isLoading_(false)
	{
#ifdef _USE_ARCHIVE_
		canUseArchive_ = true;
#else
		canUseArchive_ = false;
#endif

		try
		{
			InitViewport();

			CShader::CanUseArchive(canUseArchive_);
			CShader::CsoPath("Resources/Shader/Cso/");

			graphManager_->CanUseArchive(canUseArchive_);
			graphManager_->Graphic2D()->ImagePath(L"Resources/Images/");

			//	ローディング画面のテクスチャ読み込み
			graphManager_->AddAndLoadImage(L"NowLoading")->Png();
			graphManager_->AddAndLoadImage(L"LoadingBack")->Png();
			graph_[eImage::LoadingStl] = graphManager_->FindIndexTexture(L"NowLoading");
			graph_[eImage::LoadingBack] = graphManager_->FindIndexTexture(L"LoadingBack");

			auto fbxManager = Singleton<FbxModelManager>::Get();
			fbxManager->Initialie("Resources/Models/", FbxAxisSystem::DirectX);
			fbxManager->CanUseArchive(canUseArchive_);
			//	ローディング画面のモデル読み込み
			fbxManager->AddModel(scene_->deviceResources_->ImContext().Get(), "Logo");

			/**
			*	@fn			_beginthreadex
			*	@param[in]	Security		!<	SECURITY_ATTRIBUTES 構造体へのポインタ またはNULL
			*	@param[in]	StackSize		!<	スタックサイズ。0でもよい
			*	@param[in]	StartAddress	!<	スレッド関数のアドレス
			*	@param[in]	ArgList			!<	スレッド関数に渡す引数、またはNULL
			*	@param[in]	InitFlag		!<	0(すぐ実行) またはCREATE_SUSPENDED(一時停止)
			*	@param[in]	ThrdAddr		!<	スレッド識別子。NULLでも可。
			*	@attention	_beginthreadと違いエラーを返さなければハンドルが有効
			*	@note		https://msdn.microsoft.com/ja-jp/library/kdzttdcb.aspx
			*/
			HANDLE hInitializeThread = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, BeginInitialize, this, NULL, NULL));
			HANDLE hDrawThread = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, BeginDraw, this, NULL, NULL));

			assert(hInitializeThread && "Initialize Failed");
			assert(hDrawThread && "Initialize Failed");

			/**
			*	@fn			WaitForMultipleObjectsEx
			*	@brief		一定の状態になるまで制御を止める
			*	@param[in]	nCount			!<	配列内のオブジェクトハンドルの数
			*	@param[in]	lpHandles		!<	オブジェクトハンドルからなる配列
			*	@param[in]	fWaitAll		!<	待機オプション
			*	@param[in]	dwMilliseconds	!<	タイムアウト時間
			*	@param[in]	bAlertable		!<	アラート可能な待機のオプション
			*	@return		成功すると、関数が制御を返した原因が返ります、失敗したらWAIT_FAILED。
			*	@note		https://msdn.microsoft.com/ja-jp/library/cc429425.aspx
			*/
			const HANDLE Handles[] = { hInitializeThread, hDrawThread };
			const unsigned long Log = WaitForMultipleObjectsEx(2, Handles, true, INFINITE, false);

			assert(Log != WAIT_FAILED && "Loading error...");

			CloseHandle(hInitializeThread);
			CloseHandle(hDrawThread);
			hInitializeThread = nullptr;
			hDrawThread = nullptr;
		}
		catch (wchar_t *argException)
		{
			OutputDebugString(argException);
			exit(EXIT_FAILURE);
		}
	}

	~Impl() = default;
private:
	static unsigned int CALLBACK BeginInitialize(void *arg)
	{
		reinterpret_cast<Impl*>(arg)->Update();
		return 0;
	}
	static unsigned int CALLBACK BeginDraw(void *arg)
	{
		reinterpret_cast<Impl*>(arg)->Draw();
		return 0;
	}

	void InitGraphic()
	{
		//	テクスチャの初期化
		graphManager_->AddAndLoadImage(L"Enemies")->Png();
		graphManager_->AddAndLoadImage(L"Bullets")->Png();
		graphManager_->AddAndLoadImage(L"BackGround")->Png();
		graphManager_->AddAndLoadImage(L"EnemyCrashEffect")->Png();
		graphManager_->AddAndLoadImage(L"Democall")->Png();
		graphManager_->AddAndLoadImage(L"Item")->Png();
		graphManager_->AddAndLoadImage(L"Magiceffect")->Png();
		graphManager_->AddAndLoadImage(L"Number")->Png();
		graphManager_->AddAndLoadImage(L"Player")->Png();
		graphManager_->AddAndLoadImage(L"PlayerWeapon")->Png();
		graphManager_->AddAndLoadImage(L"Pausemenu")->Png();
		graphManager_->AddAndLoadImage(L"ResultMessege")->Png();
		graphManager_->AddAndLoadImage(L"Result")->Png();
		graphManager_->AddAndLoadImage(L"Keepout")->Png();
		graphManager_->AddAndLoadImage(L"Ready")->Png();
		graphManager_->AddAndLoadImage(L"Enemies")->Png();
		graphManager_->AddAndLoadImage(L"StageNumList")->Png();
		graphManager_->AddAndLoadImage(L"Titleback")->Png();
		graphManager_->AddAndLoadImage(L"Titletext")->Png();
		graphManager_->AddAndLoadImage(L"Titlename")->Png();
		graphManager_->AddAndLoadImage(L"Ui")->Png();
		graphManager_->AddAndLoadImage(L"UtilityCall")->Png();
		graphManager_->AddAndLoadImage(L"Vital")->Png();
		graphManager_->AddAndLoadImage(L"Leika")->Png();
		graphManager_->AddAndLoadImage(L"LeikaDetail")->Png();
		graphManager_->AddAndLoadImage(L"Resultbord")->Png();
		graphManager_->AddAndLoadImage(L"StageCall")->Png();
		graphManager_->AddAndLoadImage(L"Option")->Png();
		graphManager_->AddAndLoadImage(L"AiTest1")->Png();
		graphManager_->AddAndLoadImage(L"AiTest2")->Png();

		//	モデルの初期化
		auto immediate = scene_->deviceResources_->ImContext().Get();
		auto fbxManager = Singleton<FbxModelManager>::Get();
		fbxManager->AddModel(immediate, "Cloud");
		fbxManager->AddModel(immediate, "Stage01");
	}

	void InitSound()
	{
		SoundManager *soundManager = Singleton<SoundManager>::Get();
		soundManager->CanUseArchive(canUseArchive_);
		soundManager->SoundPath("Resources/Sounds/");
		soundManager->AddAndLoadSound("Bomb");
		soundManager->AddAndLoadSound("BombUse");
		soundManager->AddAndLoadSound("BossCrash");
		soundManager->AddAndLoadSound("CursolMove");
		soundManager->AddAndLoadSound("ChargeShot");
		soundManager->AddAndLoadSound("Cancel");
		soundManager->AddAndLoadSound("PlayerDamage");
		soundManager->AddAndLoadSound("EnemyDamage");
		soundManager->AddAndLoadSound("EnemyCrash");
		soundManager->AddAndLoadSound("Deside");
		soundManager->AddAndLoadSound("Graze");
		soundManager->AddAndLoadSound("ItemGet");
		soundManager->AddAndLoadSound("LastLife");
		soundManager->AddAndLoadSound("PlayerShot");
		soundManager->AddAndLoadSound("PowerUp");
		soundManager->AddAndLoadSound("Stage01");
		soundManager->AddAndLoadSound("Title");
	}

	void InitKeys()
	{
		auto input = Singleton<InputManager>::Get();
		//	キーボードの初期化
		input->RregisterKeyboardKey(Input::MoveUp, VK_UP);
		input->RregisterKeyboardKey(Input::MoveDown, VK_DOWN);
		input->RregisterKeyboardKey(Input::MoveRight, VK_RIGHT);
		input->RregisterKeyboardKey(Input::MoveLeft, VK_LEFT);
		input->RregisterKeyboardKey(Input::Pause, VK_SPACE);
		input->RregisterKeyboardKey(Input::Escape, VK_ESCAPE);
		input->RregisterKeyboardKey(Input::Slow, VK_LSHIFT);
		input->RregisterKeyboardKey(Input::Bomb, 'X');
		input->RregisterKeyboardKey(Input::Charge, 'A');
		input->RregisterKeyboardKey(Input::Shot, 'Z');
		input->RregisterKeyboardKey(Input::Decide, 'Z');
		input->RregisterKeyboardKey(Input::Cansel, 'X');
		input->RregisterKeyboardKey(Input::Debug, 'G');

		auto configManager = Singleton<ConfigManager>::Get();
#ifndef _USE_ARCHIVE_
		configManager->DataPath("Resources/Data/");
#endif
		configManager->Import("config.dat", true);

		//	パッドの初期化
		input->AddPadInput();
		input->AddPadInput();
		input->RregisterAllGamepadKey(Input::Shot, configManager->IntData("Shot"));
		input->RregisterAllGamepadKey(Input::Decide, configManager->IntData("Decide"));
		input->RregisterAllGamepadKey(Input::Cansel, configManager->IntData("Cansel"));
		input->RregisterAllGamepadKey(Input::Pause, configManager->IntData("Pause"));
		input->RregisterAllGamepadKey(Input::Slow, configManager->IntData("Slow"));
		input->RregisterAllGamepadKey(Input::Bomb, configManager->IntData("Bomb"));
		input->RregisterAllGamepadKey(Input::Charge, 0x80);	//	仮
	}


	void InitLua()
	{
		auto luaBind = Singleton<LuaBindLib::LuaBind>::Get();
		luaBind->ScriptPath("Resources/Scripts/");
		auto lua = luaBind->LuaState();
		lua->new_usertype<Math::Vector2>(
			"Vec2",
			sol::constructors<sol::types<>, sol::types<float>, sol::types<float, float>>(),

			sol::meta_function::addition, &Math::Vector2::operator+,
			sol::meta_function::subtraction, &Math::Vector2::operator-,

			"x", &Math::Vector2::x,
			"y", &Math::Vector2::y,

			"add", [](Math::Vector2 argRight, Math::Vector2 argLeft) {argRight += argLeft; },
			"sub", [](Math::Vector2 argRight, Math::Vector2 argLeft) {argRight -= argLeft; }

		);
		lua->set_function("Vec2", [](float argX, float argY) { return Math::Vector2(argX, argY); });
		lua->set_function("Vec2", [](float argValue) { return Math::Vector2(argValue); });

		EnemyMovePattern::MovePatterns();
		EnemyBulletMovePattern::MovePatterns();
	}

	void InitViewport()
	{
		auto WindowSize = Math::Vector2(Window::Get()->WindowRect().right * 0.5f, static_cast<float>(Window::Get()->WindowRect().bottom));
		D3D11_VIEWPORT viewport;
		unsigned int num = 1;
		scene_->deviceResources_->ImContext()->RSGetViewports(&num, &viewport);
		auto view = Singleton<ViewportManager>::Get();
		view->CreateViewport(Math::Vector2(PlayScreenLeft * 0.5f, static_cast<float>(PlayScreenTop)), Math::Vector2(WindowSize.x - PlayScreenLeft * 1.25f, PlayScreenBottom - static_cast<float>(PlayScreenTop)), "Left");
		view->CreateViewport(Math::Vector2(PlayScreenLeft * 0.5f + WindowSize.x, static_cast<float>(PlayScreenTop)), Math::Vector2(WindowSize.x - PlayScreenLeft * 1.25f, PlayScreenBottom - static_cast<float>(PlayScreenTop)), "Right");
		view->CreateViewport(viewport, "Default");
	}

	void Update()
	{
		if (FAILED(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED)))
			return;

		// グラフィックマネージャーの初期化
		InitGraphic();

		// サウンドマネージャーの初期化
		InitSound();

		//	キーの初期化
		InitKeys();

		InitLua();


		graphManager_->ConfigTextColor(D2D1::ColorF::Black);
		ReplayViewer::CanUseArchive(canUseArchive_);
		Singleton<EnemyManager>::Get()->CanUseArchive(canUseArchive_);
		ReplayViewer::DataPath("Resources/Data/");

#ifndef _USE_ARCHIVE_
		Singleton<ScoreManager>::Get()->DataPath("Resources/Data/");
#endif

		CoUninitialize();

		isLoading_ = false;
	}

	void Draw()
	{
		isLoading_ = true;
		//	ローディング画面で扱うオブジェクトの設定
		auto immediate = scene_->deviceResources_->ImContext().Get();
		std::unique_ptr<Camera> camera = std::make_unique<Camera>();
		std::unique_ptr<SolidBody> logo = std::make_unique<SolidBody>("Logo", camera.get(), immediate);
		logo->Scale(Math::Vector3(0.2f, 0.2f, 0.2f));
		logo->Rot(logo->Rot() + Math::Vector3(0.f, 0.f, Math::ToRadian(180.f)));

		RasterizerStateManager rasterizerStateManager = RasterizerStateManager(immediate);
		rasterizerStateManager.ChangeState(immediate, eShading::Cullnone);

		float speed = 0.5f;

		while (isLoading_)
		{// ローディング中のアニメーション

			scene_->deviceResources_->ClearWindow(DirectX::Colors::MidnightBlue);

			Math::Vector3 rot = logo->Rot();
			rot.y += std::sin(Math::ToRadian(speed)) * 1.f;
			logo->Rot(rot);
			DrawGraph(Math::Vector2(0, 0), graph_[eImage::LoadingBack]);
			DrawGraph(Math::Vector2(0, 0), graph_[eImage::LoadingStl]);

			graphManager_->GetSpriteBatch()->Draw();

			logo->Update();
			logo->Draw();

			scene_->deviceResources_->SwapChain()->Present(0, 0);
		}
		rasterizerStateManager.ChangeState(immediate, eShading::Default);
	}

private:
	enum eImage
	{
		LoadingStl,
		LoadingBack,

		Max,
	};
private:
	//LeikaShooting				*game_;
	LoadScene					*scene_;
	GraphManager				*graphManager_;
	std::shared_ptr<Texture>	graph_[eImage::Max];
	bool						isLoading_;
	bool						canUseArchive_;

};

LoadScene::LoadScene(SceneManager *argSceneManager, const std::shared_ptr<DeviceResources> &argDeviceResources)
: Scene(argSceneManager, argDeviceResources)
{
	pImpl = std::make_unique<Impl>(this);
}

LoadScene::~LoadScene() = default;

bool LoadScene::Update()
{
#if defined(DEBUG) || defined(_DEBUG) 
	sceneManager_->ChangeScene(eSceneID::Play);
#else
	sceneManager_->ChangeScene(eSceneID::Title);
#endif
	sceneManager_->Update();

	return true;
}

void LoadScene::Draw()
{
}

