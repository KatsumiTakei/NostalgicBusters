/**
 *	@file	FbxParser.h
 *	@date	2017 / 9 / 31
 *	@author	Katsumi Takei
 *	@note	FBXSDK-Sample
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma region

#include <fbxsdk.h>
#include "../stdafx.h"

class Node;
struct FbxDevice;

/**
 *	@class	FbxParser
 *	@brief	FbxSdk関連の解析クラス
 */
class FbxParser final
{
private:
	std::unique_ptr<FbxDevice> fbxDevice_;
	std::vector<FbxScene*> fbxScenes_;
public:
	FbxParser();
	~FbxParser();
public:

	/**
	 *  @fn				ImportScene
	 *  @brief			シーンのインポート
	 *  @param[in,out]	argScene		!<	インポートしたいシーン
	 *  @param[in,out]	argErrorCode	!<	エラーコードを格納する変数
	 *  @param[in]		argFileName		!<	読み込むファイルパス
	 *  @retval			true			!<	成功	
	 *  @retval			false			!<	失敗
	 */
	bool ImportScene(FbxScene *argScene, std::string *argErrorCode, const char *argFileName);
	/**
	 *  @fn				ImportScene
	 *  @brief			シーンのインポート
	 *  @param[in,out]	argScene		!<	インポートしたいシーン
	 *  @param[in,out]	argErrorCode	!<	エラーコードを格納する変数
	 *  @param[in]		argFileName		!<	読み込むファイルパス
	 *  @param[in]		argMemory		!<	読み込むモデルのバイナリデータ
	 *  @retval			true			!<	成功	
	 *  @retval			false			!<	失敗
	 */
	bool ImportScene(FbxScene *argScene, std::string *argErrorCode, const char *argFileName, void *argMemory, int argSize);
	/**
	 *  @fn			ExporterScene
	 *  @brief		シーンのエクスポート
	 *  @param[in]	argScene		!<	エクスポートしたいシーン
	 *  @param[in]	argFileName		!<	読み込むファイルパス
	 *  @retval		true			!<	成功	
	 *  @retval		false			!<	失敗
	 */
	bool ExporterScene(FbxScene *argScene, const char *argFileName);
	/**
	 *  @fn				CreateModelNode
	 *  @brief			シーンのインポート
	 *  @param[in]		argFileName			!<	読み込むファイルパス
	 *  @param[in,out]	argScene			!<	読み込んだシーンを格納するシーン
	 *  @param[in]		argFbxAxisSystem	!<	モデルの軸
	 *  @param[in]		argMemory			!<	読み込むモデルのバイナリデータ
	 *  @return			生成されたモデル
	 */
	std::unique_ptr<Node> CreateModelNode(const std::string &argFileName, FbxScene **argScene, FbxAxisSystem argFbxAxisSystem);
	/**
	 *  @fn				CreateModelNode
	 *  @brief			シーンのインポート
	 *  @param[in]		argFileName			!<	読み込むファイルパス
	 *  @param[in,out]	argScene			!<	読み込んだシーンを格納するシーン
	 *  @param[in]		argFbxAxisSystem	!<	モデルの軸
	 *  @param[in]		argMemory			!<	読み込むモデルのバイナリデータ
	 *  @return			生成されたモデル
	 */
	std::unique_ptr<Node> CreateModelNode(const std::string &argFileName, FbxScene **argScene, FbxAxisSystem argFbxAxisSystem, void *argMemory, int argSize);
};