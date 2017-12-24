/**
 *	@file	Node.h
 *	@date	2017 / 9 / 27
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <fbxsdk.h>

#include "../stdafx.h"
#include "nodetree.h"

struct VectorKey;
struct QuatKey;
struct VectorTrack;
struct QautTrack;
struct AnimSample;
struct AnimLayer;

/**
 *  @struct Point
 *  @brief	頂点情報
 */
struct Point
{
public:
	static const int	BoneComponentNum = 4;
	Math::Vector3		Position;
	unsigned int		BoneIDs[BoneComponentNum];
	float				BoneWeights[BoneComponentNum];
private:
	unsigned int		BoneComponentIndex;
public:
	/**
	 *  @struct Point
	 *  @brief	頂点情報
	 */
	Point()
		: BoneComponentIndex(0)
	{
		std::memset(BoneIDs, 0, sizeof(BoneIDs));
		std::memset(BoneWeights, 0, sizeof(BoneWeights));
	}
	~Point() = default;

public:

	/**
	 *  @fn			SetPosition
	 *  @brief		座標の登録
	 *  @param[in]	argPos	!<	座標
	 */
	inline void SetPosition(const fbxsdk::FbxVector4 &argPos)
	{
		Position.x = static_cast<float>(argPos[0]);
		Position.y = static_cast<float>(argPos[1]);
		Position.z = static_cast<float>(argPos[2]);
	}
	/**
	 *  @fn			AddWeight
	 *  @brief		ウェイトの追加
	 *  @param[in]	argBoneID	!<	ウェイトを与えるボーンのID
	 *  @param[in]	argWeight	!<	ウェイト
	 *	@retval		true		!<	追加成功
	 *	@retval		false		!<	追加失敗
	 */
	inline bool AddWeight(const unsigned int argBoneID, const float argWeight)
	{
		if (BoneComponentIndex > BoneComponentNum)
		{
			return false;
		}
		BoneIDs[BoneComponentIndex] = argBoneID;
		BoneWeights[BoneComponentIndex] = argWeight;
		++BoneComponentIndex;
		return true;
	}
};
/**
 *  @struct		BoneNode
 *  @brief		ボーンノード
 */
struct BoneNode : public NodeTreeItem<BoneNode>
{
public:
	/**
	 *  @enum		eInheritType
	 *  @brief		親アニメーションとの計算の種類
	 */
	enum class eInheritType
	{
		Rrs,
		RrSs,
		RSrs,
	};
public:
	std::unique_ptr<VectorTrack>	PositionTrack;			//	!<	アニメーション時の座標
	std::unique_ptr<VectorTrack>	ScaleTrack;				//	!<	アニメーション時の拡縮
	std::unique_ptr<QautTrack>		RotationTrack;			//	!<	アニメーション時の角度
	Math::Matrix					GlobalTransform;		//	!<	初期姿勢座標
	Math::Matrix					InverseTransform;		//	!<	逆姿勢行列
	eInheritType					HasInheritScale;		//	!<	親アニメーションとの計算の種類
	Math::Matrix					NextGlobalTransform;	//	!<	オフセット姿勢座標
	std::string						Name;					//	!<	名前
	int								AllocatedSize;			//	!<	アニメーションの時間
public:
	
	BoneNode()
		: Name("null"), AllocatedSize(0)
	{
	}

	~BoneNode() = default;
public:

	/**
	 *	@fn			AllocateTracks
	 *	@brief		アニメーショントラックの生成
	 *	@param[in]	argFrameNum	!<	アニメーションの総フレーム数
	 */
	void AllocateTracks(int argKeyNums);
	/**
	 *  @fn			AddPositionKey
	 *  @brief		アニメーション座標の登録
	 *  @param[in]	argkey	!<	アニメーションの座標
	 */
	void AddPositionKey(const VectorKey& argkey);
	/**
	 *  @fn			AddScaleKey
	 *  @brief		アニメーション拡縮の登録
	 *  @param[in]	argkey	!<	アニメーションの拡縮
	 */
	void AddScaleKey(const VectorKey& argkey);
	/**
	 *  @fn			AddRotationKey
	 *  @brief		アニメーション角度の登録
	 *  @param[in]	argkey	!<	アニメーションの角度
	 */
	void AddRotationKey(const QuatKey& argkey);

};
/**
 *  @struct		SurfaceMaterial
 *  @brief		定数バッファマテリアル
 */
struct SurfaceMaterial
{
	Math::Vector3 Emmisive;		//	!<	放射光、マテリアル自身が発する光の色
	Math::Vector3 Ambient;		//	!<	環境光、反射する色
	Math::Vector3 Diffuse;		//	!<	拡散反射光、反射する色
	Math::Vector3 Specular;		//	!<	鏡面反射光、反射する色
	float Shininess;			//	!<	光の強度、フォンに使う
	float TransparencyFactor;	//	!<	光の透過度、フォンとランバートに使う
	int padding[2];
	/**
	 *  @struct		SurfaceMaterial
	 *  @brief		マテリアル
	 *  @param[in]	argEmmisive				!<	放射光
	 *  @param[in]	argAmbient				!<	環境光
	 *  @param[in]	argDiffuse				!<	拡散反射光
	 *  @param[in]	argSpecular				!<	鏡面反射光
	 *  @param[in]	argShininess			!<	光の強度
	 *  @param[in]	argTransparencyFactor	!<	光の透過度
	 */
	SurfaceMaterial(Math::Vector3 argEmmisive, Math::Vector3 argAmbient, Math::Vector3 argDiffuse, Math::Vector3 argSpecular,
		float argShininess, float argTransparencyFactor)
		:Emmisive(argEmmisive), Ambient(argAmbient), Diffuse(argDiffuse), Specular(argSpecular),
		TransparencyFactor(argTransparencyFactor), Shininess(argShininess)
	{
	}
	SurfaceMaterial() = default;

};
/**
 *  @struct		MaterialNode
 *  @brief		マテリアルノード
 */
struct MaterialNode
{
private:

	/**
	 *  @fn			SetMaterialProperty
	 *  @brief		マテリアル情報の設定
	 *  @param[in]	argMaterial				!<	FbxSdkのマテリアル型
	 *  @param[in]	argPropertyName			!<	設定したいプロパティの名前
	 *  @param[in]	argFactorPropertyName	!<	設定したい要素の名前
	 *  @param[in]	argElement				!<	要素を格納する変数
	 *  @param[in]	argTextureName			!<	テクスチャの名前を格納する変数
	 */
	void SetMaterialProperty(const FbxSurfaceMaterial * argMaterial,
		const char * argPropertyName, const char *argFactorPropertyName,
		Math::Vector3* argElement, std::wstring *argTextureName);

public:
	/**
	 *  @struct		MaterialNode
	 *  @brief		マテリアルノード
	 *	@param[in]	argFbxSurfaceMaterial	!<	FbxSdkのマテリアル型
	 */
	MaterialNode(FbxSurfaceMaterial*argFbxSurfaceMaterial); 
	~MaterialNode() = default;
public:
	/**
	 *	@enum	eTexProperty
	 *	@brief	テクスチャの種類
	 */
	enum eTexProperty
	{//	!<	基本Diffuseのみ
		Emissive,
		Ambient,
		Diffuse,
		Specular,

		Max,
	};
	/**
	 *	@enum	eMaterialType
	 *	@brief	Fbxのマテリアルに当たっているShaderタイプ
	 */
	enum class eMaterialType
	{// FBXのマテリアルはLambertとPhongしかない
		Lambert,
		Phong,
	};

	eMaterialType Type;
	SurfaceMaterial Surface;
	std::wstring TexName[eTexProperty::Max];

public:
	std::shared_ptr<Texture>	texture_;
	std::unique_ptr<ConstantBuffer
		<SurfaceMaterial>>		cbMaterial_;

public:

	SurfaceMaterial CbSurfaceMaterial()const;
	void CbSurfaceMaterial(SurfaceMaterial argSurfaceMaterial);
	void Setup(ID3D11DeviceContext *argContext);
	void Draw(ID3D11DeviceContext *argContext);
};
/**
 *  @struct		Face
 *  @brief		表面
 */
struct Face
{
public:
	static const int	ComponentNum = 3;
	unsigned int		VerticeIndex[ComponentNum];	//	!<	頂点のインデックス
	Math::Vector3		Normals[ComponentNum];		//	!<	頂点の法線座標
	Math::Vector2		UVs[ComponentNum];			//	!<	頂点のUv座標

};
/**
 *  @struct		MeshNode
 *  @brief		メッシュノード
 */
struct MeshNode : public NodeTreeItem<MeshNode>
{
public:
	Math::Matrix								GlobalTransform;//	!<	絶対座標
	Math::Matrix								LocalTransform;	//	!<	ローカル座標
	std::vector<std::unique_ptr<Point>>			Points;			//	!<	頂点の情報
	std::string									Name;			//	!<	メッシュの名前
	std::vector<Face>							Faces;			//	!<	表面の情報
	std::vector<std::unique_ptr<MaterialNode>>	MaterialArray;	//	!<	マテリアル

};
/**
 *  @class 	Node
 *  @brief	モデルノード
 */
class Node
{
private:
	NodeTree<BoneNode>			boneNodes_;
	NodeTree<MeshNode>			meshNodes_;

	std::unique_ptr<AnimLayer>	animationLayer_;
	AnimSample*					currentSample_;
	AnimSample*					nextSample_;
	std::vector<MeshNode*>		meshs_;
	bool hasAnimation_;
public:
	/**
	 *  @constructor 	Node
	 *  @brief			モデルノード
	 *  @param[in]		argAnimationLayer	!<	アニメーションのレイヤー
	 */
	Node(std::unique_ptr<AnimLayer> argAnimationLayer)
		: animationLayer_(std::move(argAnimationLayer)), currentSample_(nullptr), nextSample_(nullptr), hasAnimation_(false)
	{
	}

	~Node() = default;

public:

	/**
	 *	@fn			AddChildBoneNode
	 *	@brief		ボーンツリーに親子関係を構築する
	 *	@param[in]	argParent	!<	親ボーン
	 *	@param[in]	argChild	!<	子ボーン
	 */
	void				AddChildBoneNode(BoneNode* argParent, BoneNode* argChild);
	/**
	 *	@fn			AddChildMeshNode
	 *	@brief		メッシュツリーに親子関係を構築する
	 *	@param[in]	argParent	!<	親メッシュ
	 *	@param[in]	argChild	!<	子メッシュ
	 */
	void				AddChildMeshNode(MeshNode* argParent, MeshNode *argNode);
	/**
	 *	@fn		StartAnim
	 *	@brief	アニメーションを開始する
	 */
	void				StartAnim();
	/**
	 *	@fn		ChangeAnimSample
	 *	@brief	インデックスでアニメーションを変更する
	 * 	@param[in]	argIndex !<	変更したいアニメーションのインデックス
	 */
	void				ChangeAnimSample(int argIndex);
	/**
	 *	@fn		ChangeAnimSample
	 *	@brief	名前でアニメーションを変更する
	 *	@param[in]	argAnimName	!<	切り替えたいアニメーションの名前
	 */
	void				ChangeAnimSample(const char *argAnimName);
	/**
	 *	@fn		ChangeAnimSample
	 *	@brief	次のアニメーションを変更する
	 */
	void				ChangeNextSample();

#pragma region		Properties
	inline bool			HasAnimation() { return hasAnimation_; }
	inline void			HasAnimation(bool argHasAnimation) { hasAnimation_ = argHasAnimation; }

	inline void			AddMeshs(MeshNode* argMeshNode) { meshs_.push_back(argMeshNode); }
	inline std::vector<MeshNode*> MeshList() { return meshs_; }
	/**
	 *	@fn		GetBaseSample
	 *	@brief	アニメーションのベースサンプルを取得する
	 *	@reutrn	アニメーションのベースサンプル
	 */
	AnimSample*			GetBaseSample();
	inline AnimLayer*	AnimationLayer() const { return animationLayer_.get(); }
	inline AnimSample*	CurrentSample() const { return currentSample_; }
	inline AnimSample*	NextSample() const { return nextSample_; }
	/**
	 *	@fn		GetBoneNodeRoot
	 *	@brief	ボーンツリーのルートを取得する
	 *	@reutrn	BoneNodeRoot	!<	ボーンツリーのルート
	 */
	BoneNode*			GetBoneNodeRoot();
	/**
	 *	@fn			GetBoneNodeByName
	 *	@brief		BoneNameの一致するボーンノードを取得する
	 *	@param[in]	argName		!<	取得したいボーンの名前
	 *	@retval		BoneNode	!<	BoneNameが一致したボーンノード
	 *	@retval		nullptr		!<	一致するBoneNameが存在しない
	 */
	BoneNode*			GetBoneNodeByName(const std::string &argName);
	/**
	 *	@fn		GetMeshNodeRoot
	 *	@brief	メッシュツリーのルートを取得する
	 *	@reutrn	MeshNodeRoot	!<	メッシュツリーのルート
	 */
	MeshNode*			GetMeshNodeRoot();
	/**
	 *	@fn		GetCurrentMeshNode
	 *	@brief	現在のメッシュノードを取得する
	 *	@retval	MeshNode	!<	現在のメッシュノード
	 *	@retval	nullptr		!<	メッシュノードが存在しない
	 */
	MeshNode*			GetCurrentMeshNode();

#pragma endregion	Properties
};