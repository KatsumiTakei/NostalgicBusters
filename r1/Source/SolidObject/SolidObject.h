/**
 *	@file   SolidObject.h
 *	@date   2017 / 07 / 26
 *	@author Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../stdafx.h"

class SolidBody;
class SphereNode;

class SolidObject
{
private:
	class Impl;
	std::unique_ptr<Impl>pImpl;

public:
	/**
	 *  @constructor	SolidObject
	 *  @brief			当たり判定付き３Dオブジェクト
	 *	@param[in]		argModelName	!<	モデルのパス
	 *	@param[in]		argCamera		!<	カメラ
	 *	@param[in]		argDeviceConext	!<	コンテキスト
	 */
	SolidObject(const std::string argModelName, Camera *argCamera, ID3D11DeviceContext *argDeviceConext);
	~SolidObject();

	bool Update();
	void Draw();
	SolidBody *Body()const;
	SphereNode *CollisionNode()const;
};