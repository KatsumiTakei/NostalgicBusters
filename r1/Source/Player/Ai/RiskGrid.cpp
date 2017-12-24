/**
 *	@file	RiskGrid.cpp
 *	@date	2017 / 11 / 13
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "RiskGrid.h"

RiskGrid::RiskGrid(Math::Vector2 argIndex, Math::Vector2 argSize, int argRisk)
	:size_(argSize), riskPoint_(argRisk), index_(argIndex), isHitting_(false)
{
	for (int lRiskIndex = 0; lRiskIndex < AroundGridMax; lRiskIndex++)
	{
		aroundRisk_[lRiskIndex] = nullptr;
	}

	Math::Vector2 gridLeftTop = size_ * index_;
	Math::Vector2 gridRightBottom = gridLeftTop + size_;

	cornerRect_ = { static_cast<LONG>(gridLeftTop.x), static_cast<LONG>(gridLeftTop.y), static_cast<LONG>(gridRightBottom.x), static_cast<LONG>(gridRightBottom.y), };

}

RiskGrid::~RiskGrid() = default;

RECT RiskGrid::CornerRect()const
{
	return cornerRect_;
}

void RiskGrid::IsHitting(bool argIsHittiog)
{
	isHitting_ = argIsHittiog;
}

bool RiskGrid::IsHitting() const
{
	return isHitting_;
}

int RiskGrid::RiskPoint() const
{
	return riskPoint_;
}

int *RiskGrid::RiskPoint()
{
	return &riskPoint_;
}

void RiskGrid::PulsRiskPoint(int argValue)
{
	riskPoint_ += argValue;
}

void RiskGrid::RiskPoint(int argRiskPoint)
{
	riskPoint_ = argRiskPoint;
}

void RiskGrid::AroundRisk(int * argRiskPoint, int argIndex)
{
	aroundRisk_[argIndex] = argRiskPoint;
}

int **RiskGrid::AroundRisk()
{
	return aroundRisk_;
}

Math::Vector2 RiskGrid::Index() const
{
	return index_;
}

void RiskGrid::Draw(const std::weak_ptr<Texture> &argTexture)
{
	DrawNum(size_ * index_ + Math::Vector2(size_.x * 0.5f, size_.y * 0.25f), Math::Vector2(size_.y * 0.25f, size_.y * 0.5f), static_cast<float>(Risk()), argTexture, 3);
}

int RiskGrid::Risk()
{
	risk_ = riskPoint_;

	for (int lRiskIndex = 0; lRiskIndex < AroundGridMax; lRiskIndex++)
	{
		if (aroundRisk_[lRiskIndex])
			risk_ += static_cast<int>(*aroundRisk_[lRiskIndex] * 0.5);
	}
	return risk_;
}

int *RiskGrid::pRisk()
{
	Risk();
	return &risk_;
}
