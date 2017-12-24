/**
 *	@file	QuitButton.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "ConfigButton.h"

#include "../../Input/Input.h"

class ConfigButton::Impl
{
public:
	Impl(ConfigButton *argConfigButton)
		: input_(Singleton<InputManager>::Get()), configButton_(argConfigButton)
	{

	}
	~Impl() = default;

public:
	bool Execute()
	{
		return true;
	}
	bool Enter()
	{
		Execute();

		if (input_->IsTouched(Input::Cansel))
		{
			configButton_->isEnter_ = false;
			Singleton<SoundManager>::Get()->PlaySounds("Cancel", eSoundType::Se);
		}

		return configButton_->isEnter_;
	}

	void Draw()
	{
	}
private:
	ConfigButton *configButton_;
	InputManager *input_;

};

ConfigButton::ConfigButton()
	:TitleButton(), pImpl(std::make_unique<Impl>(this))
{
	buttonColor_[TitleButton::Quit].w = 1.f;
	introduction_ = L"入力や音量を変更します";
}

ConfigButton ::~ConfigButton() = default;

bool ConfigButton::Execute()
{
	return pImpl->Execute();
}

bool ConfigButton::Enter()
{
	return TitleButton::Enter() ? Execute() : false;
}

void ConfigButton::Draw()
{
	TitleButton::Draw();
}
