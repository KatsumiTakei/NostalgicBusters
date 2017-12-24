/**
 *	@file	Result.cpp
 *	@date	2017 / 7 / 20
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "Result.h"
#include "../ScoreManager/ScoreManager.h"
#include "../Config.h"
#include "../Input/Input.h"

class Result::Impl 
{
public:
	enum eImage
	{
		Message,
		Number,

		Max,
	};

public:
	Impl()
	{
		image_[eImage::Message] = Singleton<GraphManager>::Get()->FindIndexTexture(L"ResultMessege");
		image_[eImage::Number] = Singleton<GraphManager>::Get()->FindIndexTexture(L"Number");
	}
	~Impl() = default;
	
public:
	std::shared_ptr<Texture> image_[eImage::Max];
	std::unique_ptr<Tween<Math::Vector2>> tween_;
	bool canDrawing_ = false;
};

Result::Result()
	:pImpl(new Impl())
{	
	isLiving_ = false;
	graph_ = Singleton<GraphManager>::Get()->FindIndexTexture(L"Resultbord");
	priority_ = ePriority::Efect;
	drawMode_ = eDrawMode::Alpha;
	count_ = FlameTime * 10;
}

Result::~Result() = default;

Result *Result::Create()
{
	pos_ = Math::Vector2(static_cast<float>(PlayScreenLeft), -static_cast<float>(PlayScreenBottom));
	count_ = FlameTime * 10;
	pImpl->tween_ = std::make_unique<Tween<Math::Vector2>>(pos_, Math::Vector2(static_cast<float>(PlayScreenLeft), static_cast<float>(PlayScreenTop)), static_cast<float>(FlameTime * 3), Ease::EaseIn);
	isLiving_ = true;
	return this;
}

bool Result::Update()
{
	if (Singleton<InputManager>::Get()->IsTouched(Input::Decide)) 
	{
		isLiving_ = false;
	}

	if (pos_.y < PlayScreenTop)
	{
		pImpl->tween_->Update();
	}
	else
	{
		pImpl->canDrawing_ = true;
	}

	return isLiving_;
}

void Result::Draw()
{
	DrawGraph(pos_, graph_);

	if (!pImpl->canDrawing_)
		return;

	const int DigitNum = 9;
	DrawNum(Math::Vector2(PlayScreenLeft * 15.f, PlayScreenTop * 15.f), Math::Vector2(12.f, 24.f), static_cast<float>(Singleton<ScoreManager>::Get()->Score()), pImpl->image_[Impl::eImage::Number], DigitNum, Math::Vector4(1.f, 1.f, 1.f, 1.f));
	DrawRectGraph(Math::Vector2(PlayScreenLeft * 2.f, PlayScreenTop * 2.f), Math::Vector2(0, 0), Math::Vector2(360.f, 75.f), pImpl->image_[Impl::eImage::Message]);

	Singleton<GraphManager>::Get()->ConfigTextColor(D2D1::ColorF::White);
	Singleton<GraphManager>::Get()->DrawAndWriteText(L"現在のスコア", Math::Vector2(PlayScreenLeft * 5.f, PlayScreenTop * 15.f));
	Singleton<GraphManager>::Get()->DrawAndWriteText(L"ショットキーを押してね", Math::Vector2(PlayScreenLeft * 5.f, PlayScreenBottom - PlayScreenTop * 5.f));
	Singleton<GraphManager>::Get()->ConfigTextColor(D2D1::ColorF::Black);
}
