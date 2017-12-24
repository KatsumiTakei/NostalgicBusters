/**
 *	@file   SolidBody.cpp
 *	@date   2017 / 10 / 13
 *	@author Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "SolidBody.h"
#include "../Fbx/FbxModelManager.h"

class SolidBody::Impl
{

public:
	Impl(const std::string argModelName, Camera *camera, ID3D11DeviceContext *argImmediateContext)
		: model_(Singleton<FbxModelManager>::Get()->FindModel(argModelName)),
		parentMatrix_(nullptr), scale_(Math::Vector3(1,1,1)), trans_(Math::Vector3()), rot_(Math::Vector3()), isUseQuaternion_(false),
		localWorld_(Math::Matrix::Identity), rotQuaternion_(Math::Quaternion()), camera_(camera), immediateContext_(argImmediateContext)
	{
	}
	~Impl() = default;
public:
	bool Update()
	{
		Math::Matrix scalem = Math::Matrix::CreateScale(scale_);
		Math::Matrix rotm = Math::Matrix::Identity;
		Math::Matrix transm = Math::Matrix::CreateTranslation(trans_);

		rotm = (isUseQuaternion_) ? Math::Matrix::CreateFromQuaternion(rotQuaternion_) :
			Math::Matrix::CreateFromYawPitchRoll(rot_.y, rot_.x, rot_.z);

		localWorld_ = Math::Matrix::Identity;
		localWorld_ *= scalem * rotm * transm;

		if (parentMatrix_)
			localWorld_ *= (*parentMatrix_);
		
		if (model_)
			model_->Update(immediateContext_, delta_);

		return true;
	}

	void Draw()
	{
		assert(camera_ && "Not found camera...");
		const Math::Matrix& view = camera_->View();
		const Math::Matrix& projection = camera_->Proj();

		assert(immediateContext_ && "Not found context...");

		if (model_)
			model_->Draw(immediateContext_, localWorld_, view, projection);
		
	}
public:
	ID3D11DeviceContext			*immediateContext_;
	bool						isUseQuaternion_;
	Math::Quaternion			rotQuaternion_;
	Math::Vector3				scale_;
	Math::Vector3				trans_;
	Math::Vector3				rot_;
	Math::Matrix				localWorld_;
	const Math::Matrix			*parentMatrix_;
	float						delta_;
	std::function<void(void)>	func_;
private:
	Camera						*camera_;
	std::shared_ptr<FbxModel>	model_;

};

SolidBody::SolidBody(const std::string argModelName, Camera *argCamera, ID3D11DeviceContext *argImmediateContext)
	:Task(Singleton<TaskManager>::Get()->IsDelay()), pImpl(std::make_unique<Impl>(argModelName, argCamera, argImmediateContext))
{
}

SolidBody::~SolidBody() = default;


bool SolidBody::Update()
{
	return pImpl->Update();
}

void SolidBody::Draw()
{
	pImpl->Draw();
}

#pragma region SetterAndGetter
void SolidBody::Trans(const Math::Vector3& argTrans)
{
	pImpl->trans_ = argTrans;
}
const Math::Vector3& SolidBody::Trans()
{
	return pImpl->trans_;
}
void SolidBody::Rot(const Math::Vector3& argRot)
{
	pImpl->rot_ = argRot;
}
const Math::Vector3& SolidBody::Rot()
{
	return pImpl->rot_;
}
void SolidBody::Scale(const Math::Vector3& argScale)
{
	pImpl->scale_ = argScale;
}
const Math::Vector3& SolidBody::Scale()
{
	return pImpl->scale_;
}
void SolidBody::LocalWorld(const Math::Matrix& argMat)
{
	pImpl->localWorld_ = argMat;
}
const Math::Matrix& SolidBody::LocalWorld()
{
	return pImpl->localWorld_;
}
void SolidBody::RotQuternion(const Math::Quaternion argQuternion)
{
	pImpl->rotQuaternion_ = argQuternion;
}
const Math::Quaternion& SolidBody::RotQuternion()
{
	return pImpl->rotQuaternion_;
}

void SolidBody::IsUseQuternion(bool argFlag)
{
	pImpl->isUseQuaternion_ = argFlag;
}
void SolidBody::ParentMatrix(const Math::Matrix *argParentMatrix)
{
	pImpl->parentMatrix_ = argParentMatrix;
}
bool SolidBody::IsParentMatrix()
{
	return pImpl->parentMatrix_ ? true : false;
}
#pragma endregion