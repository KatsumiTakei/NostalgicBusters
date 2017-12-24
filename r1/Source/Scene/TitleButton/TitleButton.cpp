/**
 *	@file	TitleButton.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */

#include "TitleButton.h"
#include "../../Input/Input.h"

class TitleButton::Impl
{
public:
	Impl(TitleButton *argTitleButton)
		:titleButton_(argTitleButton), graph_(Singleton<GraphManager>::Get()->FindIndexTexture(L"Titletext")),
		input_(Singleton<InputManager>::Get())
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
		if (input_->IsTouched(Input::Decide))
		{
			titleButton_->isEnter_ = true;
			Singleton<SoundManager>::Get()->PlaySounds("Deside",  eSoundType::Se);
		}

		return titleButton_->isEnter_;
	}

	void Draw()
	{
		const Math::Vector2 Pos = Math::Vector2(180.f, 440.f);
		Singleton<GraphManager>::Get()->DrawAndWriteText(titleButton_->introduction_.c_str(), Pos);
		
		auto temp = Singleton<GraphManager>::Get()->FindIndexTexture(L"Keepout");

		DrawRectGraph(Math::Vector2(140, 250), Math::Vector2(0, 0), Math::Vector2(110, 24), graph_, titleButton_->buttonColor_[TitleButton::Story]);
		DrawRectRotaGraph(Math::Vector2(170, 250), Math::Vector2(0, 0), temp->ImageSize(), 0.5f, Math::ToRadian(-45.f), temp);

		DrawRectGraph(Math::Vector2(160, 280), Math::Vector2(0, 24), Math::Vector2(110, 24), graph_, titleButton_->buttonColor_[TitleButton::Battle]);
		
		
		DrawRectGraph(Math::Vector2(180, 310), Math::Vector2(0, 48), Math::Vector2(110, 24), graph_, titleButton_->buttonColor_[TitleButton::Config]);
		DrawRectRotaGraph(Math::Vector2(210, 330), Math::Vector2(0, 0), temp->ImageSize(), 0.5f, Math::ToRadian(-45.f), temp);
		
		DrawRectGraph(Math::Vector2(200, 360), Math::Vector2(0, 72), Math::Vector2(48, 24), graph_, titleButton_->buttonColor_[TitleButton::Quit]);
	}

public:
	TitleButton *titleButton_;
	InputManager *input_;
	std::shared_ptr<Texture> graph_;
};

TitleButton::TitleButton()
	:pImpl(std::make_unique<Impl>(this)),isEnter_(false)
{
	for (int i = 0; i < TitleButton::Max; i++)
	{
		buttonColor_[i] = Math::Vector4(0.f, 0.f, 0.f, 0.5f);
 	}
}

TitleButton::~TitleButton() = default;

bool TitleButton::Enter()
{
	return pImpl->Enter();
}

void TitleButton::Draw()
{
	pImpl->Draw();
}