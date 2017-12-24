/**
 *	@file	Player.h
 *	@date	2017 / 7 / 26
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../GameObject/GameObject2D.h"

class PlayerOption;
class PlayerWeapon;
class ChargeBullet;
enum class eState;

/**
 *	@class	Player
 *	@brief	プレイヤーの基底クラス
 */
class Player : public GameObject2D
{
private:
	class Impl;
	std::unique_ptr<Impl>pImpl;

protected:
	int ShotIndex()const;
	eState GetCurrentState()const;
protected:
	std::unique_ptr<PlayerOption>				option_;
	static const int							OptionMax_ = 8;
	std::vector<Math::Vector2>					shotPos_;
	std::vector<std::unique_ptr<PlayerWeapon>>	playerWeapon_;
	std::unique_ptr<ChargeBullet>				chargeBullet_;
public:

	Player(const int argPlayerIndex, const float argSpd, std::wstring argImageName);
	virtual ~Player();
public:
	bool HitDamage();
	
	virtual void ShotBullet() = 0;
	virtual void Initialize();

	void CollisionEnter() override;
	bool Update() override;
	void Draw() override;
	
#pragma region SetterAndGetter
public:
	double Power()const;
	void Power(double argPower);
	bool IsBonusGet() const;
	void IsBonusGet(bool argFlag);
	bool IsOperation();
	int VictoryPoint()const;
#pragma endregion

};