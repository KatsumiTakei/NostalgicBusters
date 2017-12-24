/**
 *	@file   FbxModelManager.cpp
 *	@date   2017 / 10 / 13
 *	@author Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "FbxModelManager.h"


class FbxModelManager::Impl
{
public:
	Impl() = default;
	~Impl() = default;

private:
	std::unique_ptr<FbxModel> LoadModel(ID3D11DeviceContext *argContext, std::string argPath)
	{
		char* binData = nullptr;
		int length = 0;
		Singleton<Archive>::Get()->Read(argPath.c_str(), &binData, &length);

		return std::make_unique<FbxModel>(argContext, argPath, axis_, binData, length);
	}

public:
	void AddModel(ID3D11DeviceContext *argContext, std::string argFilename)
	{
		const std::string Path = fbxModelPath_ + argFilename + ".fbx";
		std::shared_ptr<FbxModel> fbx = (canUseArchive_) ? LoadModel(argContext, Path) : std::make_shared<FbxModel>(argContext, Path, axis_);
		fbxModels_.insert(std::make_pair(argFilename, fbx));
	}

public:

	std::map<std::string, std::shared_ptr<FbxModel>> fbxModels_;
	std::string fbxModelPath_;
	FbxAxisSystem axis_;
	bool canUseArchive_ = false;
};

FbxModelManager::FbxModelManager()
	:pImpl(std::make_unique<Impl>())
{
}

FbxModelManager::~FbxModelManager() = default;

void FbxModelManager::Initialie(std::string argPath, FbxAxisSystem argAxis)
{
	pImpl->fbxModelPath_ = argPath;
	pImpl->axis_ = argAxis;
}

void FbxModelManager::Initialie(std::string argPath, FbxAxisSystem argAxis, bool argCanUseArchive)
{
	pImpl->fbxModelPath_ = argPath;
	pImpl->axis_ = argAxis;
	pImpl->canUseArchive_ = argCanUseArchive;
}

void FbxModelManager::AddModel(ID3D11DeviceContext *argContext, std::string argFilename)
{
	pImpl->AddModel(argContext, argFilename);
}

std::shared_ptr<FbxModel> FbxModelManager::FindModel(std::string argFilename)const
{
	return pImpl->fbxModels_.at(argFilename);
}

void FbxModelManager::CanUseArchive(bool argCanUseArchive)
{
	pImpl->canUseArchive_ = argCanUseArchive;
}
