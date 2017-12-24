/**
 *	@file	Game.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "LeikaShooting.h"

#include "../Scene/SceneManager.h"

#ifdef _MD	//	Run-Time Library

#pragma comment(lib, "UtilityLib.lib")	//	!<	DynamicLibrary
#pragma comment(lib, "LuaBindLib.lib")	//	!<	DynamicLibrary
#else	//	_MT

#pragma comment(lib, "UtilityLib.lib")	//	!<	StaticLibrary
#pragma comment(lib, "LuaBindLib.lib")	//	!<	StaticLibrary
#endif	//	Run-Time Library

#pragma comment(linker, "/NODEFAULTLIB:libcmt.lib")

class LeikaShooting::Impl
{
public:

	Impl(LeikaShooting *argGame)
		:game_(argGame)
	{
		Singleton<GraphManager>::Get()->Initialize(game_->deviceResources_->ImContext().Get(), game_->deviceResources_->SwapChain());
		game_->sceneManager_ = std::make_unique<SceneManager>(eSceneID::Load, game_->deviceResources_);
	}
	~Impl() = default;


private:
	LeikaShooting *game_;
};

LeikaShooting::LeikaShooting()
{
	pImpl = std::make_unique<Impl>(this);
}

LeikaShooting::~LeikaShooting() = default;

