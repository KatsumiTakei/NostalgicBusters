/**
 *	@file	CollisionNode3D.cpp
 *	@date	2017 / 7 / 15
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "CollisionNode3D.h"

#include "../SolidObject/SolidBody.h"

namespace CollisionNode
{
	bool debugVisible = false;
};

CNode::CNode(Camera *argCamera, ID3D11DeviceContext *argImmediateContext, std::string argBodyName)
	:body_(std::make_unique<SolidBody>(argBodyName.c_str(), argCamera, argImmediateContext)), localRadius_(1.0f)
{
}

CNode::~CNode() = default;

void CNode::ParentMatrix(const Math::Matrix *argParentMatrix)
{
	body_->ParentMatrix(argParentMatrix);
}

bool CNode::Update()
{
	return true;
}

void CNode::Draw()
{
	if (CollisionNode::debugVisible)
		body_->Draw();
}

SphereNode::SphereNode(Camera *argCamera, ID3D11DeviceContext *argImmediateContext)
	:CNode(argCamera, argImmediateContext, "sphere")
{
}

SphereNode::~SphereNode() = default;

bool SphereNode::Update()
{
	bool result = true;

	body_->Trans(trans_);
	Math::Vector3 localScale;

	localScale.x = localRadius_;
	localScale.y = localRadius_;
	localScale.z = localRadius_;

	body_->Scale(localScale);
	result = body_->Update();

	Math::Matrix worldm = body_->LocalWorld();

	Math::Vector3 center(0.0f, 0.0f, 0.0f);
	Math::Vector3 right(1.0f, 0.0f, 0.0f);

	center = Math::Vector3::Transform(center, worldm);
	right = Math::Vector3::Transform(right, worldm);

	sphere_->center = center;
	sphere_->radius = Math::Vector3::Distance(center, right);

	return true;
}

void SphereNode::Draw()
{
	CNode::Draw();
}

CapsuleNode::CapsuleNode(Camera *argCamera, ID3D11DeviceContext *argImmediateContext)
	:CNode(argCamera, argImmediateContext, "capsule"), localRadius_(1.0f), localLength_(1.0f)
{
}

CapsuleNode::~CapsuleNode() = default;

bool CapsuleNode::Update()
{
	bool result = true;
	body_->Trans(trans_);
	Math::Vector3 localScale;

	localScale.x = localRadius_;
	localScale.z = localRadius_;
	localScale.y = localLength_;

	body_->Scale(localScale);
	result = body_->Update();

	Math::Matrix worldm = body_->LocalWorld();

	Math::Vector3 center(0.0f, 0.0f, 0.0f);
	Math::Vector3 up(0.0f, 1.0f, 0.0f);
	Math::Vector3 right(1.0f, 0.0f, 0.0f);

	center = Math::Vector3::Transform(center, worldm);
	up = Math::Vector3::Transform(up, worldm);
	right = Math::Vector3::Transform(right, worldm);

	capsule_->segment.start = center;
	capsule_->segment.end = up;

	capsule_->radius = Math::Vector3::Distance(center, right);

	return result;
}

void CapsuleNode::Draw()
{
	CNode::Draw();
}

void CapsuleNode::Rot(const Math::Vector3 &argRot)
{
	body_->Rot(argRot);
}
