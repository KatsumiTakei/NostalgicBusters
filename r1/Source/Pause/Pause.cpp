/**
 *	@file	Pause.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "Pause.h"

#include "ReturnFirstStageButton.h"
#include "ReturnTitleButton.h"
#include "ResumeButton.h"
#include "../Config.h"
#include "../Input/Input.h"

class Pause::Impl
{
public:

	Impl(Pause *argPause, SceneManager *argSceneManager)
		:isPause_(false), input_(Singleton<InputManager>::Get())
	{
		pauseMenu_ = std::make_unique<CMenu>(new ResumeButton(argPause, argSceneManager), new ReturnFirstStageButton(argSceneManager));
		pauseMenu_->AddButton(new ReturnTitleButton(argSceneManager));
		graph_ = Singleton<GraphManager>::Get()->FindIndexTexture(L"Resultbord");
	}

	~Impl() = default;

public: 


	bool Update()
	{
		if (input_->IsTouched(Input::Pause))
		{// ポーズの切り替え

			if (!isPause_)
			{
				isPause_ = true;
				//	HACK	:	ポーズ時の音
				Singleton<SoundManager>::Get()->PlaySounds("Deside", eSoundType::Se);
			}
		}

		if (!isPause_)
			return false;

		if (!pauseMenu_->Enter())
		{
			if (input_->IsTouched(Input::MoveDown))
			{
				Singleton<SoundManager>::Get()->PlaySounds("CursolMove", eSoundType::Se);
				pauseMenu_->Next();
			}
			else if (input_->IsTouched(Input::MoveUp))
			{
				Singleton<SoundManager>::Get()->PlaySounds("CursolMove", eSoundType::Se);
				pauseMenu_->Prev();
			}
		}
		return true;
	}

	void Draw()
	{
		if (!isPause_)
			return;

		DrawRotaGraph(Math::Vector2(static_cast<float>(PlayScreenLeft) + 320.f, static_cast<float>(PlayScreenTop) + 240.f), graph_->ImageSize(), 2.f, 0.f, graph_);

		pauseMenu_->Draw();
	}


private:
	std::unique_ptr<CMenu> pauseMenu_;
	std::shared_ptr<Texture> graph_;
	InputManager *input_;
	
public:
	bool isPause_;
};

Pause::Pause(SceneManager *argSceneManager)
	:pImpl(std::make_unique<Impl>(this, argSceneManager))
{
}

Pause::~Pause() = default;


bool Pause::Update()
{
	return pImpl->Update();
}

void Pause::Draw()
{
	pImpl->Draw();
}

bool Pause::IsPause()const
{
	return pImpl->isPause_;
}

void Pause::IsPause(bool argFlag)
{
	pImpl->isPause_ = argFlag;
}
