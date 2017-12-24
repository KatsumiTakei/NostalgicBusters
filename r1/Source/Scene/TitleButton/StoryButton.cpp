/**
 *	@file	BattleButton.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "StoryButton.h"

#include "../../Scene/SceneManager.h"
#include "../../Input/Input.h"

class StoryButton::Impl
{
public:
	Impl(StoryButton  *argStartButton, SceneManager *argSceneManager)
		:sceneManager_(argSceneManager),input_(Singleton<InputManager>::Get()), startButton_(argStartButton)
	{

	}
	~Impl() = default;

public:
	bool Execute()
	{
		sceneManager_->FadeChangeScene(eSceneID::Play);

		return true;
	}
	bool Enter()
	{
		Execute();

		if (input_->IsTouched(Input::Cansel))
		{
			startButton_->isEnter_ = false;
			Singleton<SoundManager>::Get()->PlaySounds("Cancel", eSoundType::Se);
		}

		return startButton_->isEnter_;
	}
	
	void Draw()
	{
	}
private:
	StoryButton  *startButton_;
	SceneManager *sceneManager_;
	InputManager *input_;

};


StoryButton::StoryButton(SceneManager *argSceneManager)
	:TitleButton(),pImpl(new Impl(this, argSceneManager))
{
	buttonColor_[TitleButton::Battle].w = 1.f;
	introduction_ = L"すぐに対戦が始まります";
}

StoryButton::~StoryButton() = default;

bool StoryButton::Execute()
{
	return pImpl->Execute();
}

bool StoryButton::Enter()
{
	return TitleButton::Enter() ? pImpl->Enter() : false;
}

void StoryButton::Draw()
{
	TitleButton::Draw();
}
