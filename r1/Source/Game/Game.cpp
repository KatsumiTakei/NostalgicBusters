/**
 *	@file	Game.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "Game.h"
#include "../Scene/SceneManager.h"
#include <LuaBind.h>

#ifndef _USE_ARCHIVE_
#include "../../Resources/Icon/resource.h"
#endif // _USE_ARCHIVE_

class Game::Impl
{
public:

	Impl(Game *argGame) 
		: game_(argGame)
	{
		setlocale(LC_ALL, "japanese");
#if defined(DEBUG) || defined(_DEBUG)
		//	ウィンドウイベント登録
		auto WinEventProc = [](HWINEVENTHOOK hWinEventHook, unsigned long argEvent, HWND argHwnd, long argIdObject, long argIdChild, unsigned long argDwEventThread, unsigned long argDwmsEventTime)
		{
			if (argEvent == EVENT_SYSTEM_FOREGROUND)
			{
				Singleton<LuaBindLib::LuaBind>::Get()->ReloadScript("EnemyMove.lua");
				Singleton<LuaBindLib::LuaBind>::Get()->ReloadScript("ShotMove.lua");
			}
		};
		SetWinEventHook(EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND, nullptr, WinEventProc, 0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
		Debug::CreateConsole();
#endif

		LPCSTR ArchiveName = "Nostalgic Busters";

#ifdef _USE_ARCHIVE_
		game_->archive_ = Singleton<Archive>::Get();
		game_->archive_->Export("Resources", ArchiveName);
#endif// _USE_ARCHIVE_

		LPCWCHAR WindowTitle = L"Nostalgic Busters";

		if (game_->archive_)
		{
			game_->archive_->Import(ArchiveName);
			HICON icon = Icon::Load(game_->archive_, "Resources/Icon/icon.ico");
			window_ = std::make_unique<Window>(WindowTitle, icon);
		}
		else
		{
#ifndef _USE_ARCHIVE_
			window_ = std::make_unique<Window>(WindowTitle, Icon::Load(IDI_ICON1));
#else
			game_->archive_->Import("Leika.dat");
			HICON icon = Icon::Load(game_->archive_, "Resources/Icon/icon.ico");
			window_ = std::make_unique<Window>(WindowTitle, icon);
#endif// _USE_ARCHIVE_
		}

	}
	~Impl()
	{
		Debug::DestroyConsole();
		SingletonFinalizer::Finalize();
	}
private:

	void Execute()
	{
		game_->fps_->Wait();

		game_->deviceResources_->ClearWindow(DirectX::Colors::MidnightBlue);
		
		game_->sceneManager_->Update();
		game_->sceneManager_->Draw();

		game_->deviceResources_->SwapChain()->Present(0, 0);
	}

public:

	MSG Run()
	{
		MSG msg = { 0 };
		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				Execute();
			}
		}
		return msg;
	}

private:
	Game *game_;
	std::unique_ptr<Window> window_;
};

Game::Game()
	:pImpl(std::make_unique<Impl>(this)), fps_(std::make_unique<QueryPerformanceTimer>()),
	deviceResources_(std::make_shared<DeviceResources>(Window::Get()->WindowHundle()))
{
}

Game::~Game() = default;


MSG Game::Run()
{
	return pImpl->Run();
}