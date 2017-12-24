/**
 *	@file	AnimHandle.h
 *	@date	2017 / 9 / 27
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../stdafx.h"

struct QuatKey;
struct VectorKey;
struct QautTrack;
struct VectorTrack;
class Node;
struct BoneNode;
struct AnimSample;

/**
 *  @class	AnimHandle
 *  @brief	アニメーションの管理
 */
class AnimHandle
{
private:
	float		globalTime_;
	float		speed_;
	float		weight_;
private:
	/**
	 *	@fn			InterpolatePosition
	 *	@brief		座標の補間
	 *	@param[in]	argSample	!<	補間するアニメーション時間
	 *	@param[in]	argFrame	!<	アニメーションのフレームレート
	 *	@param[in]	argPosTrack	!<	アニメーション座標の配列
	 *	@return		補完された座標
	 */
	Math::Vector3	InterpolatePosition(float argSample, int argFrame, VectorTrack *argPosTrack);
	/**
	 *	@fn			InterpolateScale
	 *	@brief		拡縮の補間
	 *	@param[in]	argSample		!<	補間するアニメーション時間
	 *	@param[in]	argFrame		!<	アニメーションのフレームレート
	 *	@param[in]	argScaleTrack	!<	アニメーション拡縮の配列
	 *	@return		補完された拡縮
	 */
	Math::Vector3	InterpolateScale(float argSample, int argFrame, VectorTrack* argScaleTrack);
	/**
	 *	@fn			InterpolateScale
	 *	@brief		角度の補間
	 *	@param[in]	argSample		!<	補間するアニメーション時間
	 *	@param[in]	argFrame		!<	アニメーションのフレームレート
	 *	@param[in]	argScaleTrack	!<	アニメーション角度の配列
	 *	@return		補完された角度
	 */
	Math::Quaternion InterpolateRotation(float argSample, int argFrame, QautTrack* argTrack);
	/**
	 *	@fn			GetLocalTime
	 *	@brief		アニメーション時間の取得
	 *	@param[in]	argNode		!<	総経過時間
	 *	@param[in]	argDelta	!<	時間を取得したいアニメーション
	 *	@return		アニメーション時間
	 */
	float GetLocalTime(float argGlobalTime, AnimSample * argSample);
	/**
	 *	@fn			GetLocalFrame
	 *	@brief		アニメーションのフレーム数の取得
	 *	@param[in]	argSample	!<	アニメーションのフレームレート
	 *	@param[in]	argTrack	!<	アニメーション座標の配列
	 *	@return		アニメーションのフレーム数
	 */
	int GetLocalFrame(float argSample, VectorTrack *argTrack);
	/**
	 *	@fn			GetLocalFactor
	 *	@brief		正規化された1フレームの時間を取得
	 *	@param[in]	argFrame	!<	アニメーションのフレームレート
	 *	@param[in]	argSample	!<	正規化したいアニメーション
	 *	return		正規化された1フレームの時間
	 */
	float GetLocalFactor(int argFrame, AnimSample *argSample);
	/**
	 *	@fn				EvalNodes
	 *	@brief			ノードの現在のアニメーションを適用する
	 *	@param[in]		argSample			!<	アニメーションの現在時間
	 *	@param[in]		argFrame			!<	アニメーションのフレームレート
	 *	@param[in,out]	argBoneNode			!<	アニメーションするボーン
	 *	@param[in]		argParentScale		!<	親になる拡縮行列
	 *	@param[in]		argParentRotation	!<	親になる回転行列
	 */
	void EvalNodes(float argSample, int argFrame, BoneNode *argBoneNode, const Math::Matrix &argParentScale, const Math::Matrix &argParentRotation);
	/**
	 *	@fn				EvalNextNodes
	 *	@brief			ノードの次のアニメーションを適用する
	 *	@param[in]		argSample			!<	アニメーションの現在時間
	 *	@param[in]		argFrame			!<	アニメーションのフレームレート
	 *	@param[in,out]	argBoneNode			!<	アニメーションするボーン
	 *	@param[in]		argParentScale		!<	親になる拡縮行列
	 *	@param[in]		argParentRotation	!<	親になる回転行列
	 */
	void EvalNextNodes(float argSample, int argFrame, BoneNode * argBoneNode, const Math::Matrix &argParentScale, const Math::Matrix &argParentRotation);

public:
	AnimHandle();
	~AnimHandle();

public:
	/**
	 *	@fn				UpdateNodes
	 *	@brief			ノードの更新
	 *	@param[in,out]	argNode		!<	ソースになるノード
	 *	@param[in]		argDelta	!<	アニメーションに扱うデルタタイム
	 */
	void UpdateNodes(Node *argNode, const float argDelta);
	inline float Weight()const { return weight_; }
};
