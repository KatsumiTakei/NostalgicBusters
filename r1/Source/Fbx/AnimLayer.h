/**
 *	@file	AnimLayer.h
 *	@date	2017 / 9 / 27
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once
#include <fbxsdk.h>
#include "../stdafx.h"

#define FRAME_60 FbxTime::eFrames60

/**
 *  @struct AnimSample
 *  @brief	アニメーションの情報
 */
struct AnimSample
{
public:
	float AnimStart, AnimEnd, AnimDuration;
	int FrameStart, FrameEnd, FrameCount;
	float AnimFps;
	int Index;
	std::string Name;
public:
	AnimSample()
		: AnimStart(0.0f), AnimEnd(0.0f), AnimDuration(0.0f),
		FrameStart(0), FrameEnd(0), FrameCount(0), Index(0)
	{
	}
};
/**
 *  @struct AnimLayer
 *  @brief	FBX内のアニメーションをまとめた構造体
 */
struct AnimLayer
{
public:
	std::vector<std::unique_ptr<AnimSample>>	Samples;
	std::vector<const char*>	SamplesNames;
	int CurrentSampleIndex;
	int NumSamples;

public:
	AnimLayer(FbxImporter* argImporter);

public:
	inline AnimSample* GetCurrentSample()
	{
		return Samples[CurrentSampleIndex].get();
	}

	/**
	 *	@fn		ChangeAnimSample
	 *	@brief	インデックスでアニメーションを変更する
	 *	@param[in]	argIndex !<	変更したいアニメーションのインデックス
	 */
	void AnimLayer::ChangeAnimSample(int argIndex)
	{
		CurrentSampleIndex = argIndex;
	}
	/**
	 *	@fn		ChangeAnimSample
	 *	@brief	名前でアニメーションを変更する
	 *	@param[in]	argAnimName	!<	切り替えたいアニメーションの名前
	 */
	void AnimLayer::ChangeAnimSample(const char *argAnimName)
	{
		for (int lPoseIndex = 0, lAnimStackNames = SamplesNames.size(); lPoseIndex < lAnimStackNames; ++lPoseIndex)
		{
			// アニメーションスタックのインデックスを検索する
			if (SamplesNames[lPoseIndex] == argAnimName)
			{
				CurrentSampleIndex = lPoseIndex;
				break;
			}
		}
	}
};

