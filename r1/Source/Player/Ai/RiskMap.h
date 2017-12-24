/**
 *	@file	RiskMap.h
 *	@date	2017 / 11 / 13
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../../stdafx.h"

class RiskGrid;

namespace
{
	static const int MovePattern = 3;
	static const RECT Area = { 0,0,640,480 };
	static const Math::Vector2 GridSize = Math::Vector2(Area.right * 0.1f, Area.bottom * 0.1f);
}

class RiskMap
{
private:
	static const int GridMax = 10;
	//	!<	@attention	unique_ptrはoperatorが重いので生ポインタ
	RiskGrid *grids_[GridMax][GridMax];
	std::shared_ptr<Texture> graph_;
	RECT gridSizeList[GridMax][GridMax];

private:

	void Reset();
	
	/**
	 *	@fn			SetAroundGrid
	 *	@brief		指定のグリッドの周囲の設定
	 *	@param[in]	argGrid	!<	中心のグリッド
	 */
	void SetAroundGrid(RiskGrid *argGrid)const;
	/**
	 *	@fn			IsInGrid
	 *	@brief		座標が指定のグリッドン内に存在しているか
	 *	@param[in]	argPos		!<	座標
	 *	@param[in]	argIndexX	!<	グリッド番号x
	 *	@param[in]	argIndexY	!<	グリッド番号y
	 *	@retval		true		!<	
	 *	@retval		false		!<	
	 */
	bool IsInGrid(Math::Vector2 argPos, int argIndexX, int argIndexY) const;

	/**
	 *	@fn			IsEdge
	 *	@brief		指定のインデックスのグリッドが隅かどうか
	 *	@param[in]	argIndexX	!<	x軸
	 *	@param[in]	argIndexY	!<	y軸
	 *	@retval		true		!<	隅である
	 *	@retval		false		!<	隅でない
	 */
	bool IsEdge(int argIndexX, int argIndexY);
	/**
	 *	@fn			IsCornersPoint
	 *	@brief		指定のインデックスのグリッドが四隅かどうか
	 *	@param[in]	argIndexX	!<	x軸
	 *	@param[in]	argIndexY	!<	y軸
	 *	@retval		true	!<	四隅である
	 *	@retval		false	!<	四隅でない
	 */
	bool IsCornersPoint(int argIndexX, int argIndexY);
	

public:
	RiskMap();
	~RiskMap();

public:

	void RefreshRiskPoint();

	/**
	 *	@fn			IsEdge
	 *	@brief		指定の座標が隅に存在するか
	 *	@param[in]	argPos	!<	座標
	 *	@retval		true	!<	隅に存在する
	 *	@retval		false	!<	隅に存在しない
	 */
	bool IsEdge(Math::Vector2 argPos);
	/**
	 *	@fn			IsCornersPoint
	 *	@brief		指定の座標が四隅に存在するか
	 *	@param[in]	argPos	!<	座標
	 *	@retval		true	!<	四隅に存在する
	 *	@retval		false	!<	四隅に存在しない
	 */
	bool IsCornersPoint(Math::Vector2 argPos);
	/**
	 *	@fn			CurrentGrid
	 *	@brief		指定の座標が所属しているグリッドの取得
	 *	@param[in]	argPos		!<	座標
	 *	@return		グリッド
	 */
	RiskGrid* CurrentGrid(Math::Vector2 argPos)const;
	/**
	 *	@fn			CurrentIndex
	 *	@brief		指定の座標のグリッドのインデックスを取得
	 *	@param[in]	argPos	!<	座標
	 *	@return		指定の座標のグリッドのインデックス
	 */
	Math::Vector2 CurrentIndex(Math::Vector2 argPos)const;

	/**
	 *	@fn			InObstacle
	 *	@brief		危険度の上昇
	 *	@param[in]	argPos			!<	座標
	 *	@retval		現在のグリッド	!<	範囲内
	 *	@retval		nullptr			!<	範囲外
	 */
	RiskGrid *InObstacle(Math::Vector2 argPos);
	/**
	 *	@fn			GetGridIndex
	 *	@brief		グリッドの番号からグリッドを取得する
	 *	@param[in]	argIndexI	!<	横のグリッド番号
	 *	@param[in]	argIndexJ	!<	縦のグリッド番号
	 *	@return		グリッド
	 */
	RiskGrid* GetGridIndex(int argIndexI, int argIndexJ)const;
	/**
	 *	@fn			GetGridIndex
	 *	@brief		グリッドの番号からグリッドを取得する
	 *	@param[in]	argIndex	!<	グリッド番号
	 *	@return		グリッド
	 */
	RiskGrid* GetGridIndex(Math::Vector2 argIndex)const;
	/**
	 *	@fn			SearchSafeGrid
	 *	@brief		現在地と周囲8方向のグリッドから安全なグリッドを取得
	 *	@param[in]	argPos	!<	現在地
	 *	@return		グリッド
	 */
	RiskGrid* SearchSafeGrid(Math::Vector2 argPos)const;
	/**
	 *	@fn			SearchSafeGrid
	 *	@brief		現在地から指定のマスまでの全グリッドから安全なグリッドを取得
	 *	@param[in]	argPos		!<	現在地
	 *	@param[in]	argIndex	!<	どのぐらい離れているか
	 *	@return		グリッド
	 */
	RiskGrid* SearchSafeGrid(Math::Vector2 argPos, int argRange)const;

	/**
	 *	@fn			IsInAround
	 *	@brief		オブジェクトが周囲に存在するか
	 *	@param[in]	argSrc	!<	中心グリッド
	 *	@param[in]	argTar	!<	比較対象のグリッド
	 *	@retval		true	!<	argSrc周囲８マスにargTarが存在する
	 *	@retval		false	!<	argSrc周囲８マスにargTarが存在しない
	 */
	bool IsInAround(RiskGrid* argSrc, RiskGrid* argTar);
	/**
	 *	@fn			IsInAroundOrSame
	 *	@brief		オブジェクトが周囲または同じマスに存在するか
	 *	@param[in]	argSrc	!<	中心グリッド
	 *	@param[in]	argTar	!<	比較対象のグリッド
	 *	@retval		true	!<	argSrcと同じマスか周囲８マスにargTarが存在する
	 *	@retval		false	!<	argSrcと同じマスか周囲８マスにargTarが存在しない
	 */
	bool IsInAroundOrSame(RiskGrid* argSrc, RiskGrid* argTar);

	void Draw()const;


public:

};
