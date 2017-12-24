/**
 *	@file	FbxModel.cpp
 *	@date	2017 / 9 / 27
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "FbxModel.h"
#include "SkinMesh.h"
#include "FbxParser.h"
#include "Node.h"
#include "AnimLayer.h"
#include "AnimHandle.h"
#include "Track.h"

FbxModel::FbxModel(ID3D11DeviceContext *argContext, const std::string &argFilename, FbxAxisSystem argFbxAxisSystem)
	: handle_(std::make_unique<AnimHandle>()), node_(parser_->CreateModelNode(argFilename, &scene_, argFbxAxisSystem)),
	cbBone_(std::make_unique<ConstantBuffer<BoneBufferData>>(Utility::GetD3Ddevice(argContext).Get(), eShaderType::Vertex, 1)),
	parser_(std::make_unique<FbxParser>())
{
	for (auto Mesh : node_->MeshList())
	{//	メッシュ取得
		skinMesh_.push_back(std::make_unique<SkinMesh>(Mesh, argContext, false));
	}

	cbBone_->IsAnimation = node_->HasAnimation();
	if (cbBone_->IsAnimation)
	{
		node_->StartAnim();
	}

}

FbxModel::FbxModel(ID3D11DeviceContext * argContext, const std::string & argFilename, FbxAxisSystem argFbxAxisSystem, void * argMemory, int argSize)
	: handle_(std::make_unique<AnimHandle>()), node_(parser_->CreateModelNode(argFilename, &scene_, argFbxAxisSystem, argMemory, argSize)),
	cbBone_(std::make_unique<ConstantBuffer<BoneBufferData>>(Utility::GetD3Ddevice(argContext).Get(), eShaderType::Vertex, 1)),
	parser_(std::make_unique<FbxParser>())
{
	for (auto Mesh : node_->MeshList())
	{//	メッシュ取得
		skinMesh_.push_back(std::make_unique<SkinMesh>(Mesh, argContext, false));
	}

	cbBone_->IsAnimation = node_->HasAnimation();
	if (cbBone_->IsAnimation)
	{
		node_->StartAnim();
	}
}

FbxModel::~FbxModel() = default;

/**
 *	@fn			UpdateBoneTransform
 *	@brief		ボーンの更新
 *	@param[in]	argConext	!<	GPU連携用のコンテキスト
 *	@param[in]	argNode		!<	モデルノード
 */
void FbxModel::UpdateBoneTransform(Node *argNode)
{
	BoneNode* rootBoneNode = argNode->GetBoneNodeRoot();
	ProcessBoneNode(rootBoneNode);
	ProcessNextBoneNode(rootBoneNode);

	cbBone_->Weight = handle_->Weight();
}
/**
 *	@fn			ProcessBoneNode
 *	@brief		現在のアニメーションのボーンの更新
 *	@param[in]	argRootBoneNode	!<	ボーンのルートのノード
 */
void FbxModel::ProcessBoneNode(BoneNode * argRootBoneNode)
{
	for (BoneNode* node = argRootBoneNode; node != nullptr; node = node->next_)
	{//	定数バッファの更新
		//	HACK	:	Drawに移譲
		const auto Local = node->GlobalTransform;
		const auto LocalInv = node->InverseTransform;
		const int BoneID = node->id_;

		cbBone_->CurrentBones[BoneID] = (Local * LocalInv).Transpose();
		if (node->firstChild_)
			ProcessBoneNode(node->firstChild_);
	}
}
/**
 *	@fn			ProcessNextBoneNode
 *	@brief		次のアニメーションのボーンの更新
 *	@param[in]	argRootBoneNode	!<	ボーンのルートのノード
 */
void FbxModel::ProcessNextBoneNode(BoneNode * argRootBoneNode)
{
	for (BoneNode* node = argRootBoneNode; node != nullptr; node = node->next_)
	{//	定数バッファの更新
		//	HACK	:	Drawに移譲
		const auto Local = node->NextGlobalTransform;
		const auto LocalInv = node->InverseTransform;
		const int BoneID = node->id_;
		
		cbBone_->NextBones[BoneID] = (Local * LocalInv).Transpose();
		if (node->firstChild_)
			ProcessNextBoneNode(node->firstChild_);
	}
}

/**
 *	@fn		ChangeAnimation
 *	@brief	アニメーションの変更	
 */
void FbxModel::ChangeAnimation(int argIndex)
{
	if (!node_->HasAnimation())
		return;
	node_->ChangeAnimSample(argIndex);
}
/**
 *	@fn		Update
 *	@brief	モデルの更新
 */
void FbxModel::Update(ID3D11DeviceContext * argContext, float argDeltaTime)
{
	if (cbBone_->IsAnimation)
	{
		handle_->UpdateNodes(node_.get(), argDeltaTime);
		UpdateBoneTransform(node_.get());
	}

}
/**
 *	@fn			Draw
 *	@brief		モデルの描画
 *	@param[in]	argConext	!<	GPU連携用のコンテキスト
 */
void FbxModel::Draw(ID3D11DeviceContext *argContext, const DirectX::XMFLOAT4X4 &argWorld, const DirectX::XMFLOAT4X4 &argView, const DirectX::XMFLOAT4X4 &argProj)
{
	if (skinMesh_.empty())
		return;

	cbBone_->Update(argContext);

	for (const auto &SkinMesh : skinMesh_)
	{
		SkinMesh->CbSimpleMatrix(SimpleMatrix(argWorld, argView, argProj));
		SkinMesh->Draw(argContext);
	}
}

AnimHandle* FbxModel::GetHandle()
{
	return handle_.get();
}