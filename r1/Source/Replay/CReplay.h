/**
 *	@file	CReplay.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../stdafx.h"
#include "IReplay.h"

/**
 *  @class	CReplay
 *  @brief	リプレイの抽象クラス
 */
class CReplay abstract : IReplay
{
protected:
	/**
	 *	@struct	RecData
	 *	@brief	動作データ保存用データ構造体
	 */
	typedef struct RecData
	{
		double Power;
		int  Count, Score, HiScore;
		float PosX, PosY;
		bool IsShot;

	} *lpRecData;

protected:
	
	lpRecData recData_;
	int recFrame_;
	bool isPlaying_;
	static std::string dataPath_;
public:

	/**
	 *  @constructor	CReplay
	 *  @brief			リプレイの抽象クラス
	 */
	CReplay() 
		: isPlaying_(false), recFrame_(0), recData_(lpRecData())
	{
	}
	virtual ~CReplay() = default;

public:
	virtual void Initialize() override = 0;
	virtual void Update() override = 0;
	virtual void Draw() override = 0;
	virtual void Finalize() override = 0;

public:
	/**
	 *  @fn			IsPlaying
	 *  @brief		再生中か
	 *  @retval		true	!<	再生中
	 *  @retval		false	!<	再生していない
	 */
	inline bool IsPlaying() const override { return isPlaying_; }
	/**
	 *  @fn			DataPath
	 *  @brief		リプレイデータのパス
	 *  @param[in]	argPath	!<	パス
	 */
	static void DataPath(std::string argPath) { dataPath_ = argPath; }
	/**
	 *  @fn		DataPath
	 *  @brief	データのパスを取得
	 *  @return	データのパス
	 */
	static std::string DataPath() { return dataPath_; }
};

__declspec(selectany) std::string CReplay::dataPath_ = "Resources/Data/";
