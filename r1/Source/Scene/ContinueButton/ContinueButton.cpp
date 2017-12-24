/**
 *	@file	ContinueButton.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */

#include "ContinueButton.h"
#include "../../Input/Input.h"

class ContinueButton::Impl
{
public:
	Impl(ContinueButton *argContinueButton)
		:continueButton_(argContinueButton), graph_(Singleton<GraphManager>::Get()->FindIndexTexture(L"PausemMenu")),
		input_(Singleton<InputManager>::Get())
	{
	}

	~Impl() = default;

public:

	bool Enter()
	{
		bool result = false;
		if (input_->IsTouched(Input::Decide))
		{
			result = true;
			Singleton<SoundManager>::Get()->PlaySounds("Deside", eSoundType::Se);
		}

		return result;
	}

	void Draw()
	{
		DrawRectGraph(Math::Vector2(250, 330), Math::Vector2(0, 138), Math::Vector2(160, 24), graph_, continueButton_->buttonColor_[ContinueButton::Restart]);
		DrawRectGraph(Math::Vector2(250, 360), Math::Vector2(0, 24), Math::Vector2(160, 20), graph_, continueButton_->buttonColor_[ContinueButton::ReturnTitle]);
	}

public:
	ContinueButton *continueButton_;
	InputManager *input_;
	std::shared_ptr<Texture> graph_;
};

ContinueButton::ContinueButton()
	:pImpl(new Impl(this))
{
	for (int i = 0; i < ContinueButton::Max; i++)
	{
		buttonColor_[i] = Math::Vector4(0.f, 0.f, 0.f, 0.5f);
 	}
}

ContinueButton::~ContinueButton() = default;

bool ContinueButton::Enter()
{
	return pImpl->Enter();
}

void ContinueButton::Draw()
{
	pImpl->Draw();
}