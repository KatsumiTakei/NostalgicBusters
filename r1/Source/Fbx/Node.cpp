/**
 *	@file	Node.cpp
 *	@date	2017 / 9 / 27
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "Node.h"
#include "AnimLayer.h"
#include "Track.h"

/**
 *	@fn			AllocateTracks
 *	@brief		アニメーショントラックの生成
 *	@param[in]	argFrameNum	!<	アニメーションの総フレーム数
 */
void BoneNode::AllocateTracks(int argFrameNum)
{
	PositionTrack = std::make_unique<VectorTrack>(argFrameNum);
	ScaleTrack = std::make_unique<VectorTrack>(argFrameNum);
	RotationTrack = std::make_unique<QautTrack>(argFrameNum);
	AllocatedSize = argFrameNum;
}
/**
 *	@fn			AddPositionKey
 *	@brief		アニメーション座標の登録
 *	@param[in]	argKey	!<	アニメーションの座標
 */
void BoneNode::AddPositionKey(const VectorKey &argKey)
{
	PositionTrack->Keys.push_back(argKey);
}
/**
 *	@fn			AddScaleKey
 *	@brief		アニメーション拡縮の登録
 *	@param[in]	argKey	!<	アニメーションの拡縮
 */
void BoneNode::AddScaleKey(const VectorKey &argKey)
{
	ScaleTrack->Keys.push_back(argKey);
}
/**
 *	@fn			AddRotationKey
 *	@brief		アニメーションの角度の登録
 *	@param[in]	argKey	!<	アニメーションの角度
 */
void BoneNode::AddRotationKey(const QuatKey &argKey)
{
	RotationTrack->Keys.push_back(argKey);
}

MaterialNode::MaterialNode(FbxSurfaceMaterial* argFbxSurfaceMaterial)
{
	SetMaterialProperty(argFbxSurfaceMaterial, FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissiveFactor, &Surface.Emmisive, &TexName[eTexProperty::Emissive]);
	SetMaterialProperty(argFbxSurfaceMaterial, FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor, &Surface.Ambient, &TexName[eTexProperty::Ambient]);
	SetMaterialProperty(argFbxSurfaceMaterial, FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor, &Surface.Diffuse, &TexName[eTexProperty::Diffuse]);
	SetMaterialProperty(argFbxSurfaceMaterial, FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor, &Surface.Specular, &TexName[eTexProperty::Specular]);

	Surface.TransparencyFactor = 1.f;
	const auto TransparencyFactorProperty = argFbxSurfaceMaterial->FindProperty(FbxSurfaceMaterial::sTransparencyFactor);
	if (TransparencyFactorProperty.IsValid())
	{
		Surface.TransparencyFactor = static_cast<float>(TransparencyFactorProperty.Get<FbxDouble>());
	}

	Surface.Shininess = 1.f;
	const auto ShininessProperty = argFbxSurfaceMaterial->FindProperty(FbxSurfaceMaterial::sShininess);
	if (ShininessProperty.IsValid())
	{
		Surface.Shininess = static_cast<float>(ShininessProperty.Get<FbxDouble>());
	}

	if (argFbxSurfaceMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId))
	{
		Type = eMaterialType::Lambert;
	}
	else if (argFbxSurfaceMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
	{
		Type = eMaterialType::Phong;
	}
}
/**
 *	@fn			SetMaterialProperty
 *	@brief		特定のプロパティ値と接続されているテクスチャがあればそれを取得
 *	@param[in]	argMaterial				!<	取得したい値が存在するマテリアル
 *	@param[in]	argPropertyName			!<	プロパティの名前
 *	@param[in]	argFactorPropertyName	!<	ファクターの名前
 *	@param[in]	argElement				!<	プロパティを格納するポインタ
 *	@param[in]	argTextureName			!<	テクスチャ名を格納するポインタ
 */
void  MaterialNode::SetMaterialProperty(const FbxSurfaceMaterial * argMaterial, const char *argPropertyName, const char *argFactorPropertyName, Math::Vector3* argElement, std::wstring *argTextureName)
{
	FbxDouble3 result(0, 0, 0);
	const auto Property = argMaterial->FindProperty(argPropertyName);
	const auto FactorProperty = argMaterial->FindProperty(argFactorPropertyName);

	if (Property.IsValid())
	{
		if (FactorProperty.IsValid())
		{
			result = Property.Get<FbxDouble3>();
			double factor = FactorProperty.Get<FbxDouble>();
			if (factor != 1)
			{
				result[0] *= factor;
				result[1] *= factor;
				result[2] *= factor;
			}
		}
		const int TextureCount = Property.GetSrcObjectCount<FbxFileTexture>();
		if (TextureCount)
		{
			const FbxFileTexture* Texture = Property.GetSrcObject<FbxFileTexture>();
			if (Texture)
			{
				// 絶対パスのテクスチャ名をロード
				const FbxString FilePath = Texture->GetFileName();

				// 拡張子とテクスチャ名のみに変更
				const std::wstring FileName = String::ToWstring(String::GetFileName(FilePath.Buffer()));

				//	パスと結合
				*argTextureName = Singleton<GraphManager>::Get()->Graphic2D()->ImagePath() + FileName;

				Debug::OutputDebugStringFormat(L"Load UV Name : " + *argTextureName + L"\n");
			}
		}
	}

	argElement->x = static_cast<float>(result[0]);
	argElement->y = static_cast<float>(result[1]);
	argElement->z = static_cast<float>(result[2]);
}


void MaterialNode::Setup(ID3D11DeviceContext * argContext)
{
	auto graphManager = Singleton<GraphManager>::Get();
	if (!graphManager->FindIndexTexture(L"Notex"))
	{
		graphManager->AddAndLoadImage(L"Notex")->Png();
	}
	auto device = Utility::GetD3Ddevice(argContext).Get();
	cbMaterial_ = std::make_unique<ConstantBuffer<SurfaceMaterial>>(device, eShaderType::Pixel, 0);
	CbSurfaceMaterial(Surface);
	std::wstring ws = TexName[MaterialNode::eTexProperty::Diffuse];
	if (!ws.empty())
	{
		Debug::OutputDebugStringFormat(L"Setup UV Name :" + ws + L"\n");
		texture_ = graphManager->FindIndexTexture(ws);

	}
	else
	{
		texture_ = graphManager->FindIndexTexture(L"Notex");
	}
}

void MaterialNode::Draw(ID3D11DeviceContext * argContext)
{
	cbMaterial_->Update(argContext);
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplers = texture_->Sampler();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srvs = texture_->Srv();

	argContext->PSSetSamplers(0, 1, samplers.GetAddressOf());
	argContext->PSSetShaderResources(0, 1, srvs.GetAddressOf());
}

SurfaceMaterial MaterialNode::CbSurfaceMaterial()const
{
	return SurfaceMaterial(cbMaterial_->Emmisive, cbMaterial_->Ambient, cbMaterial_->Diffuse, cbMaterial_->Specular, cbMaterial_->TransparencyFactor, cbMaterial_->Shininess);
}

void MaterialNode::CbSurfaceMaterial(SurfaceMaterial argSurfaceMaterial)
{
	//	HACK	:	
	cbMaterial_->Emmisive = argSurfaceMaterial.Emmisive;
	cbMaterial_->Ambient = argSurfaceMaterial.Ambient;
	cbMaterial_->Diffuse = argSurfaceMaterial.Diffuse;
	cbMaterial_->Specular = argSurfaceMaterial.Specular;
	cbMaterial_->TransparencyFactor = argSurfaceMaterial.TransparencyFactor;
	cbMaterial_->Shininess = argSurfaceMaterial.Shininess;
}

#pragma region	BoneNode
/**
 *	@fn		GetBoneNodeRoot
 *	@brief	ボーンツリーのルートを取得する
 *	@reutrn	BoneNodeRoot	!<	ボーンツリーのルート
 */
BoneNode *Node::GetBoneNodeRoot()
{
	return boneNodes_.GetRoot();
}


/**
 *	@fn			AddChildBoneNode
 *	@brief		ボーンツリーに親子関係を構築する
 *	@param[in]	argParent	!<	親ボーン
 *	@param[in]	argChild	!<	子ボーン
 */
void Node::AddChildBoneNode(BoneNode* argParent, BoneNode* argChild)
{
	boneNodes_.AddChild(argParent, argChild);
}
/**
 *	@fn			GetBoneNodeByName
 *	@brief		BoneNameの一致するボーンノードを取得する
 *	@param[in]	argName		!<	取得したいボーンの名前
 *	@retval		BoneNode	!<	BoneNameが一致したボーンノード
 *	@retval		nullptr		!<	一致するBoneNameが存在しない
 */
BoneNode* Node::GetBoneNodeByName(const std::string &argName)
{
	for (BoneNode* boneNode = boneNodes_.GetRoot(); boneNode != nullptr; boneNode = boneNodes_.GetNextChildFirst(boneNode))
	{
		if (boneNode->Name == argName)
			return boneNode;
		
	}
	return nullptr;
}
#pragma endregion	BoneNode

#pragma region	MeshNode
/**
 *	@fn		GetMeshNodeRoot
 *	@brief	メッシュツリーのルートを取得する
 *	@reutrn	MeshNodeRoot	!<	メッシュツリーのルート
 */
MeshNode* Node::GetMeshNodeRoot()
{
	return meshNodes_.GetRoot();
}
/**
 *	@fn			AddChildMeshNode
 *	@brief		メッシュツリーに親子関係を構築する
 *	@param[in]	argParent	!<	親メッシュ
 *	@param[in]	argChild	!<	子メッシュ
 */
void Node::AddChildMeshNode(MeshNode* argParent, MeshNode *argNode)
{
	meshNodes_.AddChild(argParent, argNode);
}
/**
 *	@fn		GetCurrentMeshNode
 *	@brief	現在のメッシュノードを取得する
 *	@retval	MeshNode	!<	現在のメッシュノード
 *	@retval	nullptr		!<	メッシュノードが存在しない
 */
MeshNode* Node::GetCurrentMeshNode()
{
	for (auto* meshNode = meshNodes_.GetRoot(); meshNode != nullptr; meshNode = meshNodes_.GetNextChildFirst(meshNode))
	{
		if (meshNode) 
			return meshNode;
	}
	return nullptr;
}

#pragma endregion	MeshNode

/**
 *	@fn		GetBaseSample
 *	@brief	アニメーションのベースサンプルを取得する
 *	@reutrn	アニメーションのベースサンプル
 */
AnimSample* Node::GetBaseSample()
{
	return animationLayer_->Samples[0].get();
}
/**
 *	@fn		StartAnim
 *	@brief	アニメーションを開始する
 */
void Node::StartAnim()
{
	animationLayer_->ChangeAnimSample(0);
	currentSample_ = animationLayer_->Samples[animationLayer_->CurrentSampleIndex].get();
}
/**
 *	@fn		ChangeAnimSample
 *	@brief	インデックスでアニメーションを変更する
 *	@param[in]	argIndex !<	変更したいアニメーションのインデックス
 */
void Node::ChangeAnimSample(int argIndex)
{
	animationLayer_->ChangeAnimSample(argIndex);
	nextSample_ = animationLayer_->GetCurrentSample();
}
/**
 *	@fn		ChangeAnimSample
 *	@brief	名前でアニメーションを変更する
 *	@param[in]	argAnimName	!<	切り替えたいアニメーションの名前
 */
void Node::ChangeAnimSample(const char * argAnimName)
{
	animationLayer_->ChangeAnimSample(argAnimName);
	nextSample_ = animationLayer_->GetCurrentSample();
}
/**
 *	@fn		ChangeAnimSample
 *	@brief	次のアニメーションを変更する
 */
void Node::ChangeNextSample()
{
	assert(nextSample_ && "Not found NextSample...");
	currentSample_ = nextSample_;
	nextSample_ = nullptr;
}
