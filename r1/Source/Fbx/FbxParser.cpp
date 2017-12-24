/**
 *	@file	FbxParser.cpp
 *	@date	2017 / 10 / 05
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "FbxParser.h"
#include "Node.h"
#include "Track.h"
#include "AnimLayer.h"
#include "CustomStream.h"
#include "FbxBinaryStream.h"


#ifdef _MD	//	Run-Time Library

#pragma comment(lib, "libfbxsdk-md.lib")	//	!<	DynamicLibrary
#else	//	_MT

#pragma comment(lib, "libfbxsdk-mt.lib")	//	!<	StaticLibrary
#endif	//	Run-Time Library

/**
 *	@struct	FbxDevice
 *	@brief	FbxSdk関連のリソースデバイス
 */
struct FbxDevice final
{
public:
	FbxManager	*SdkManager;
	std::unique_ptr<FbxBinaryStream> Stream;
	FbxImporter *Importer;
	FbxExporter *Exporter;
	std::unique_ptr<FbxGeometryConverter> Converter;
public:
	FbxDevice()
	{
		SdkManager = FbxManager::Create();
		assert(SdkManager && "Create FbxManager Failed...");

		// IOSettings オブジェクト作成、このオブジェクトは全てのインポート/エクスポートの設定を保持する
		auto* ios = FbxIOSettings::Create(SdkManager, IOSROOT);
		SdkManager->SetIOSettings(ios);

		// 実行可能なディレクトリからプラグインをロードする
		auto path = FbxGetApplicationDirectory();
		SdkManager->LoadPluginsDirectory(path.Buffer());

		Importer = FbxImporter::Create(SdkManager, "");
		assert(Importer && "Create Importer Failed...");

		Exporter = FbxExporter::Create(SdkManager, "");
		assert(Exporter && "Create Exporter Failed...");

		Converter = std::make_unique<FbxGeometryConverter>(SdkManager);
		assert(Converter.get() && "Create Converter Failed...");

	}

	~FbxDevice()
	{
		if (Importer)
			Importer->Destroy();

		if (Exporter)
			Exporter->Destroy();

		if (SdkManager)
			SdkManager->Destroy();
	}
};//	struct FbxDevice

class Detail
{
public:
	/**
	 *	@fn			SetPoses
	 *	@brief		シーン内のポーズの取得
	 *	@param[in]	argScene	!<	ポーズを取得したいシーン
	 *	@param[out]	argPoses	!<	ポーズを格納する配列
	 */
	void SetPoses(FbxScene *argScene, std::vector<FbxPose*> argPoses)
	{
		for (int lPoseIndex = 0, lPoseCount = argScene->GetPoseCount(); lPoseIndex < lPoseCount; ++lPoseIndex)
		{
			auto* pose = argScene->GetPose(lPoseIndex);
			argPoses.push_back(pose);
		}
	}
	/**
	 *	@fn			Reset
	 *	@brief		シーンの初期化
	 *	@param[in]		argFbxDevice		!<	FbxDevice
	 *	@param[in,out]	argScene			!<	初期化するfbxシーン
	 *	@param[in]		argFbxAxisSystem	!<	fbxシーンの軸
	 */
	void Initialize(FbxDevice* argFbxDevice, FbxScene *argScene, FbxAxisSystem argFbxAxisSystem)
	{
		//	軸の変換
		const FbxAxisSystem SceneAxisSystem = argScene->GetGlobalSettings().GetAxisSystem();
		const FbxAxisSystem OurAxisSystem = argFbxAxisSystem;
		if (SceneAxisSystem != OurAxisSystem)
		{// 必要に応じてAxis Systemを使用されているものに変換する
			OurAxisSystem.ConvertScene(argScene);
		}
		
		//	必要に応じてユニットシステムを使用されるものに変換する
		const FbxSystemUnit SceneSystemUnit = argScene->GetGlobalSettings().GetSystemUnit();
		if (SceneSystemUnit.GetScaleFactor() != 1.0)
		{// 今回はセンチメートルに変換する
			FbxSystemUnit::cm.ConvertScene(argScene);
		}

		auto *fbxRootNode = argScene->GetRootNode();
		const bool IsTri = IsTriangleMesh(fbxRootNode);
		if (!IsTri)
		{//	三角形メッシュに変換
			DebugLog("convert mesh to trianulate");
			argFbxDevice->Converter->Triangulate(argScene, true);
			//	未使用のメッシュを除去
			argFbxDevice->Converter->RemoveBadPolygonsFromMeshes(argScene);
			// マテリアルごとにメッシュを分離
			bool result = argFbxDevice->Converter->SplitMeshesPerMaterial(argScene, true);
			if (!result)
				DebugWarning("SplitMeshesPerMaterial is Failed...");
			
		}

		//	テクスチャの登録
		Debug::OutputDebugStringFormat(L"\n");
		for (int lTextureIndex = 0, TextureCount = argScene->GetTextureCount(); lTextureIndex < TextureCount; ++lTextureIndex)
		{
			//	テクスチャの取得
			auto *texture = argScene->GetTexture(lTextureIndex);
			const auto *FileTexture = FbxCast<FbxFileTexture>(texture);
			if (FileTexture)
			{
				// 絶対パスのテクスチャ名をロード
				const FbxString FilePath = FileTexture->GetFileName();
				
				// 拡張子とテクスチャ名のみに変更
				const std::wstring FileName = String::ToWstring(String::GetFileName(FilePath.Buffer()));

				//	パスと結合
				const std::wstring TexPath = Singleton<GraphManager>::Get()->Graphic2D()->ImagePath() + FileName;

				// テクスチャのロード	!<	Onry WIC textures are supported now.
				Singleton<GraphManager>::Get()->AddAndLoadImage(TexPath)->Tex();

				Debug::OutputDebugStringFormat(L"Register UV Name : " + TexPath + L"\n");
			}
		}
	}
	/**
	 *	@fn			IsTriangleMesh
	 *	@brief		ノードが三角形メッシュか判別する
	 *	@param[in]	argFbxNode		!<	判別したいノード
	 *	@retval		true			!<	メッシュであり三角形
	 *	@retval		false			!<	メッシュでは無い又は三角形では無い
	 */
	bool IsTriangleMesh(FbxNode *argFbxNode)
	{
		const FbxNodeAttribute* FbxNodeAttr = argFbxNode->GetNodeAttribute();
		if (FbxNodeAttr)
		{
			const auto AttrType = FbxNodeAttr->GetAttributeType();
			if (AttrType == FbxNodeAttribute::eMesh && argFbxNode->GetMesh()->IsTriangleMesh())
			{//	三角形メッシュならOK
				return true;
			}

		}
		for (int lChildIndex = 0, lChildCount = argFbxNode->GetChildCount(); lChildIndex < lChildCount; ++lChildIndex)
		{//	子検索
			FbxNode* childNode = argFbxNode->GetChild(lChildIndex);
			IsTriangleMesh(childNode);
		}
		return false;
	}

	
	/**
	 *	@fn			ProcessNodes
	 *	@brief		ノードの初期化
	 *	@param[in]	argImporter			!<	fbxインポーター
	 *	@param[in]	argNode				!<	親になるノード
	 *	@param[in]	argFbxNode			!<	ソースになるfbxノード
	 *	@param[in]	argParentBoneNode	!<	親ボーン
	 *	@param[in]	argParentMeshNode	!<	親メッシュ
	 */
	bool ProcessNodes(FbxImporter* argImporter, Node* argNode, FbxNode* argFbxNode, BoneNode* argParentBoneNode, MeshNode* argParentMeshNode)
	{
		BoneNode* newBoneNode = nullptr;
		MeshNode* newMeshNode = nullptr;

		const FbxNodeAttribute* NodeAttr = argFbxNode->GetNodeAttribute();
		if (NodeAttr)
		{//	ノードタイプの判別
			const auto AttrType = NodeAttr->GetAttributeType();
			switch (AttrType)
			{
			case fbxsdk::FbxNodeAttribute::eMesh:	//	メッシュ
				newMeshNode = ProcessMeshNode(argNode, argFbxNode, argParentMeshNode);
				argNode->AddMeshs(newMeshNode);
				//	HACK	:	2more meshs support
				break;
			case fbxsdk::FbxNodeAttribute::eSkeleton:	//	ボーン
				newBoneNode = ProcessBoneNode(argImporter, argNode, argFbxNode, argParentBoneNode);
				break;

			default:
				break;
			}
		}

		for (int lMaterialIndex = 0, lMaterialCount = argFbxNode->GetMaterialCount(); lMaterialIndex < lMaterialCount; ++lMaterialIndex)
		{//	マテリアルの設定
			FbxSurfaceMaterial* mat = argFbxNode->GetMaterial(lMaterialIndex);
			if (!mat)
				continue;

			std::unique_ptr<MaterialNode> destMat = std::make_unique<MaterialNode>(mat);
			newMeshNode->MaterialArray.push_back(std::move(destMat));
			
		}

		for (int lChildIndex = 0, lChildCount = argFbxNode->GetChildCount(); lChildIndex < lChildCount; ++lChildIndex)
		{//	子検索
			FbxNode* childNode = argFbxNode->GetChild(lChildIndex);
			ProcessNodes(argImporter, argNode, childNode,
				newBoneNode ? newBoneNode : argParentBoneNode,
				newMeshNode ? newMeshNode : argParentMeshNode);
		}

		return true;
	}
	/**
	 *	@fn				ProcessBoneNode
	 *	@brief			ボーンノードの初期化
	 *	@param[in]		argImporter			!<	fbxインポーター
	 *	@param[in,out]	argNode				!<	親になるノード
	 *	@param[in]		argFbxNode			!<	ソースになるfbxノード
	 *	@param[in]		argParentBoneNode	!<	親ボーン
	 */
	BoneNode* ProcessBoneNode(FbxImporter* argImporter, Node* argNode, FbxNode* argFbxNode, BoneNode* argBoneParent)
	{
		const auto* const Skeleton = static_cast<const FbxSkeleton*>(argFbxNode->GetNodeAttribute());
		if (!Skeleton)
		{
			DebugError("failed to convert skeleton node");
			return  nullptr;
		}
		//	ボーンの作成
		BoneNode* boneNode = new BoneNode();
		argNode->AddChildBoneNode(argBoneParent, boneNode);
		boneNode->Name = argFbxNode->GetName();
		//	初期姿勢登録
		const auto FBXGlobal = argFbxNode->EvaluateGlobalTransform(0);
		boneNode->GlobalTransform = Math::Matrix(FBXGlobal.Transpose());
		boneNode->NextGlobalTransform = Math::Matrix(FBXGlobal.Transpose());

#pragma region		AnimationLayerSample
		
		if(argImporter->GetAnimStackCount() <= 0)
			return boneNode;//	!<	アニメーション無しなら

		argNode->HasAnimation(true);

		//	アニメーションの取得
		const auto* Take = argImporter->GetTakeInfo(0);
		FbxTime start, end;
		if (Take)
		{
			start = Take->mLocalTimeSpan.GetStart();
			end = Take->mLocalTimeSpan.GetStop();
		}
		else
		{//	TODO	:	タイムラインから取得
#if 0
			// タイムラインの値を取得する
			FbxTimeSpan timeLineTimeSpan;
			argScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(timeLineTimeSpan);

			Start = timeLineTimeSpan.GetStart();
			End = timeLineTimeSpan.GetStop();
#endif // 0
		}

		//	HACK	:	アニメーションの長さを動的に変更できるようにする
		//	現在のアニメーションの終了時間を計算
		const FbxTime Length = end.GetFrameCount(FRAME_60) - start.GetFrameCount(FRAME_60) + 1;

		if (Length.Get())
			boneNode->AllocateTracks(static_cast<int>(Length.Get()));

		for (auto lStartFrame = start.GetFrameCount(FRAME_60), lEndFrame = end.GetFrameCount(FRAME_60); lStartFrame <= lEndFrame; ++lStartFrame)
		{//	アニメーション情報の登録
			FbxTime currentFbxTime;
			currentFbxTime.SetFrame(lStartFrame, FRAME_60);

			//	アニメーション時の座標を取得
			const auto FBXLocal = argFbxNode->EvaluateLocalTransform(currentFbxTime);
			const Math::Matrix LocalM(FBXLocal.Transpose());

			//	アニメーション情報の計算
			const Math::Vector3 Prositon = LocalM.GetRowTransform();
			const Math::Vector3 Scale = LocalM.GetRowScale();
			const Math::Quaternion Rotation = Math::Quaternion::CreateFromRotationMatrix(LocalM.Transpose());

			const float CurrentTime = currentFbxTime.GetMilliSeconds() * 0.001f;

			//	アニメーション情報の登録
			const VectorKey PositionKey(Prositon, CurrentTime);
			const VectorKey ScaleKey(Scale, CurrentTime);
			const QuatKey RotationKey(Rotation, CurrentTime);

			boneNode->AddPositionKey(PositionKey);
			boneNode->AddScaleKey(ScaleKey);
			boneNode->AddRotationKey(RotationKey);
		}
		if (argBoneParent)
		{//	親との関係取得
			//	行列を掛ける順番の違い
			const auto InheritType = argFbxNode->InheritType.Get();
			switch (InheritType)
			{
			case FbxTransform::eInheritRrs:
				boneNode->HasInheritScale = BoneNode::eInheritType::Rrs;
				break;
			case FbxTransform::eInheritRrSs:
				boneNode->HasInheritScale = BoneNode::eInheritType::RrSs;
				break;
			case FbxTransform::eInheritRSrs:
				boneNode->HasInheritScale = BoneNode::eInheritType::RSrs;
				break;
			}
		}
		else
		{
			boneNode->HasInheritScale = BoneNode::eInheritType::Rrs;
		}
#pragma endregion	AnimationLayerSample

		return boneNode;
	}
	/**
	 *	@fn				ProcessMeshNode
	 *	@brief			メッシュノードの初期化
	 *	@param[in,out]	argNode				!<	親になるノード
	 *	@param[in]		argFbxNode			!<	ソースになるfbxノード
	 *	@param[in]		argParentMeshNode	!<	親メッシュ
	 */
	MeshNode* ProcessMeshNode(Node* argNode, FbxNode *argFbxNode, MeshNode *argParentMeshNode)
	{
		auto* mesh = argFbxNode->GetMesh();
		if (!mesh)
		{
			DebugLog("this node is not supported a mesh");
			return nullptr;
		}
		//	メッシュ生成
		MeshNode* meshNode = new MeshNode;
		argNode->AddChildMeshNode(argParentMeshNode, meshNode);
		meshNode->Name = argFbxNode->GetName();

		//	座標を取得
		FbxAMatrix localTRS;
		const FbxVector4 T = argFbxNode->LclTranslation.Get();
		const FbxVector4 R = argFbxNode->LclRotation.Get();
		const FbxVector4 S = argFbxNode->LclScaling.Get();
		localTRS.SetTRS(T, R, S);	//	!<	列中心

		Math::Matrix LocalM(localTRS.Transpose());
		meshNode->GlobalTransform = LocalM;

		BuildMeshNode(mesh, meshNode);

		return meshNode;
	}
	/**
	 *	@fn				BuildMeshNode
	 *	@brief			メッシュノードの作成
	 *	@param[in]		argFbxNode			!<	ソースになるfbxノード
	 *	@param[in,out]	argMeshNode			!<	作成したメッシュを格納するポインタ
	 */
	void BuildMeshNode(FbxMesh* argFbxMesh, MeshNode* argMeshNode)
	{
		const int VertexMax = argFbxMesh->GetControlPointsCount();
		argMeshNode->Points.resize(VertexMax);
		for (auto &lPoint : argMeshNode->Points)
		{//	HACK	:	頂点情報を別のクラスに移譲したい
			lPoint = std::make_unique<Point>();
		}

		const auto* const ContolPoints = argFbxMesh->GetControlPoints();
		for (int lVertexIndex = 0; lVertexIndex < VertexMax; ++lVertexIndex)
		{//	頂点座標の登録
			argMeshNode->Points[lVertexIndex]->SetPosition(-ContolPoints[lVertexIndex]);
		}

		const int FaceMax = argFbxMesh->GetPolygonCount();
		argMeshNode->Faces.resize(FaceMax);

		for (int lFaceIndex = 0; lFaceIndex < FaceMax; ++lFaceIndex)
		{//	頂点情報の登録
			Face *face = &argMeshNode->Faces[lFaceIndex];
			LoadVertexIndices(argFbxMesh, lFaceIndex, face);
			LoadNormals(argFbxMesh, lFaceIndex, face);
			LoadUVs(argFbxMesh, lFaceIndex, face);
		}
	}
	/**
	 *	@fn				ProcessSkins
	 *	@brief			スキンメッシュの初期化
	 *	@param[in,out]	argNode				!<	親になるノード
	 *	@param[in]		argFbxNode			!<	ソースになるfbxノード
	 *	@param[in,out]	argMeshNode			!<	作成したメッシュを格納するポインタ
	 *	@param[in,out]	argMeshIndex		!<	メッシュのインデックス
	 *	retval			true				!<	成功
	 *	retval			false				!<	失敗
	 */
	bool ProcessSkins(Node* argNode, FbxNode* argFbxNode, MeshNode* argMeshNode, int *argMeshIndex)
	{
		bool loadedSkin = false;
		const FbxNodeAttribute* NodeAttribute = argFbxNode->GetNodeAttribute();
		if (NodeAttribute)
			if (NodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
			{
				auto mesh = argFbxNode->GetMesh();
				if (mesh)
				{//	ノードタイプがメッシュなら生成
					if (ProcessSkin(argNode, mesh, argNode->MeshList(), argMeshIndex))
						loadedSkin = true;
				}
			}
		for (int lChildIndex = 0, lChildCount = argFbxNode->GetChildCount(); lChildIndex < lChildCount; ++lChildIndex)
		{//	子検索
			ProcessSkins(argNode, argFbxNode->GetChild(lChildIndex), argMeshNode, argMeshIndex);
		}

		return loadedSkin;
	}

	/**
	 *	@fn				ProcessSkin
	 *	@brief			スキンメッシュの取得
	 *	@param[in,out]	argNode				!<	ボーンの情報を持つノード
	 *	@param[in]		argGeo				!<	スキンメッシュの情報を持つジオメトリ
	 *	@param[in,out]	argMeshNode			!<	スキンメッシュの情報を格納するメッシュ
	 *	@param[in,out]	argMeshIndex		!<	メッシュのインデックス
	 *	@retval			true				!<	1個のボーンにウェイトが4個以下
	 *	@retval			false				!<	1個のボーンにウェイトが4個以上
	 */
	bool ProcessSkin(Node* argNode, const FbxGeometry *argGeo, std::vector<MeshNode*> argMeshNode, int *argMeshIndex)
	{
		const int VertexNum = argGeo->GetControlPointsCount();
		for (int lSkinIndex = 0, lSkinCount = argGeo->GetDeformerCount(FbxDeformer::eSkin); lSkinIndex < lSkinCount; ++lSkinIndex)
		{
			//	Skin取得
			const auto* const Skin = static_cast<FbxSkin*>(argGeo->GetDeformer(lSkinIndex, FbxDeformer::eSkin));
			assert(Skin && "Not Found Cluster Skin...");

			for (int lClusterIndex = 0, lClusterCount = Skin->GetClusterCount(); lClusterIndex < lClusterCount; ++lClusterIndex)
			{
				//	Cluster取得
				const auto* const Cluster = Skin->GetCluster(lClusterIndex);
				assert((Cluster->GetLinkMode() == FbxCluster::eNormalize || Cluster->GetLinkMode() == FbxCluster::eTotalOne) && 
					"failed to find link bone node");

				const auto* const LinkBoneNode = Cluster->GetLink();
				if (!LinkBoneNode)
				{//	対応していないリンクモード
					DebugLog("failed to find linked bone node");
					continue;
				}

				BoneNode* boneNode = argNode->GetBoneNodeByName(LinkBoneNode->GetName());
				assert(boneNode);

				//	ローカル座標空間でのボーンの行列を取得
				FbxAMatrix linkedMatrix;
				Cluster->GetTransformLinkMatrix(linkedMatrix);

				//	ローカル座標空間でのボーンの逆行列を取得
				const auto LinkedInverseMatrix = linkedMatrix.Inverse();
				boneNode->InverseTransform = Math::Matrix(LinkedInverseMatrix.Transpose());

				//	ボーンが影響を与える頂点のインデックスとウェイトを取得
				const int ClusterIndicesNum = Cluster->GetControlPointIndicesCount();
				const int *ControlPointIndices = Cluster->GetControlPointIndices();
				if (!ControlPointIndices)
				{
					DebugError("Not Found Indices...");
					continue;
				}
				const double *ControlPointWeights = Cluster->GetControlPointWeights();
				if (!ControlPointWeights)
				{
					DebugError("Not Found Weights...");
					continue;
				}
				for (int lVertexIndex = 0; lVertexIndex < ClusterIndicesNum; ++lVertexIndex)
				{//	影響を与える頂点の数だけウェイト計算
					const unsigned int ControlIndex = ControlPointIndices[lVertexIndex];
					
					if(ControlIndex >= argMeshNode[*argMeshIndex]->Points.size())
					{
						DebugError("ControlIndex Over Points...");
						continue;
					}

					const double ControlWeight = ControlPointWeights[lVertexIndex];
					if (ControlWeight == 0.0)
						continue;
					const unsigned int BoneID = boneNode->id_;

					//	頂点のウェイトが4っつ以下ならウェイトを追加
					//	HACK	:	2more meshs support
					if (!argMeshNode[*argMeshIndex]->Points[ControlIndex]->AddWeight(BoneID, static_cast<float>(ControlWeight)))
					{//	失敗にするかも
					}

				}
			}
		}
		*argMeshIndex++;

		return true;
	}
	/**
	 *	@fn			LoadVertexIndices
	 *	@brief		ポリゴンの頂点インデックスの取得
	 *	@param[in]	argFbxMesh		!<	ポリゴンの頂点インデックの情報を持つノード
	 *	@param[in]	argFaceIndex	!<	ポリゴン内のインデックス(三角形化したので最大3点)
	 *	@param[out]	argFace			!<	頂点インデックスを格納するポインタ
	 */
	void LoadVertexIndices(FbxMesh *argFbxMesh, int argFaceIndex, Face *argFace)
	{
		for (int lFaceVertexIndex = 0; lFaceVertexIndex < Face::ComponentNum; ++lFaceVertexIndex)
		{
			const int VertexIndex = argFbxMesh->GetPolygonVertex(argFaceIndex, lFaceVertexIndex);
			argFace->VerticeIndex[lFaceVertexIndex] = VertexIndex;
		}
	}
	/**
	 *	@fn			LoadNormals
	 *	@brief		法線の取得
	 *	@param[in]	argFbxMesh		!<	法線の情報を持つノード
	 *	@param[in]	argFaceIndex	!<	ポリゴン内のインデックス(三角形化したので最大3点)
	 *	@param[out]	argFace			!<	法線を格納するポインタ
	 */
	void LoadNormals(FbxMesh *argFbxMesh, int argFaceIndex, Face *argFace)
	{
		for (int lFacePointIndex = 0; lFacePointIndex < Face::ComponentNum; ++lFacePointIndex)
		{
			FbxVector4 normal;
			argFbxMesh->GetPolygonVertexNormal(argFaceIndex, lFacePointIndex, normal);
			argFace->Normals[lFacePointIndex].x = -static_cast<float>(normal[0]);
			argFace->Normals[lFacePointIndex].y = -static_cast<float>(normal[1]);
			argFace->Normals[lFacePointIndex].z = -static_cast<float>(normal[2]);
		}
	}
	/**
	 *	@fn			LoadUVs
	 *	@brief		UV座標の取得
	 *	@param[in]	argFbxMesh		!<	UV座標の情報を持つノード
	 *	@param[in]	argFaceIndex	!<	UV座標を取得したいポリゴンのインデックス
	 *	@param[out]	argFace			!<	UV座標を格納するポインタ
	 */
	void LoadUVs(FbxMesh *argFbxMesh, int argFaceIndex, Face *argFace)
	{//	UV名取得
		FbxStringList	uvSetName;
		argFbxMesh->GetUVSetNames(uvSetName);
		const int UVSetNum = uvSetName.GetCount();

		bool unmapped = false;
		for (int lUvIndex = 0; lUvIndex < UVSetNum; ++lUvIndex)
		{
			for (int lFacePointIndex = 0; lFacePointIndex < Face::ComponentNum; ++lFacePointIndex)
			{//	UV座標取得
				FbxString name = uvSetName.GetStringAt(lUvIndex);
				FbxVector2 texCoord;
				argFbxMesh->GetPolygonVertexUV(argFaceIndex, lFacePointIndex, name, texCoord, unmapped);
				argFace->UVs[lFacePointIndex].x = -static_cast<float>(texCoord[0]);
				argFace->UVs[lFacePointIndex].y = -static_cast<float>(texCoord[1]);
			}
		}
	}

	void ProcessPoses(FbxNode *argNode, FbxImporter *argImporter)
	{
		//TODO : add Pose Matrix to each boneNode
	}

	static bool ImportScene(FbxScene * argScene, std::string * argErrorCode, std::string argFileName, bool argState, FbxDevice* argFbxDevice)
	{
		//	SDKのVerを取得
		int sdkMajor, sdkMinor, sdkRevision;
		argFbxDevice->SdkManager->GetFileFormatVersion(sdkMajor, sdkMinor, sdkRevision);

		//	ファイルの形式を取得
		int fileMajor, fileMinor, fileRevision;
		argFbxDevice->Importer->GetFileVersion(fileMajor, fileMinor, fileRevision);

		if (!argState)
		{
			auto error = argFbxDevice->Importer->GetStatus().GetErrorString();
			Debug::OutputDebugStringFormat(L"Call to FbxImporter::Initialize() failed.\n");
			Debug::OutputDebugStringFormat(L"Error File " + Utility::String::ToWstring(argFileName) + L"\n");
			Debug::OutputDebugStringFormat(L"Error returned: " + Utility::String::ToWstring(error) + L"\n\n");

			if (argFbxDevice->Importer->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
			{//	FBX SDK がサポートするファイルバージョンと一致しないバージョンのファイルをロードした場合はエラーにする
				Debug::OutputDebugStringFormat(L"FBX file format version for this FBX SDK is %d.%d.%d\n", sdkMajor, sdkMinor, sdkRevision);
				Debug::OutputDebugStringFormat(L"FBX file format version for file" + Utility::String::ToWstring(argFileName) + L"is %d.%d.%d\n\n", fileMajor, fileMinor, fileRevision);
			}
			assert(false && "Call to FbxImporter::Initialize Failed");
		}

		Debug::OutputDebugStringFormat(L"FBX file format version for this FBX SDK is %d.%d.%d\n\n", sdkMajor, sdkMinor, sdkRevision);

		if (argFbxDevice->Importer->IsFBX())
		{//	FBXならアニメーション情報を出力
			Debug::OutputDebugStringFormat(L"FBX file format version for file '" + Utility::String::ToWstring(argFileName) + L"' is %d.%d.%d\n\n", fileMajor, fileMinor, fileRevision);
			Debug::OutputDebugStringFormat(L"Animation Stack Information\n");

			const int AnimStackCount = argFbxDevice->Importer->GetAnimStackCount();
			Debug::OutputDebugStringFormat(L"    Number of Animation Stacks: %d\n", AnimStackCount);
			Debug::OutputDebugStringFormat((L"    Current Animation Stack: " + Utility::String::ToWstring(argFbxDevice->Importer->GetActiveAnimStackName().Buffer()) + L"\n").c_str());
			Debug::OutputDebugStringFormat(L"\n");

			for (int lAnimIndex = 0; lAnimIndex < AnimStackCount; lAnimIndex++)
			{
				auto *takeInfo = argFbxDevice->Importer->GetTakeInfo(lAnimIndex);

				Debug::OutputDebugStringFormat(L"    Animation Stack %d\n", lAnimIndex);
				Debug::OutputDebugStringFormat((L"         Name: " + Utility::String::ToWstring(takeInfo->mName.Buffer()) + L"\n"));
				Debug::OutputDebugStringFormat((L"         Description: " + Utility::String::ToWstring(takeInfo->mDescription.Buffer()) + L"\n"));

				Debug::OutputDebugStringFormat((L"         Import Name: " + Utility::String::ToWstring(takeInfo->mImportName.Buffer()) + L"\n"));

				std::wstring ws = (takeInfo->mSelect) ? L"true" : L"false";
				Debug::OutputDebugStringFormat(std::wstring(L"         Import State: ") + ws + L"\n");
				Debug::OutputDebugStringFormat(L"\n");
			}
			//	FBXの形式を決定
			auto ios = argFbxDevice->SdkManager->GetIOSettings();
			ios->SetBoolProp(IMP_FBX_MATERIAL, true);
			ios->SetBoolProp(IMP_FBX_TEXTURE, true);
			ios->SetBoolProp(IMP_FBX_LINK, true);
			ios->SetBoolProp(IMP_FBX_SHAPE, true);
			ios->SetBoolProp(IMP_FBX_GOBO, true);
			ios->SetBoolProp(IMP_FBX_ANIMATION, true);
			ios->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
		}

		bool status = argFbxDevice->Importer->Import(argScene);
		if (!status)
		{//	暗号化の可能性を見て復号化する
			*argErrorCode = argFbxDevice->Importer->GetStatus().GetErrorString();
			if (argFbxDevice->Importer->GetStatus().GetCode() == FbxStatus::ePasswordError)
			{
				Debug::OutputDebugStringFormat(L"Please enter password: ");
				char password[1024];
				password[0] = '\0';

				FBXSDK_CRT_SECURE_NO_WARNING_BEGIN
					scanf("%s", password);
				FBXSDK_CRT_SECURE_NO_WARNING_END

					FbxString str(password);

				auto ios = argFbxDevice->SdkManager->GetIOSettings();
				ios->SetStringProp(IMP_FBX_PASSWORD, str);
				ios->SetBoolProp(IMP_FBX_PASSWORD_ENABLE, true);

				status = argFbxDevice->Importer->Import(argScene);

				if (!status && argFbxDevice->Importer->GetStatus().GetCode() == FbxStatus::ePasswordError)
				{
					Debug::OutputDebugStringFormat(L"\nPassword is wrong, import aborted.\n");
				}
			}
		}
		return status;
	}
};//	class Detail


FbxParser::FbxParser()
	:fbxDevice_(std::make_unique<FbxDevice>())
{
}

FbxParser::~FbxParser()
{
	if (!fbxScenes_.empty())
	{
		for (auto fbxScene : fbxScenes_)
		{
			fbxScene->Destroy();
		}
		fbxScenes_.clear();
	}
}
/**
 *	@fn			ExporterScene
 *	@brief		シーンの書き出し
 *	@param[in]	argScene	!<	書き出したいシーン
 *	@param[out]	argFileName	!<	書き出すファイル名
 *	@retval		true		!<	書き出し成功
 *	@retval		false		!<	書き出し失敗
 */
bool FbxParser::ExporterScene(FbxScene *argScene, const char *argFileName)
{
	const auto ExportFilename = (std::string(argFileName) + ".fbx").c_str();
	int fileFormat = -1;
	if (fileFormat < 0 || fileFormat >= fbxDevice_->SdkManager->GetIOPluginRegistry()->GetWriterFormatCount())
	{//	ファイルの形式取得
		fileFormat = fbxDevice_->SdkManager->GetIOPluginRegistry()->GetNativeWriterFormat();

		for (int lFormatIndex = 0, lFormatCount = fbxDevice_->SdkManager->GetIOPluginRegistry()->GetWriterFormatCount(); lFormatIndex < lFormatCount; lFormatIndex++)
		{//	ファイルの書き出し形式取得
			if (fbxDevice_->SdkManager->GetIOPluginRegistry()->WriterIsFBX(lFormatIndex))
			{//	FBXなら書き出し
				const FbxString Desc = fbxDevice_->SdkManager->GetIOPluginRegistry()->GetWriterFormatDescription(lFormatIndex);
				const char *ASCII = "ascii";
				if (Desc.Find(ASCII) >= 0)
				{
					fileFormat = lFormatIndex;
					break;
				}
			}
		}
	}
	//	FBXの設定を決める
	auto ios = fbxDevice_->SdkManager->GetIOSettings();
	ios->SetBoolProp(EXP_FBX_MATERIAL, true);
	ios->SetBoolProp(EXP_FBX_TEXTURE, true);
	ios->SetBoolProp(EXP_FBX_EMBEDDED, false);
	ios->SetBoolProp(EXP_FBX_SHAPE, true);
	ios->SetBoolProp(EXP_FBX_GOBO, true);
	ios->SetBoolProp(EXP_FBX_ANIMATION, true);
	ios->SetBoolProp(EXP_FBX_GLOBAL_SETTINGS, true);
	if (!fbxDevice_->Exporter->Initialize(argFileName, fileFormat, ios))
	{
		Debug::OutputDebugStringFormat(L"Call to FbxExporter::Initialize() failed.\n");
		Debug::OutputDebugStringFormat(L"Error returned: %s\n\n", fbxDevice_->Exporter->GetStatus().GetErrorString());
		return false;
	}
	//	ファイルのバージョンを取得
	int major, minor, revision;

	FbxManager::GetFileFormatVersion(major, minor, revision);
	Debug::OutputDebugStringFormat(L"FBX file format version %d.%d.%d\n\n", major, minor, revision);

	bool exportStatus = fbxDevice_->Exporter->Initialize(ExportFilename, fileFormat, fbxDevice_->SdkManager->GetIOSettings());
	assert(exportStatus && "Exporter Initialize failed...");

	exportStatus = fbxDevice_->Exporter->Export(argScene);

	return exportStatus;
}

std::unique_ptr<Node> FbxParser::CreateModelNode(const std::string & argFileName, FbxScene ** argScene, FbxAxisSystem argFbxAxisSystem)
{
	 assert(argFileName.c_str() && "Argment Filename Nothing...");

	 // ファイルからインポート/エクスポートされるほとんどのオブジェクトを保持するFBXシーンを作成する
	 auto fbxScene = FbxScene::Create(fbxDevice_->SdkManager, argFileName.c_str());
	 assert(fbxScene && "Error: Unable to create FBX scene!\n");

	 std::string errorCode;

	 //	シーンのインポート
	 bool result = ImportScene(fbxScene, &errorCode, argFileName.c_str());
	 errorCode = "ImportScene Failed... Error Code : " + errorCode;
	 assert(result && errorCode.c_str());

	 //	シーンの初期化
	 std::unique_ptr<Detail> pImpl = std::make_unique<Detail>();
	 pImpl->Initialize(fbxDevice_.get(), fbxScene, argFbxAxisSystem);

	 auto FbxRoot = fbxScene->GetRootNode();

	 std::unique_ptr<Node> node = std::make_unique<Node>(std::make_unique<AnimLayer>(fbxDevice_->Importer));

	 //	ノードの初期化
	 pImpl->ProcessNodes(fbxDevice_->Importer, node.get(), FbxRoot, node->GetBoneNodeRoot(), node->GetMeshNodeRoot());
	 int meshIndex = 0;	//	!<	HACK	:	ノードオブジェクトに持たせるかも
	 pImpl->ProcessSkins(node.get(), FbxRoot, node->GetCurrentMeshNode(), &meshIndex);

	 (*argScene) = fbxScene;
	 fbxScenes_.push_back(fbxScene);
	 return std::move(node);
 }
/**
 *	@fn				ImportScene
 *	@brief			Fbxシーンの作成
 *	@param[in,out]	argScene		!<	読み込んだシーンを格納するシーンのポインタ
 *	@param[in,out]	argErrorCode	!<	エラーコードを格納するポインタ
 *	@param[in]		argFileName		!<	読み込むファイルの名前
 *	@retval			true			!<	読み込み成功
 *	@retval			false			!<	読み込み失敗
 */

std::unique_ptr<Node> FbxParser::CreateModelNode(const std::string &argFileName, FbxScene **argScene, FbxAxisSystem argFbxAxisSystem, void *argMemory, int argSize)
{
	assert(argFileName.c_str() && "Argment Filename Nothing...");

	// ファイルからインポート/エクスポートされるほとんどのオブジェクトを保持するFBXシーンを作成する
	auto fbxScene = FbxScene::Create(fbxDevice_->SdkManager, argFileName.c_str());
	assert(fbxScene && "Error: Unable to create FBX scene!\n");

	std::string errorCode;

	//	シーンのインポート
	bool result = ImportScene(fbxScene, &errorCode, argFileName.c_str(), argMemory, argSize);
	errorCode = "ImportScene Failed... Error Code : " + errorCode;
	assert(result && errorCode.c_str());

	//	シーンの初期化
	std::unique_ptr<Detail> pImpl = std::make_unique<Detail>();
	pImpl->Initialize(fbxDevice_.get(), fbxScene, argFbxAxisSystem);

	auto FbxRoot = fbxScene->GetRootNode();

	std::unique_ptr<Node> node = std::make_unique<Node>(std::make_unique<AnimLayer>(fbxDevice_->Importer));

	//	ノードの初期化
	pImpl->ProcessNodes(fbxDevice_->Importer, node.get(), FbxRoot, node->GetBoneNodeRoot(), node->GetMeshNodeRoot());
	int meshIndex = 0;	//	!<	HACK	:	ノードオブジェクトに持たせるかも
	pImpl->ProcessSkins(node.get(), FbxRoot, node->GetCurrentMeshNode(), &meshIndex);

	(*argScene) = fbxScene;
	fbxScenes_.push_back(fbxScene);
	return std::move(node);
}

bool FbxParser::ImportScene(FbxScene * argScene, std::string * argErrorCode, const char * argFileName)
{
	int fileFormat = -1;

	if (!fbxDevice_->SdkManager->GetIOPluginRegistry()->DetectReaderFileFormat(argFileName, fileFormat))
	{// 検出できないファイル形式なのでFbxImporter::eFBX_BINARY 形式でトライする
		fileFormat = fbxDevice_->SdkManager->GetIOPluginRegistry()->FindReaderIDByDescription("FBX binary (*.fbx)");
	}
	
	//	パスからロード
	bool importState = fbxDevice_->Importer->Initialize(argFileName, fileFormat, fbxDevice_->SdkManager->GetIOSettings());

	return Detail::ImportScene(argScene, argErrorCode, argFileName, importState, fbxDevice_.get());
}


bool FbxParser::ImportScene(FbxScene * argScene, std::string * argErrorCode, const char * argFileName, void * argMemory, int argSize)
{
	int fileFormat = -1;

	auto manager = fbxDevice_->SdkManager;
	if (!manager->GetIOPluginRegistry()->DetectReaderFileFormat(argFileName, fileFormat))
	{// 検出できないファイル形式なのでFbxImporter::eFBX_BINARY 形式でトライする
		fileFormat = manager->GetIOPluginRegistry()->FindReaderIDByDescription("FBX binary (*.fbx)");
	}
	//	バイナリデータでロード
	fbxDevice_->Stream = std::make_unique<FbxBinaryStream>(argSize);
	bool importState = fbxDevice_->Importer->Initialize(fbxDevice_->Stream.get(), argMemory, fileFormat, manager->GetIOSettings());


	return Detail::ImportScene(argScene, argErrorCode, argFileName, importState, fbxDevice_.get());
}
