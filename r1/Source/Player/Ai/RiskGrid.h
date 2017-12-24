/**
 *	@file	RiskGrid.h
 *	@date	2017 / 11 / 13
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../../stdafx.h"

class RiskGrid
{
private:
	static const int AroundGridMax = 8;
	int *aroundRisk_[AroundGridMax];
	Math::Vector2 size_;
	Math::Vector2 index_;
	RECT cornerRect_;
	int riskPoint_;
	int risk_;
	bool isHitting_;

public:
	RiskGrid(Math::Vector2 argIndex, Math::Vector2 argSize, int argRisk);
	~RiskGrid();

public:

	void Draw(const std::weak_ptr<Texture> &argTexture);
	/**
	 *	@fn			Risk
	 *	@brief		グリッドの危険度を周囲と計算してから取得
	 *	@return		グリッドの危険度
	 */
	int Risk();
	/**
	 *	@fn			pRisk
	 *	@brief		グリッドの危険度を周囲と計算してから取得
	 *	@return		グリッドの危険度
	 */
	int *pRisk();


	/**
	 *	@fn			CornerRect
	 *	@brief		グリッドを構成する4点の座標を取得
	 *	@return		グリッドを構成する4点の座標
	 */
	RECT CornerRect()const;

	void IsHitting(bool argIsHittiog);
	bool IsHitting()const;
	int RiskPoint() const;
	int *RiskPoint();
	void PulsRiskPoint(int argValue);
	void RiskPoint(int argRiskPoint);
	void AroundRisk(int* argRiskPoint, int argIndex);
	int **AroundRisk();
	/**
	 *	@fn		Index
	 *	@brief	グリッドの縦軸と横軸のインデックス
	 *	@return	グリッドの縦軸と横軸のインデックス
	 */
	Math::Vector2 Index()const;

public:

};
