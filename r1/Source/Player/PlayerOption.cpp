/**
 *	@file	PlayerOption.cpp
 *	@date	2017 / 11 / 01
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */

#include "PlayerOption.h"

class PlayerOption::Impl
{
public:
	Impl(const int argMax)
		:optionMax_(argMax), graph_(Singleton<GraphManager>::Get()->FindIndexTexture(L"Option")), angle_(0.f)
	{
		optionPos_.resize(argMax);
	}
	~Impl() = default;

public:
	void Update(const Math::Vector2 argPlayerPos, const Math::Vector2 *argShotPos)
	{
		for (int lOptionIndex = 0; lOptionIndex < optionMax_; lOptionIndex++)
		{
			optionPos_[lOptionIndex] = argPlayerPos + argShotPos[lOptionIndex + 1];
		}
		angle_ += Math::ToRadian(5);
	}

	void Draw(const int argShotMax)
	{
		for (int lOptionIndex = 0, lOptionMax = argShotMax; lOptionIndex < lOptionMax; lOptionIndex++)
		{
			const float Angle = (lOptionIndex % 2 == 0) ? -angle_ : angle_;
			DrawRotaGraph(optionPos_[lOptionIndex], Math::Vector2(18), Math::Vector2(9), 1.f, Angle, graph_);
		}
	}

private:
	//	HACK	:	角度はPlayer別のオプションに移譲
	float angle_;
public:
	int optionMax_;
	std::vector<Math::Vector2>	optionPos_;
	std::shared_ptr<Texture>	graph_;

};

PlayerOption::PlayerOption(const int argMax)
	:pImpl(std::make_unique<Impl>(argMax))
{
}

PlayerOption::~PlayerOption() = default;

void PlayerOption::Update(const Math::Vector2 argPlayerPos, const Math::Vector2 *argShotPos)
{
	pImpl->Update(argPlayerPos, argShotPos);
}

void PlayerOption::Draw(const int argShotMax)
{
	pImpl->Draw(argShotMax);
}

