/**
 *	@file	AnimHandle.cpp
 *	@date	2017 / 9 / 27
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "AnimHandle.h"
#include "node.h"
#include "AnimLayer.h"
#include "track.h"

namespace
{
	static const int TrackInterval = 1;
}

AnimHandle::AnimHandle()
	: speed_(1.0f), globalTime_(0.0f),	weight_(0.0f)
{
}

AnimHandle::~AnimHandle() = default;

/**
 *	@fn				UpdateNodes
 *	@brief			ノードの更新
 *	@param[in,out]	argNode		!<	ソースになるノード
 *	@param[in]		argDelta	!<	アニメーションに扱うデルタタイム
 */
void AnimHandle::UpdateNodes(Node *argNode, const float argDelta)
{
	BoneNode* root = argNode->GetBoneNodeRoot();
	AnimSample* sample = argNode->CurrentSample();
	globalTime_ += argDelta * speed_;
	
	//	アニメーションの現在時間取得
	const float SampleTime = GetLocalTime(globalTime_, sample);
	//	アニメーションのフレームレート取得
	const int SampleFrame = GetLocalFrame(SampleTime, root->PositionTrack.get());
	AnimSample* nextSample = argNode->NextSample();

	if (!nextSample) 
	{
		weight_ = 0.0f;
	}
	if (nextSample && sample != nextSample) 
	{//	アニメーションを遷移するなら次のアニメーションの計算をする
		float nextSampleTime = GetLocalTime(globalTime_, nextSample);
		int nextSampleFrame = GetLocalFrame(nextSampleTime, root->PositionTrack.get());
		if (weight_ != 1.0f) 
		{//	補間用のWeightを更新
			weight_ = GetLocalFactor(nextSampleFrame, nextSample);
		}
		else
		{
			argNode->ChangeNextSample();
		}
		//	次のアニメーション座標を計算
		const Math::Matrix NextR, NextS;
		EvalNextNodes(nextSampleTime, nextSampleFrame, root, NextS, NextR);
	}
	//	現在のアニメーション座標を計算
	const Math::Matrix ParentS, ParentR;
	EvalNodes(SampleTime, SampleFrame, root, ParentS, ParentR);
}
/**
 *	@fn				EvalNodes
 *	@brief			ノードの現在のアニメーションを適用する
 *	@param[in]		argSample			!<	アニメーションの現在時間
 *	@param[in]		argFrame			!<	アニメーションのフレームレート
 *	@param[in,out]	argBoneNode			!<	アニメーションするボーン
 *	@param[in]		argParentScale		!<	親になる拡縮行列
 *	@param[in]		argParentRotation	!<	親になる回転行列
 */
void AnimHandle::EvalNodes(float argSample, int argFrame, BoneNode *argBoneNode, const Math::Matrix &argParentScale, const Math::Matrix &argParentRotation)
{
	for (argBoneNode; argBoneNode != nullptr; argBoneNode = argBoneNode->next_)
	{//	ボーンからアニメーション時の情報を取得
		VectorTrack* PositionTrack = argBoneNode->PositionTrack.get();
		VectorTrack* ScaleTrack = argBoneNode->ScaleTrack.get();
		QautTrack* RotationTrack = argBoneNode->RotationTrack.get();

		//	アニメーション時の情報を計算
		auto Position	= InterpolatePosition(argSample, argFrame, PositionTrack);
		auto Scale		= InterpolateScale(argSample, argFrame, ScaleTrack);
		auto Roatation	= InterpolateRotation(argSample, argFrame, RotationTrack);
		
		const Math::Matrix LocalS = Math::Matrix::CreateScale(Scale);
		const Math::Matrix LocalR = Math::Matrix::CreateFromQuaternion(Roatation).Transpose();
		
		if (argBoneNode->parent_)
		{//	ボーンに親が存在すれば親の行列と掛ける
		
			const Math::Matrix& ParentTransform = argBoneNode->parent_->GlobalTransform;
			Math::Matrix globalRS;
			switch (argBoneNode->HasInheritScale)
			{
			case BoneNode::eInheritType::Rrs:
				globalRS = argParentRotation * LocalR * LocalS;
				break;
			case BoneNode::eInheritType::RrSs:
				globalRS = argParentRotation * LocalR * argParentScale * LocalS;
				break;
			case BoneNode::eInheritType::RSrs:
				globalRS = argParentRotation * argParentScale * LocalR * LocalS;
				break;
			}
			//	絶対座標計算
			const Math::Vector3 GlobalPosion = Math::Vector3::MultiplyMatrix(Position, ParentTransform);
			const Math::Matrix GlobalT = Math::Matrix::CreateTranslation(GlobalPosion).Transpose();
			argBoneNode->GlobalTransform = GlobalT * globalRS;

			if (argBoneNode->firstChild_)
			{//	子の行列に影響を与える
				const Math::Matrix GlobalS = argParentScale * LocalS;
				const Math::Matrix GlobalR = argParentRotation * LocalR;

				EvalNodes(argSample, argFrame, argBoneNode->firstChild_, GlobalS, GlobalR);
			}
		}
		else
		{
			//	絶対座標計算
			const Math::Matrix LocalT = Math::Matrix::CreateTranslation(Position).Transpose();
			argBoneNode->GlobalTransform = LocalT * LocalR * LocalS;

			if (argBoneNode->firstChild_)
			{//	子の行列を計算
				EvalNodes(argSample, argFrame, argBoneNode->firstChild_, LocalS, LocalR);
			}
		}
	}
}
/**
 *	@fn				EvalNextNodes
 *	@brief			ノードの次のアニメーションを適用する
 *	@param[in]		argSample			!<	アニメーションの現在時間
 *	@param[in]		argFrame			!<	アニメーションのフレームレート
 *	@param[in,out]	argBoneNode			!<	アニメーションするボーン
 *	@param[in]		argParentScale		!<	親になる拡縮行列
 *	@param[in]		argParentRotation	!<	親になる回転行列
 */
void AnimHandle::EvalNextNodes(float argSample, int argFrame, BoneNode * argBoneNode, const Math::Matrix &argParentScale, const Math::Matrix &argParentRotation)
{
	for (argBoneNode; argBoneNode != nullptr; argBoneNode = argBoneNode->next_)
	{//	ボーンからアニメーション時の情報を取得
		VectorTrack* PositionTrack = argBoneNode->PositionTrack.get();
		VectorTrack* ScaleTrack = argBoneNode->ScaleTrack.get();
		QautTrack* RotationTrack = argBoneNode->RotationTrack.get();

		//	アニメーション時の情報を計算
		auto Position = InterpolatePosition(argSample, argFrame, PositionTrack);
		auto Scale = InterpolateScale(argSample, argFrame, ScaleTrack);
		auto Roatation = InterpolateRotation(argSample, argFrame, RotationTrack);

		const Math::Matrix LocalS = Math::Matrix::CreateScale(Scale);
		const Math::Matrix LocalR = Math::Matrix::CreateFromQuaternion(Roatation).Transpose();
		
		if (argBoneNode->parent_)
		{//	ボーンに親が存在すれば親の行列と掛ける
			const Math::Matrix& ParentTransform = argBoneNode->parent_->NextGlobalTransform;
			Math::Matrix globalRS;
			switch (argBoneNode->HasInheritScale)
			{
			case BoneNode::eInheritType::Rrs:
				globalRS = argParentRotation * LocalR * LocalS;
				break;
			case BoneNode::eInheritType::RrSs:
				globalRS = argParentRotation * LocalR * argParentScale * LocalS;
				break;
			case BoneNode::eInheritType::RSrs:
				globalRS = argParentRotation * argParentScale * LocalR * LocalS;
				break;
			}
			//	絶対座標計算
			const Math::Vector3 GlobalPosion = Math::Vector3::MultiplyMatrix(Position, ParentTransform);
			const Math::Matrix GlobalT = Math::Matrix::CreateTranslation(GlobalPosion).Transpose();
			argBoneNode->NextGlobalTransform = GlobalT * globalRS;

			if (argBoneNode->firstChild_)
			{//	子の行列に影響を与える
				const Math::Matrix GlobalS = argParentScale * LocalS;
				const Math::Matrix GlobalR = argParentRotation * LocalR;

				EvalNextNodes(argSample, argFrame, argBoneNode->firstChild_, GlobalS, GlobalR);
			}
		}
		else
		{
			//	絶対座標計算
			const Math::Matrix LocalT = Math::Matrix::CreateTranslation(Position).Transpose();
			argBoneNode->NextGlobalTransform = LocalT * LocalR * LocalS;

			if (argBoneNode->firstChild_)
			{//	子の行列を計算
				EvalNextNodes(argSample, argFrame, argBoneNode->firstChild_, LocalS, LocalR);
			}
		}

	}
}
/**
 *	@fn			InterpolatePosition
 *	@brief		座標の補間
 *	@param[in]	argSample	!<	補間するアニメーション時間
 *	@param[in]	argFrame	!<	アニメーションのフレームレート
 *	@param[in]	argPosTrack	!<	アニメーション座標の配列
 *	@return		補完された座標
 */
Math::Vector3 AnimHandle::InterpolatePosition(float argSample, int argFrame, VectorTrack *argPosTrack)
{
	//	アニメーション時の座標取得
	const VectorKey& Current = argPosTrack->Keys[argFrame];
	const VectorKey& Next = argPosTrack->Keys[argFrame + TrackInterval];
	
	//	補間係数取得
	const float Factor = Utility::Math::Normalize(Current.Time, Next.Time, argSample);
	//	補間
	Math::Vector3 Position = Math::Vector3::Lerp(Current.VecKey, Next.VecKey, Factor);
	return Position;
}
/**
 *	@fn			InterpolateScale
 *	@brief		拡縮の補間
 *	@param[in]	argSample		!<	補間するアニメーション時間
 *	@param[in]	argFrame		!<	アニメーションのフレームレート
 *	@param[in]	argScaleTrack	!<	アニメーション拡縮の配列
 *	@return		補完された拡縮
 */
Math::Vector3 AnimHandle::InterpolateScale(float argSample, int argFrame, VectorTrack* argScaleTrack)
{
	//	アニメーション時の拡縮取得
	const VectorKey& Current = argScaleTrack->Keys[argFrame];
	const VectorKey& Next = argScaleTrack->Keys[argFrame + TrackInterval];

	//	補間係数取得
	const float Factor = Utility::Math::Normalize(Current.Time, Next.Time, argSample);

	//	補間
	const Math::Vector3 Scale = Math::Vector3::Lerp(Current.VecKey, Next.VecKey, Factor);
	return Scale;
}
/**
 *	@fn			InterpolateScale
 *	@brief		角度の補間
 *	@param[in]	argSample		!<	補間するアニメーション時間
 *	@param[in]	argFrame		!<	アニメーションのフレームレート
 *	@param[in]	argScaleTrack	!<	アニメーション角度の配列
 *	@return		補完された角度
 */
Math::Quaternion AnimHandle::InterpolateRotation(float argSample, int argFrame,QautTrack* argTrack)
{
	//	アニメーション時の角度取得
	const QuatKey &Current = argTrack->Keys[argFrame];
	const QuatKey &Next = argTrack->Keys[argFrame + TrackInterval];
	//	補間係数取得
	const float Factor = Utility::Math::Normalize(Current.Time, Next.Time, argSample);

	//	補間
	Math::Quaternion rotation;
	Math::Quaternion::Interpolate(&rotation, Current.QautKey, Next.QautKey, Factor);
	rotation.Normalize();
	return rotation;
}
/**
 *	@fn			GetLocalTime
 *	@brief		アニメーション時間の取得
 *	@param[in]	argNode		!<	総経過時間
 *	@param[in]	argDelta	!<	時間を取得したいアニメーション
 *	@return		アニメーション時間
 */
float AnimHandle::GetLocalTime(float argGlobalTime, AnimSample *argSample)
{
	return fmod(argGlobalTime, argSample->AnimDuration) + argSample->AnimStart;
}
/**
 *	@fn			GetLocalFrame
 *	@brief		アニメーションのフレーム数の取得
 *	@param[in]	argSample	!<	アニメーションのフレームレート
 *	@param[in]	argTrack	!<	アニメーション座標の配列
 *	@return		アニメーションのフレーム数
 */
int AnimHandle::GetLocalFrame(float argSample, VectorTrack *argTrack)
{
	int frame = 0;
	for (int lKeyIndex = 0, lKeyMax = argTrack->KeyNums - 1; lKeyIndex < lKeyMax; ++lKeyIndex)
	{
		if (argSample < argTrack->Keys[lKeyIndex + 1].Time)
		{
			frame = lKeyIndex;
			break;
		}
	}
	return frame;
}
/**
 *	@fn			GetLocalFactor
 *	@brief		正規化された1フレームの時間を取得
 *	@param[in]	argFrame	!<	アニメーションのフレームレート
 *	@param[in]	argSample	!<	正規化したいアニメーション
 *	return		正規化された1フレームの時間
 */
float AnimHandle::GetLocalFactor(int argFrame, AnimSample *argSample)
{
	const float FrameStart = static_cast<float>(argFrame);
	const float FrameEnd = static_cast<float>(argSample->FrameStart + argSample->FrameCount -3);
	const float Result = Utility::Math::Normalize(0.0f, FrameEnd, FrameStart);
	return Result;
}