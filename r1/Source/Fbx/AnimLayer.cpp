/**
 *	@file	AnimLayer.cpp
 *	@date	2017 / 9 / 27
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "AnimLayer.h"

AnimLayer::AnimLayer(FbxImporter* argImporter)
	: CurrentSampleIndex(0), NumSamples(0)
{
	for (int lAnimStackIndex = 0, lAnimStackCount = argImporter->GetAnimStackCount(); lAnimStackIndex < lAnimStackCount; ++lAnimStackIndex)
	{
		const FbxTakeInfo* Take = argImporter->GetTakeInfo(lAnimStackIndex);
		std::unique_ptr<AnimSample> sample = std::make_unique<AnimSample>();

		const FbxTime Start = Take->mLocalTimeSpan.GetStart();
		const FbxTime End = Take->mLocalTimeSpan.GetStop();
		const FbxTime Duration = Take->mLocalTimeSpan.GetDuration();
		const FbxTime Length = End.GetFrameCount(FRAME_60) - Start.GetFrameCount(FRAME_60) + 1;

		sample->Name = Take->mName.Buffer();
		sample->AnimStart = Start.GetMilliSeconds() * 0.001f;
		sample->AnimEnd = End.GetMilliSeconds() * 0.001f;
		sample->AnimDuration = (End.GetMilliSeconds() - Start.GetMilliSeconds()) * 0.001f;

		sample->FrameStart = static_cast<int>(Start.GetFrameCount(FRAME_60));
		sample->FrameEnd = static_cast<int>(End.GetFrameCount(FRAME_60));
		sample->FrameCount = static_cast<int>(Length.Get());
		sample->Index = NumSamples;

		const auto Mode = Start.GetGlobalTimeMode();
		sample->AnimFps = static_cast<float>(Start.GetFrameRate(Mode));

		SamplesNames.push_back(sample->Name.c_str());
		Samples.push_back(std::move(sample));
		NumSamples++;
	}
}