/**
 *	@file	FbxModel.h
 *	@date	2017 / 9 / 27
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../stdafx.h"
#include <fbxsdk.h>

class SkinMesh;
class Node;
class AnimHandle;
class FbxParser;
struct BoneNode;

namespace
{
	static const int BoneMax = 256;
}

struct BoneBufferData
{
	BoneBufferData() 
		: Weight(1.0f),IsAnimation(true)
	{
	}
	Math::Matrix	CurrentBones[BoneMax];
	Math::Matrix	NextBones[BoneMax];
	float	Weight;
	bool	IsAnimation;
	int		padding[2];
};

class FbxModel
{
private:
	/**
	 *	@enum	eShadingMode
	 *	@brief	描画モード
	 */
	enum eShadingMode
	{
		Wireframe,
		Solid,
		CullNone,
		Max,
	};
private:
	std::unique_ptr<FbxParser>	parser_;
	std::unique_ptr<Node>				node_;
	std::vector<std::unique_ptr<SkinMesh>>			skinMesh_;
	FbxScene				*scene_;
	FbxArray<FbxString*>	animStackNameArray_;
	std::unique_ptr<AnimHandle>		handle_;
	std::unique_ptr<ConstantBuffer
		<BoneBufferData>>	cbBone_;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState_[eShadingMode::Max];
private:
	void UpdateBoneTransform(Node* argNode);
	void ProcessBoneNode(BoneNode* argRootBoneNode);
	void ProcessNextBoneNode(BoneNode* argRootBoneNode);

public:
	FbxModel(ID3D11DeviceContext *argContext, const std::string &argFilename, FbxAxisSystem argFbxAxisSystem);
	FbxModel(ID3D11DeviceContext *argContext, const std::string &argFilename, FbxAxisSystem argFbxAxisSystem, void *argMemory, int argSize);
	~FbxModel();
public:

	void Update(ID3D11DeviceContext *argContext, float argDeltaTime);
	void Draw(ID3D11DeviceContext *argContext, const DirectX::XMFLOAT4X4 &argWorld, const DirectX::XMFLOAT4X4 &argView, const DirectX::XMFLOAT4X4 &argProj);
	AnimHandle* GetHandle();
	void ChangeAnimation(int argIndex);

};


