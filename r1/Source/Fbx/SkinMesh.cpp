/**
 *	@file	SkinMesh.cpp
 *	@date	2017 / 9 / 27
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "SkinMesh.h"
#include "Node.h"
#include "AnimLayer.h"
#include "Track.h"

SkinMesh::SkinMesh(MeshNode *argMeshNode, ID3D11DeviceContext *argContext, bool argIsRowMat)
	:meshNode_(argMeshNode)
{
	//assert(meshNode_->id_ > 0 && "dosent supported 2more meshes in this version");
	//	TODO	:	複数メッシュ未対応
	for (int faceIndex = 0, faceMax = meshNode_->Faces.size(); faceIndex < faceMax; ++faceIndex)
	{
		for (int facePointIndex = 0; facePointIndex < Face::ComponentNum; ++facePointIndex)
		{
			//	頂点情報取得
			const Face FaceNode = meshNode_->Faces[faceIndex];
			const int vertexIndex = FaceNode.VerticeIndex[facePointIndex];
			SkinVertex vertex;
			vertex.Position = meshNode_->Points[vertexIndex]->Position;
			vertex.Normal = FaceNode.Normals[facePointIndex];
			vertex.Uv = FaceNode.UVs[facePointIndex];

			if (!argIsRowMat)
			{//	モデルの行列によって変換
				vertex.Position = -vertex.Position;
				vertex.Normal = -vertex.Normal;
				vertex.Uv = -vertex.Uv;
			}

			const float* weights = meshNode_->Points[vertexIndex]->BoneWeights;
			const unsigned int* ids = meshNode_->Points[vertexIndex]->BoneIDs;
			for (int i = 0, lBoneMax = meshNode_->Points[vertexIndex]->BoneComponentNum; i < lBoneMax; ++i)
			{//	ウェイト取得
				vertex.BoneWeight[i] = weights[i];
				vertex.BoneID[i] = ids[i];
			}

			//	バッファの要素の追加
			auto vertices = vertices_.Vertices();
			vertices.push_back(vertex);
			vertices_.Vertices(vertices);
			auto indices = indices_.Indices();
			indices.push_back(faceIndex * 3 + facePointIndex);
			indices_.Indices(indices);

		}
	}

	meshNode_->Faces.clear();
	meshNode_->Points.clear();

	CreateBuffer(argContext);

	for (const auto &Mat : meshNode_->MaterialArray)
	{//	マテリアルのセット
		Mat->Setup(argContext);
	}
}

SkinMesh::~SkinMesh() = default;
/**
 *	@fn			CreateBuffer
 *	@brief		GPUバッファの作成
 *	@param[in]	argContext	!<	コンスタントバッファ作成用のコンテキスト
 */
void SkinMesh::CreateBuffer(ID3D11DeviceContext *argContext)
{
	vertexShader_ = VertexShader(argContext, &inputLayout_, "FbxDrawerVS");
	pixelShader_ = PixelShader(argContext, "FbxDrawerPS");
	auto device = Utility::GetD3Ddevice(argContext).Get();
	cbFbxMatrix_ = std::make_unique<ConstantBuffer<SimpleMatrix>>(device, eShaderType::Vertex, 0);

	vertices_.Create(device);
	indices_.Create(device);
}
/**
 *	@fn			Draw
 *	@brief		メッシュの描画
 *	@param[in]	argContext	!<	GPU連携用のコンテキスト
 */
void SkinMesh::Draw(ID3D11DeviceContext *argContext)
{
	//	TODO	:	複数メッシュ未対応
	vertices_.Update(argContext);
	cbFbxMatrix_->Update(argContext);
	size_t indexOffset = 0;
	
	//	HACK	:	テクスチャに依存しているのでマテリアルから更新回数を割り出す
	for (int lMatIndex = 0, lMatCount = (meshNode_->MaterialArray.size() > 0) ? meshNode_->MaterialArray.size() : 1; lMatIndex < lMatCount; lMatIndex++)
	{
		meshNode_->MaterialArray[lMatIndex]->Draw(argContext);
		vertexShader_.SetShader(argContext);
		pixelShader_.SetShader(argContext);

		const UINT Stride = sizeof(SkinVertex);
		const UINT Offset = 0;

		inputLayout_.SetInputLayout(argContext);
		vertices_.SetVertexBuffers(argContext);
		argContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		indices_.SetIndexBuffer(argContext);

		const size_t ElementCount = indices_.Indices().size() / lMatCount;
		if (lMatIndex != 0)
		{//	HACK	:	マテリアルからメッシュの更新回数をとる
			indexOffset += ElementCount;
		}
		argContext->DrawIndexed(ElementCount, indexOffset, 0);
	}
}

SimpleMatrix SkinMesh::CbSimpleMatrix()const 
{
	return SimpleMatrix(cbFbxMatrix_->World, cbFbxMatrix_->View, cbFbxMatrix_->Proj);
}

void SkinMesh::CbSimpleMatrix(SimpleMatrix argSimpleMatrix)
{
	cbFbxMatrix_->World = argSimpleMatrix.World;
	cbFbxMatrix_->View = argSimpleMatrix.View;
	cbFbxMatrix_->Proj = argSimpleMatrix.Proj;
}
