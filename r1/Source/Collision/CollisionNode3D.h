/**
 *	@file	CollisionNode3D.h
 *	@date	2017 / 7 / 15
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../stdafx.h"

class SolidBody;
struct ID3D11DeviceContext;

namespace CollisionNode
{
	extern bool debugVisible;
};

class INode
{
protected:
	virtual bool Update() = 0;
	virtual void Draw() = 0;

public:
	INode() = default;
	virtual ~INode() = default;
};

class CNode : public INode
{
protected:
	std::unique_ptr<SolidBody> body_;
	float localRadius_;
	Math::Vector3 trans_;
protected:
	virtual bool Update() override;
	virtual void Draw() override;

public:
	void ParentMatrix(const Math::Matrix *argParentMatrix);
	void LocalRadius(float argRadius) { localRadius_ = argRadius; }
	void Trans(const Math::Vector3 &argTrans) { trans_ = argTrans; }


public:
	CNode(Camera *argCamera, ID3D11DeviceContext *argImmediateContext, std::string argBodyName);
	virtual ~CNode();
};

class SphereNode : public CNode
{
protected:

	std::unique_ptr<Sphere> sphere_;
public:
	SphereNode(Camera *argCamera, ID3D11DeviceContext *argImmediateContext);
	~SphereNode();
public:
	bool Update() override;
	void Draw() override;

};

class CapsuleNode : public CNode
{
protected:
	std::unique_ptr<SolidBody> body_;
	float localLength_;
	float localRadius_;
	Math::Vector3 trans_;
	std::unique_ptr<Capsule> capsule_;

public:
	CapsuleNode(Camera *argCamera, ID3D11DeviceContext *argImmediateContext);
	~CapsuleNode();
	bool Update() override;
	void Draw() override;

	inline void Rot(const Math::Vector3 &argRot);
	inline void Length(float argLength) { localLength_ = argLength; }
	
};