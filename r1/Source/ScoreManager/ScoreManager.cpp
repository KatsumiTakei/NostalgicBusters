/**
 *	@file	ScoreManager.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "ScoreManager.h"
#include "../Config.h"

class ScoreManager::Impl
{
public:
	Impl()
		: score_(0), hiScore_(0),canUseArchive_(false)
	{
	}
	~Impl() = default;
public:
	bool Update()
	{
		if (score_ > hiScore_)
		{
			hiScore_ = score_;
		}
		return true;
	}

public:
	unsigned int score_;
	unsigned int hiScore_;
	std::string dataPath_;
	bool canUseArchive_;
};

ScoreManager::ScoreManager()
	:pImpl(std::make_unique<Impl>())
{
}

ScoreManager::~ScoreManager() = default;

bool ScoreManager::Update()
{
	return pImpl->Update();
}

void ScoreManager::ReadHiScore()
{
	//	HACK	:	キャラとか難易度によって変える
	std::string path = pImpl->dataPath_ + "HiScore.dat";
	std::fstream fs(path, std::ios::binary | std::ios::in);
	assert(fs && "read hiscore file failed...");
	
	fs.read(reinterpret_cast<char*>(&pImpl->hiScore_), sizeof(unsigned int));
	fs.close();
}
void ScoreManager::WritingHiScore()
{
	//	HACK	:	キャラとか難易度によって変える
	std::string path = pImpl->dataPath_ + "HiScore.dat";
	std::ofstream ofs(path, std::ios::binary | std::ios::out);
	assert(ofs && "write hiscore file failed...");

	ofs.write(reinterpret_cast<char*>(&pImpl->hiScore_), sizeof(unsigned int));
	ofs.close();
}
unsigned int ScoreManager::HiScore()const
{
	return pImpl->hiScore_;
}
void ScoreManager::HiScore(unsigned int argHiScore)
{
	pImpl->hiScore_ = argHiScore;
}
unsigned int ScoreManager::Score()const
{ 
	return pImpl->score_; 
}
void ScoreManager::Score(unsigned int argScore)
{ 
	pImpl->score_ = argScore;
}

void ScoreManager::AddScore(unsigned int argAdd)
{
	pImpl->score_ += argAdd;
}

void ScoreManager::DataPath(std::string argPath)
{
	pImpl->dataPath_ = argPath;
}

std::string ScoreManager::DataPath() const
{
	return pImpl->dataPath_;
}

void ScoreManager::CanUseArchive(bool argCanUseArchive)
{
	pImpl->canUseArchive_ = argCanUseArchive;
}

