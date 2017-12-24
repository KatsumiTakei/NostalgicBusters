/**
 *	@file	ReplayRecorder.cpp
 *	@date	2017 / 7 / 28
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "ReplayRecorder.h"
#include "../Player/Player.h"
#include "../ScoreManager/ScoreManager.h"
#include "../Config.h"
#include "../Input/Input.h"

namespace
{
	const int RecTime = 20;
}


ReplayRecorder::ReplayRecorder(Player *argPlayer)
 : player_(argPlayer)
{
}

ReplayRecorder::~ReplayRecorder() = default;

void ReplayRecorder::Reset(int argCount, double argPower)
{
	Initialize();
	recData_[0].Count = argCount;
	recData_[0].Score = Singleton<ScoreManager>::Get()->Score();
	recData_[0].HiScore = Singleton<ScoreManager>::Get()->HiScore();
	recData_[0].Power = argPower;
	isPlaying_ = true;
}

void ReplayRecorder::Initialize()
{	
	//	HACK:		技術アピール用、就活が終わったら変える
	//	@section	mallocによる動的確保
	recData_ = static_cast<lpRecData>(malloc(sizeof(RecData) * FlameTime * RecTime));
	recFrame_ = 0;
}

void ReplayRecorder::Update()
{
	// キーの保存
	recData_[recFrame_].IsShot = Singleton<InputManager>::Get()->IsPressed(Input::Shot);

	recData_[recFrame_].PosX = player_->Pos().x;
	recData_[recFrame_].PosY = player_->Pos().y;

	recFrame_++;

	if (recFrame_ == FlameTime * RecTime)
	{
		Finalize();
	}
}

void ReplayRecorder::Draw()
{
	wchar_t *Str = (isPlaying_) ? L"動作を保存中です" : L"リプレイの保存が可能です";
	Singleton<GraphManager>::Get()->DrawAndWriteText(Str, Math::Vector2());
}

void ReplayRecorder::Finalize()
{
	isPlaying_ = false;
	//	HACK:		技術アピール用、就活が終わったらofstreamに変える
	//	@section	ファイルの書き込み

	//	HACK	:	デモデータ以外にも対応
	auto path = (dataPath_ + "DemoReplay.dat");
	FILE *fp = nullptr;
	errno_t err = fopen_s(&fp, path.c_str(), "wb");
	assert(err == 0 && "File Open Fialed");
	assert(fp && "File Writing Fialed");
	
	fwrite(&recFrame_, sizeof(int), 1, fp);
	fwrite(recData_, sizeof(RecData) * recFrame_, 1, fp);
	
	fclose(fp);

	free(recData_);
}