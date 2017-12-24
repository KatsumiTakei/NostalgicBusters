/**
 *	@file	PauseButton.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "PauseButton.h"
#include "../Scene/SceneManager.h"
#include "../Input/Input.h"

class PauseButton::Impl
{
public:

	Impl(PauseButton *argPauseButton, SceneManager *argSceneManager)
		:sceneManager_(argSceneManager),pauseButton_(argPauseButton),input_(Singleton<InputManager>::Get()),
		isAccept_(false), graph_(Singleton<GraphManager>::Get()->FindIndexTexture(L"Pausemenu"))
	{
	}

	~Impl() = default;

public:

	bool Execute()
	{
		if (input_->IsTouched(Input::MoveDown) || input_->IsTouched(Input::MoveUp))
		{
			isAccept_ = !isAccept_;
			Singleton<SoundManager>::Get()->PlaySounds("CursolMove", eSoundType::Se);
		}
		
		if (input_->IsTouched(Input::Decide))
		{
			if (isAccept_)
			{
				Singleton<SoundManager>::Get()->PlaySounds("Deside", eSoundType::Se);
				sceneManager_->ChangeScene(pauseButton_->nextSceneId_);
			}
			else
			{
				Singleton<SoundManager>::Get()->PlaySounds("Cancel", eSoundType::Se);
				//pauseButton_->isEnter_ = !pauseButton_->isEnter_;

			}
		}
		return true;
	}

	bool Enter()
	{
		if (pauseButton_->isEnter_)
		{
			Execute();
		}
		if (input_->IsTouched(Input::Decide))
		{
			pauseButton_->isEnter_ = !pauseButton_->isEnter_;
			if(pauseButton_->isEnter_)
				Singleton<SoundManager>::Get()->PlaySounds("Deside", eSoundType::Se);
		}

		return pauseButton_->isEnter_;
	}

	void Draw()
	{
		// ボタン
		DrawRectGraph(Math::Vector2(240, 210), Math::Vector2(0, 0), Math::Vector2(160, 22), graph_, pauseButton_->buttonColor_[PauseButton::Resume]);
		DrawRectGraph(Math::Vector2(240, 240), Math::Vector2(0, 24), Math::Vector2(160, 20), graph_, pauseButton_->buttonColor_[PauseButton::ReturnTitle]);
		DrawRectGraph(Math::Vector2(240, 270), Math::Vector2(0, 48), Math::Vector2(160, 20), graph_, pauseButton_->buttonColor_[PauseButton::ReturnFirstStage]);

		if (!pauseButton_->isEnter_)
			return;


		// 確認のお言葉
		DrawRectGraph(Math::Vector2(300, 300), Math::Vector2(0, 68), Math::Vector2(100, 24), graph_);

		Math::Vector4 colorYes = Math::Vector4(0.f, 0.f, 0.f, 0.5f);
		Math::Vector4 colorNo = Math::Vector4(0.f, 0.f, 0.f, 0.5f);

		(isAccept_) ? colorYes = Math::Vector4(1.f, 1.f, 1.f, 1.f) : colorNo = Math::Vector4(1.f, 1.f, 1.f, 1.f);
	
		DrawRectGraph(Math::Vector2(300, 330), Math::Vector2(0, 92), Math::Vector2(100, 22), graph_, colorYes);
		DrawRectGraph(Math::Vector2(300, 360), Math::Vector2(0, 114), Math::Vector2(100, 22), graph_, colorNo);

	}

public:
	PauseButton *pauseButton_;
	SceneManager *sceneManager_;
	InputManager *input_;
	std::shared_ptr<Texture> graph_;
	bool isAccept_;
};

PauseButton::PauseButton(SceneManager *argSceneManager)
	:pImpl(std::make_unique<Impl>(this, argSceneManager)),
	nextSceneId_(eSceneID::None), isEnter_(false)
{
	for (int i = 0; i < PauseButton::Num; i++)
	{
		buttonColor_[i] = Math::Vector4(0.f, 0.f, 0.f, 0.5f);
	}
}

PauseButton::~PauseButton() = default;

bool PauseButton::Execute()
{
	return pImpl->Execute();
}

bool PauseButton::Enter()
{
	return pImpl->Enter();
}

void PauseButton::Draw()
{
	return pImpl->Draw();
}