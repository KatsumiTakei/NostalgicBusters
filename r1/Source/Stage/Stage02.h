/**
 * @file   Stage02.h
 * @brief  Stage02�̐錾
 * @author Katsumi Takei
 * @date   2017 / 02 / 24
 */
#pragma once

#include "Stage.h"
#include "../stdafx.h"

class Stage02 : public Stage
{

#pragma region Object
private:
	class Impl;
	std::unique_ptr<Impl>pImpl;

#pragma endregion
#pragma region PrivateFunction
private:
	void InitializeBackGround() override;
	void ScrollBack() override;

#pragma endregion
#pragma region CreaterAndDestroyer
public:

	Stage02();
	~Stage02();
#pragma endregion
#pragma region PublicFunction
public:
	//void initialize() override;
	bool Update() override;
	void Draw() override;
	//void finalize() override;
#pragma endregion
};