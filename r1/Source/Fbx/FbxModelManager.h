/**
 *	@file   FbxModelManager.h
 *	@date   2017 / 10 / 13
 *	@author Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../stdafx.h"
#include "FbxModel.h"

class FbxModelManager final
{
private:
	class Impl;
	std::unique_ptr<Impl>pImpl;

public:
	FbxModelManager();
	~FbxModelManager();
public:
	void Initialie(std::string argPath, FbxAxisSystem argAxis);
	void Initialie(std::string argPath, FbxAxisSystem argAxis, bool argCanUseArchive);
	void AddModel(ID3D11DeviceContext *argContext, std::string argFilename);
	std::shared_ptr<FbxModel> FindModel(std::string argFilename)const;
	void CanUseArchive(bool argCanUseArchive);
};