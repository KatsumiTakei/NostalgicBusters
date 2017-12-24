/**
 *	@file   SolidBody.h
 *	@date   2017 / 07 / 26
 *	@author Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../stdafx.h"

class SolidBody : public Task
{
private:
	class Impl;
	std::unique_ptr<Impl>pImpl;

public:
	/**
	 *  @constructor	SolidBody
	 *  @brief			３Dオブジェクト
	 *	@param[in]		argModelName	!<	モデルのパス
	 *	@param[in]		argCamera		!<	カメラ
	 *	@param[in]		argDeviceConext	!<	コンテキスト
	 */
	SolidBody(const std::string argModelName, Camera *argCamera, ID3D11DeviceContext *argDeviceContext);
	~SolidBody();
public:
	bool Update() override;
	void Draw() override;

#pragma region SetterAndGetter
public:
	void Trans(const Math::Vector3& argTrans);
	const Math::Vector3& Trans();
	void Rot(const Math::Vector3& argRot);
	const Math::Vector3& Rot();
	void Scale(const Math::Vector3& argScale);
	const Math::Vector3& Scale();
	void LocalWorld(const Math::Matrix& argMat);
	const Math::Matrix& LocalWorld();
	void RotQuternion(const Math::Quaternion argQuternion);
	const Math::Quaternion& RotQuternion();

	void ParentMatrix(const Math::Matrix *argParentMatrix);
	bool IsParentMatrix();
	void IsUseQuternion(bool argFlag);
#pragma endregion

};