/**
 *	@file   SolidObject.cpp
 *	@date   2017 / 07 / 26
 *	@author Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "SolidObject.h"
#include "../SolidObject/SolidBody.h"
#include "../Collision/CollisionNode3D.h"

class SolidObject::Impl
{
public:
	Impl(const std::string argModelName, Camera *argCamera, ID3D11DeviceContext *argImmediateContext)
		:body_(std::make_unique<SolidBody>(argModelName, argCamera, argImmediateContext))
	{
		sphere = std::make_unique<SphereNode>(argCamera, argImmediateContext);
		sphere->ParentMatrix(&body_->LocalWorld());
		sphere->LocalRadius(3.f);
	}

	~Impl() = default;

public:
	void Update()
	{
		sphere->Update();
		body_->Update();
	}

	void Draw()
	{
		sphere->Draw();
		body_->Draw();
	}

public:
	std::unique_ptr<SolidBody> body_;
	std::unique_ptr<SphereNode> sphere;
};

SolidObject::SolidObject(const std::string argModelName, Camera *argCamera, ID3D11DeviceContext *argDeviceConext)
	:pImpl(new Impl(argModelName, argCamera, argDeviceConext))
{
}

SolidObject::~SolidObject() = default;

bool SolidObject::Update()
{
	pImpl->Update();
	return true;
}

void SolidObject::Draw()
{
	pImpl->Draw();
}

SolidBody *SolidObject::Body()const
{
	return pImpl->body_.get();
}

SphereNode *SolidObject::CollisionNode()const
{
	return pImpl->sphere.get();
}
