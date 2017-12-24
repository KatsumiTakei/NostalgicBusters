/**
 *	@file	ScoreManager.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../stdafx.h"

class ScoreManager final
{
private:
	class Impl;
	std::unique_ptr<Impl>pImpl;

public:
	ScoreManager();
	~ScoreManager();

public:

	bool Update();
	void ReadHiScore();
	void WritingHiScore();

public:
	unsigned int HiScore()const;
	void HiScore(unsigned int argHiScore);
	unsigned int Score()const;
	void Score(unsigned int argScore);
	void AddScore(unsigned int argAdd);

	void DataPath(std::string argPath);
	std::string DataPath()const;
	void CanUseArchive(bool argCanUseArchive);

};