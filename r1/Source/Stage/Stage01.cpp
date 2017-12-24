/**
 *	@file	Stage01.cpp
 *	@date	2017 / 7 / 26
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "Stage01.h"
#include "../SolidObject/SolidBody.h"
#include "../Enemy/EnemyFactory/EnemyManager.h"

class Stage01::Impl
{
public:

	Impl(Stage01 *argStage, const std::weak_ptr<DeviceResources> & argDeviceResources)
		:stage_(argStage)
	{
		assert(!argDeviceResources.expired() && "Already Delete DeviceResources");
		context_ = argDeviceResources._Get()->ImContext().Get();

		model_[eModel::Big] = std::make_unique<SolidBody>("Cloud", argStage->camera_.get(), context_);
		model_[eModel::Middle] = std::make_unique<SolidBody>("Cloud", argStage->camera_.get(), context_);
		model_[eModel::Small] = std::make_unique<SolidBody>("Cloud", argStage->camera_.get(), context_);
		model_[eModel::SkyFoward] = std::make_unique<SolidBody>("Stage01", argStage->camera_.get(), context_);
		model_[eModel::SkyDepth] = std::make_unique<SolidBody>("Stage01", argStage->camera_.get(), context_);

		for (int lModelIndex = 0; lModelIndex < eModel::Max; ++lModelIndex)
		{
			assert(model_[lModelIndex] && "ModelData Loading Failed");
		}

		model_[eModel::SkyFoward]->Trans(Math::Vector3(0.f, -95.f, 0.f));
		model_[eModel::SkyFoward]->Scale(Math::Vector3(2.f, 2.f, 2.f));
		model_[eModel::SkyDepth]->Trans(Math::Vector3(0.f, 0.f, -1.f));
		model_[eModel::SkyDepth]->Scale(Math::Vector3(2.f, 2.f, 2.f));
		
		model_[eModel::Big]->Rot(Math::Vector3(Math::ToRadian(180), 1.f, 1.f));

		model_[eModel::Middle]->Trans(Math::Vector3(-1.f, -10.f, 1.5f));
		model_[eModel::Middle]->Scale(Math::Vector3(0.5f, 0.5f, 0.5f));
		model_[eModel::Middle]->Rot(Math::Vector3(Math::ToRadian(180), 1.f, 1.f));
		
		model_[eModel::Small]->Trans(Math::Vector3(3.f, -5.f, 2.5f));
		model_[eModel::Small]->Scale(Math::Vector3(0.25f, 0.25f, 0.25f));
		model_[eModel::Small]->Rot(Math::Vector3(Math::ToRadian(90), 1.f, 1.f));
	}

	~Impl() = default;

public:

	void ScrollBack()
	{
		const float StartDepth = -150.f;
		const Math::Vector3 MoveSpd = Math::Vector3(0.f, -0.05f, 0.f);

		if (model_[eModel::SkyFoward]->Trans().y >= StartDepth)
		{// ループ処理
			Math::Vector3 trans = Math::Vector3(0.f, -(StartDepth * 0.5f), 0.f);
			model_[eModel::SkyFoward]->Trans(trans);
		}
		if (model_[eModel::SkyDepth]->Trans().y >= StartDepth)
		{// ループ処理
			Math::Vector3 trans = Math::Vector3(0.f, -(StartDepth * 0.5f), -1.f);
			model_[eModel::SkyDepth]->Trans(trans);
		}
		for (int lCloudIndex = eModel::Big; lCloudIndex < eModel::Small + 1; ++lCloudIndex)
		{
			if (model_[lCloudIndex]->Trans().y <= -7)
			{
				Math::Vector3 trans = Math::Vector3(model_[lCloudIndex]->Trans().x, 10.f, model_[lCloudIndex]->Trans().z);
				model_[lCloudIndex]->Trans(trans);
			}
		}

		for (int lModelIndex = 0; lModelIndex < eModel::Max; ++lModelIndex)
		{
			Math::Vector3 trans = model_[lModelIndex]->Trans() + MoveSpd;
			model_[lModelIndex]->Trans(trans);
		}
	}

public:
	bool Update()
	{
		for (int lModelIndex = 0; lModelIndex < eModel::Max; ++lModelIndex)
		{
			model_[lModelIndex]->Update();
		}
		return true;
	}

	void Draw()
	{
		for (int lModelIndex = 0; lModelIndex < eModel::Max; ++lModelIndex)
		{
			model_[lModelIndex]->Draw();
		}
	}

private:
	enum eModel
	{
		SkyFoward,
		SkyDepth,
		Big,
		Middle,
		Small,

		Max,
	};

public:

	std::unique_ptr<SolidBody> model_[eModel::Max];
	ID3D11DeviceContext *context_;
	Stage01 *stage_;
};

Stage01::Stage01(Player *argPlayer, const std::shared_ptr<DeviceResources> &argDeviceResources, Stage *argNextStage, std::string argSoundFileName, std::string argViewPortName)
:Stage(argPlayer, argNextStage, L"stage01", argSoundFileName, argViewPortName), pImpl(std::make_unique<Impl>(this, argDeviceResources))
{
}

Stage01::~Stage01() = default;

void Stage01::ScrollBack()
{
	pImpl->ScrollBack();
}

bool Stage01::Update()
{
	camera_->Update();
	Stage::Update();

	return pImpl->Update();
}

void Stage01::Draw()
{
	pImpl->Draw();
}