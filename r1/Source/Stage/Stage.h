/**
 *	@file	Stage.h
 *	@date	2017 / 7 / 26
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../stdafx.h"

class Player;
class Enemy;

class Stage : public Task
{
private:
	class Impl;
	std::unique_ptr<Impl>pImpl;

protected:
	std::unique_ptr<Camera> camera_;
	std::wstring enemyFileName_;
	std::string viewPortName_;
protected:
	/**
	 *  @fn		ScrollBack
	 *  @brief	背景のスクロール
	 */
	virtual void ScrollBack() = 0;

public:
	/**
	 *  @constructor		Stage01
	 *  @brief				ステージ01
	 *  @param[in]			argPlayer			!<	プレイヤー
	 *  @param[in]			argNextStage		!<	次のステージ
	 *  @param[in]			argEnemyFileName	!<	ステージの敵のファイルパス
	 *  @param[in]			argSoundFileName	!<	ステージのサウンドパス
	 */
	Stage(Player *argPlayer, Stage *argNextStage, std::wstring argEnemyFileName, std::string argSoundFileName, std::string argViewPortName);
	virtual ~Stage();

public:
	virtual void Initialize();
	virtual bool Update() override;
	virtual void Draw() override = 0;
	void Finalize();

public:
	std::vector<std::shared_ptr<Enemy>> EnemyList()const;
	void Count(int argCount);
	int Count()const;

	static bool IsFinished();
	static void IsFinished(bool argIsFinished);

};