/**
 *	@file	SkinMesh.h
 *	@date	2017 / 9 / 27
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../stdafx.h"

struct MeshNode;
struct SurfaceMaterial;

/**
 *  @struct	SkinVertex
 *  @brief	定数バッファ頂点構造体
 */
struct SkinVertex
{
public:
	Math::Vector3 Position;
	Math::Vector3 Normal;
	Math::Vector2 Uv;
	float	BoneWeight[4];
	int		BoneID[4];
};

class SkinMesh
{
private:
	/**
	 *	@enum	eVBO
	 *	@brief	頂点バッファの種類
	 */
	enum eVBO
	{
		Vertex,
		Index,
		Max,
	};
private:
	IndexBuffer										indices_;
	InputLayout										inputLayout_;
	VertexBuffer<SkinVertex>						vertices_;
	std::unique_ptr<ConstantBuffer<SimpleMatrix>>	cbFbxMatrix_;
	MeshNode										*meshNode_;
	VertexShader									vertexShader_;
	PixelShader										pixelShader_;
private:
	void CreateBuffer(ID3D11DeviceContext *argContext);

public:
	/**
	 *  @class		SkinMesh
	 *  @brief		FBXのメッシュクラス
	 *	@param[in]	argMeshNode	!<	メッシュノード
	 *	@param[in]	argContext	!<	コンテキスト
	 *	@param[in]	argIsRowMat	!<	行列が行優先か
	 */
	SkinMesh(MeshNode *argMeshNode, ID3D11DeviceContext *argContext, bool argIsRowMat = true);
	~SkinMesh();

public:
	void Draw(ID3D11DeviceContext *argContext);
public:
	SimpleMatrix CbSimpleMatrix()const;
	void CbSimpleMatrix(SimpleMatrix argSimpleMatrix);

};

