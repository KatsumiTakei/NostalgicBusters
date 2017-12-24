/**
 *	@file	Track.h
 *	@date	2017 / 9 / 27
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../stdafx.h"

/**
 *  @struct	AnimKey
 *  @brief	アニメのキー
 */
struct AnimKey
{
public:
	float Time;
public:
	/**
	 *  @struct		AnimKey
	 *  @brief		アニメのキー
	 *  @param[in]	argTime	!<	登録するアニメーション時間
	 */
	AnimKey(float argTime)
		: Time(argTime)
	{
	}
	virtual ~AnimKey() = default;
};
/**
 *  @struct	VectorKey
 *  @brief	アニメの大きさと座標のキー
 */
struct VectorKey : public AnimKey
{
public:
	Math::Vector3 VecKey;
public:
	/**
	 *  @struct	VectorKey
	 *  @brief	アニメの大きさと座標のキー
	 */
	VectorKey()
		: AnimKey(0)
	{
	}
	/**
	 *  @struct		VectorKey
	 *  @brief		アニメの大きさと座標のキー
	 *  @param[in]	argX	!<	要素x
	 *  @param[in]	argY	!<	要素y
	 *  @param[in]	argZ	!<	要素z
	 *  @param[in]	argTime	!<	登録するアニメーション時間
	 */
	VectorKey(float argX, float argY, float argZ, float argTime)
		: VecKey(argX, argY, argZ), AnimKey(argTime)
	{
	}
	/**
	 *  @struct		VectorKey
	 *  @brief		アニメの大きさと座標のキー
	 *  @param[in]	argOther	!<	要素
	 *  @param[in]	argTime		!<	登録するアニメーション時間
	 */
	VectorKey(const Math::Vector3 &argOther, float argTime)
		: VecKey(argOther), AnimKey(argTime)
	{
	}

};
/**
 *  @struct	QuatKey
 *  @brief	アニメの角度キー
 */
struct QuatKey : public AnimKey
{
public:
	Math::Quaternion QautKey;
public:
	/**
	 *  @struct	QuatKey
	 *  @brief	アニメの角度キー
	 */
	QuatKey()
		: AnimKey(0)
	{
	}
	/**
	 *  @struct		QuatKey
	 *  @brief		アニメの角度キー
	 *  @param[in]	argX	!<	要素x
	 *  @param[in]	argY	!<	要素y
	 *  @param[in]	argZ	!<	要素z
	 *  @param[in]	argW	!<	要素w
	 *  @param[in]	argTime	!<	登録するアニメーション時間
	 */
	QuatKey(float argX, float argY, float argZ, float argW, float argTime)
		: QautKey(argX, argY, argZ, argW), AnimKey(argTime)
	{
	}
	/**
	 *  @struct		VectorKey
	 *  @brief		アニメの大きさと座標のキー
	 *  @param[in]	argOther	!<	要素
	 *  @param[in]	argTime		!<	登録するアニメーション時間
	 */
	QuatKey(const Math::Quaternion &argOther, float argTime)
		: QautKey(argOther), AnimKey(argTime)
	{
	}

};
/**
 *  @struct	VectorTrack
 *  @brief	アニメの大きさと座標のキー
 */
struct VectorTrack
{
public:
	int KeyNums;
	std::vector<VectorKey> Keys;
public:
	/**
	 *  @struct		VectorTrack
	 *  @brief		アニメの大きさと座標の保存
	 *  @param[in]	argTotalKeyNum	!<	アニメーションキーの数
	 */
	VectorTrack(const int argTotalKeyNum) 
		: KeyNums(argTotalKeyNum)
	{
		Keys.reserve(argTotalKeyNum);
	}
};
/**
 *  @struct	QuatKey
 *  @brief	アニメの角度キー
 */
struct QautTrack
{
public:
	int KeyNums;
	std::vector<QuatKey> Keys;
public:
	/**
	 *  @struct		QuatKey
	 *  @brief		アニメの角度キー
	 *	@param[in]	argTotalKeyNum	!<	アニメーションキーの数
	 */
	QautTrack(const int argTotalKeyNum)
		: KeyNums(argTotalKeyNum)
	{
		Keys.reserve(argTotalKeyNum);
	}

};