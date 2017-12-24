/**
 *	@file	BattleButton.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "BattleButton.h"

#include "../../Scene/SceneManager.h"
#include "../../Input/Input.h"

class BattleButton::Impl
{
public:
	Impl(BattleButton *argStartButton, SceneManager *argSceneManager)
		:sceneManager_(argSceneManager),input_(Singleton<InputManager>::Get()), startButton_(argStartButton)
	{

	}
	~Impl() = default;

public:
	bool Execute()
	{
		sceneManager_->ChangeScene(eSceneID::Play);

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
	BattleButton *startButton_;
	SceneManager *sceneManager_;
	InputManager *input_;

};


BattleButton::BattleButton(SceneManager *argSceneManager)
	:TitleButton(),pImpl(std::make_unique<Impl>(this, argSceneManager))
{
	buttonColor_[TitleButton::Story].w = 1.f;
	introduction_ = L"そのままゲームを始めます";
}

BattleButton::~BattleButton() = default;

bool BattleButton::Execute()
{
	return pImpl->Execute();
}

bool BattleButton::Enter()
{
	return TitleButton::Enter() ? pImpl->Enter() : false;
}

void BattleButton::Draw()
{
	TitleButton::Draw();
}
