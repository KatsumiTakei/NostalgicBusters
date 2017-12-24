/**
 *	@file	PlayerStateManager.cpp
 *	@date	2017 / 11 / 01
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "PlayerStateManager.h"
#include "../Player.h"

#include "DefaultSpdState.h"
#include "LowSpdState.h"

class PlayerStateManager::Impl
{
public:
	Impl(Player * argPlayer)
		: currentState_(eState::Default)
	{
		stateList_[static_cast<int>(eState::Default)] = std::make_unique<DefaultSpdState>(argPlayer);
		stateList_[static_cast<int>(eState::Low)] = std::make_unique<LowSpdState>(argPlayer);
	}
	~Impl() = default;

public:
	std::unique_ptr<PlayerState> stateList_[static_cast<int>(eState::Max)];
	eState currentState_;

};

PlayerStateManager::PlayerStateManager(Player * argPlayer)
	:pImpl(std::make_unique<Impl>(argPlayer))
{
}

PlayerStateManager::~PlayerStateManager() = default;

bool PlayerStateManager::Update()
{
	return pImpl->stateList_[static_cast<int>(pImpl->currentState_)]->Update();
}

void PlayerStateManager::Draw()
{
	pImpl->stateList_[static_cast<int>(pImpl->currentState_)]->Draw();
}

void PlayerStateManager::ChangeState(eState argNextState)
{
	pImpl->currentState_ = argNextState;
	pImpl->stateList_[static_cast<int>(pImpl->currentState_)]->OnChangeState();
}

eState PlayerStateManager::GetCurrentState() const
{
	return pImpl->currentState_;
}

