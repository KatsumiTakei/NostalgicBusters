/**
 *	@file	EnemyManager.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../../stdafx.h"

class Enemy;
class Player;
struct EnemyData;

/**
 *	@class	EnemyManager
 *	@brief	敵の管理
 */
class EnemyManager final
{
private:
	class Impl;
	std::unique_ptr<Impl> pImpl;

public:
	EnemyManager();
	~EnemyManager();

public:
	/**
	 *	@fn			EnemyList
	 *	@brief		敵のリストを取得
	 *	@param[in]	敵のリスト
	 *	@return		敵のリスト
	 */
	std::vector<std::shared_ptr<Enemy>> EnemyList(Player *argPlayer)const;
	void EnemyList(Player *argPlayer, std::vector<std::shared_ptr<Enemy>> argEnemyList);
	
	std::vector<EnemyData> EnemyDataList(Player *argPlayer)const;
	void CanUseArchive(bool argCanUseArchive);

	std::wstring DataPath();
	void DataPath(std::wstring argPath);
	
	void ClearEnemyList();
	void DestroyActiveEnemis();
	
	bool LoadEnemy(const std::wstring argFilename, Player *argPlayer);

};
