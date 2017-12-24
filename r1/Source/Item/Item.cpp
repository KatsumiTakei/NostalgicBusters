/**
 *	@file	Item.cpp
 *	@date	2017 / 7 / 28
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "Item.h"
#include "../Player/Player.h"
#include "../Collision/CollisionManager2D.h"
#include "../ScoreManager/ScoreManager.h"
#include "../Config.h"

namespace
{
	const float Gravity = -9.8f * (0.0005f * 0.5f);
	const float Acceleration = -1960.f * (0.00008f * 0.5f);
}

class Item::Impl
{
public:
	Impl(Item *argItem, eItemId argItemId, Player *argPlayer)
		:isHoming_(false), acceleration_(Acceleration), itemId_(argItemId), item_(argItem),
		player_(argPlayer)
	{
	}
	~Impl() = default;

public:
	void ChackPlayerState()
	{
		if (player_->Power() >= 5.)
		{
			if (itemId_ == eItemId::PowerUpItemLittle || itemId_ == eItemId::PowerUpItemBig)
			{
				itemId_ = eItemId::ScoreUpItem;
			}
		}
		if (player_->Pos().y < PlayScreenBottom * 0.16f)
		{
			isHoming_ = true;
		}
		if (isHoming_)
		{
			float dest = atan2(player_->Pos().y - item_->pos_.y, player_->Pos().x - item_->pos_.x);
			item_->angle_ = dest;
			item_->spd_ = Math::Vector2(8.f, 8.f);
		}
	}

	void Pick()
	{
		if (itemId_ == eItemId::PowerUpItemLittle || itemId_ == eItemId::PowerUpItemBig)
		{
			double add = (itemId_ == eItemId::PowerUpItemLittle) ? 0.1 : 0.3;
			
			player_->Power(player_->Power() + add);

			if (fmod(player_->Power(), 1.) == 0.)
			{
				Singleton<SoundManager>::Get()->PlaySounds("PowerUp", eSoundType::Se);
			}
			if (player_->Power() >= 5.)
			{
				const int InitCnt = 180;
				//*argMaxPowerReport = InitCnt;
			}
		}
		else if (itemId_ == eItemId::ScoreUpItem)
		{
			Singleton<ScoreManager>::Get()->AddScore(1000 * adjustScore_);
		}
	}

public:
	bool	isHoming_;
	float	acceleration_;
	eItemId	itemId_;
	Item	*item_;
	Player	*player_;
	int adjustScore_;
};

Item::Item(eItemId argItemId, Player *argPlayer)
	:GameObject2D(eShapeType::Circle), pImpl(new Impl(this, argItemId, argPlayer))
{
	radius_ = 24.f;
	isLiving_ = false;
	graph_ = Singleton<GraphManager>::Get()->FindIndexTexture(L"Item");
}

Item::~Item() = default;

Item *Item::Create(Math::Vector2 argPos)
{
	angle_ = 1.f;
	spd_ = Math::Vector2(0.f, 0.f);
	pos_ = argPos;
	count_ = 90;
	isLiving_ = true;
	pImpl->isHoming_ = false;
	pImpl->acceleration_ = Acceleration;

	return this;
}

void Item::CollisionEnter()
{
	if (collider_->PlayerInfo)
	{
		pImpl->Pick();
		Singleton<SoundManager>::Get()->PlaySounds("ItemGet", eSoundType::Se);
		isLiving_ = false;
	}
	collider_->Clear();
}

void Item::Pick(int *argMaxPowerReport)
{
	//pImpl->Pick(argMaxPowerReport);
}

bool Item::Update()
{
	if (--count_ > 0)
	{
		pImpl->acceleration_ -= Gravity;
		spd_.y += pImpl->acceleration_;
	}
	
	pImpl->ChackPlayerState();
	pos_.x += cos(angle_) * spd_.x;
	pos_.y += sin(angle_) * spd_.y;

	if (pos_.y <= PlayScreenBottom * 0.75f)
	{
		pImpl->adjustScore_ = 5;

		if (pos_.y <= PlayScreenBottom * 0.5f)
		{
			pImpl->adjustScore_ = 10;

			if (pos_.y <= PlayScreenBottom * 0.25f)
			{
				pImpl->adjustScore_ = 20;
			}
		}
	}
	if (PlayScreenBottom < pos_.y)
	{
		isLiving_ = false;
	}
	Singleton<CollisionManager2D>::Get()->Entry(this);

	return isLiving_;
}

void Item::Draw()
{
	Math::Vector2 size = graph_->ImageSize();
	size = Math::Vector2(size.x * 0.5f, size.y);
	Math::Vector2 center = size * 0.5f;

	if (pImpl->itemId_ == eItemId::PowerUpItemLittle)
	{
		DrawRotaGraph(pos_, size, Math::Vector2(0,0), 1.0, 0, graph_);
	}
	else if (pImpl->itemId_ == eItemId::PowerUpItemLittle)
	{
		DrawRotaGraph(pos_, size, Math::Vector2(0, 0), 2.0, 0, graph_);
	}
	else if (pImpl->itemId_ == eItemId::ScoreUpItem)
	{
		DrawRectRotaGraph(pos_, Math::Vector2(size.x, 0.f), size, 1.0, 0, graph_);
	}
	GameObject2D::Draw();
}