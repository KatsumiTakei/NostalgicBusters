/**
*	@file	SceneManager.cpp
*	@date	2017 / 7 / 25
*	@author	Katsumi Takei
*	Copyright (c) Kastumi Takei. All rights reserved.
*/
#include "SceneManager.h"

#include "LoadScene.h"
#include "TitleScene.h"
#include "PlayScene.h"

class SceneManager::Impl
{
public:
	/**
	 *	@constructor	Impl
	 *	@brief			インプリメントクラス
	 *	@param[in]		argSceneManager		!<	シーンマネージャー
	 *	@param[in]		argStartSceneID		!<	スタートシーンのID
	 *	@param[in]		argDeviceResources	!<	デバイスリソース
	 */
	Impl(SceneManager *argSceneManager, eSceneID argStartSceneID, const std::shared_ptr<DeviceResources> &argDeviceResources)
		: scene_(nullptr), currentSceneID_(argStartSceneID), nextSceneID_(argStartSceneID), sceneManager_(argSceneManager),
		deviceResources_(argDeviceResources), shouldFade_(false), inputManager_(Singleton<InputManager>::Get()),
		rd2d_(Singleton<GraphManager>::Get()->RenderTarget()), update_([=] { return ChangeScene(); }), viewportManager_(Singleton<ViewportManager>::Get())
	{
	}

	~Impl() = default;

private:
	/**
	 *	@fn			CreateNextScene
	 *	@brief		次のシーンの生成
	 *	@return		次のシーンのunique_ptr
	 */
	std::unique_ptr<Scene> CreateNextScene()
	{
		std::unique_ptr<Scene> nextScene = nullptr;

		switch (nextSceneID_)
		{

		case eSceneID::Load:
			nextScene = std::make_unique<LoadScene>(sceneManager_, deviceResources_);
			break;

		case eSceneID::Title:
			nextScene = std::make_unique<TitleScene>(sceneManager_, deviceResources_);
			break;

		case eSceneID::Play:
			nextScene = std::make_unique<PlayScene>(sceneManager_, deviceResources_);
			break;
		}

		currentSceneID_ = nextSceneID_;
		nextSceneID_ = eSceneID::None;

		return nextScene;
	}


public:
	bool ChangeScene()
	{
		if (nextSceneID_ != eSceneID::None)
		{
			scene_.reset();
			scene_ = CreateNextScene();
			scene_->Update();
			update_ = [=]() { return FadeUpdate(); };

		}
		else
		{
			update_ = nullptr;
		}
		return (scene_ != nullptr);
	}

	/**
	 *	@fn			Update
	 *	@brief		更新
	 *	@retval		true	!<	シーンの更新、又はフェード中
	 *	@retval		false	!<	フェード終了
	 */
	bool Update()
	{
		return (update_) ? update_() : SceneUpdate();
	}

	bool SceneUpdate()
	{
		inputManager_->Update();
		return (scene_) ? scene_->Update() : ChangeScene();
	}

	bool FadeUpdate()
	{
		shouldFade_ = scene_->Fade()->Update();

		if (!shouldFade_)
		{
			shouldFade_ = true;
			update_ = [=] { return ChangeScene(); };
		}
		return shouldFade_;
	}

	/**
	 *	@fn			Draw
	 *	@brief		描画
	 */
	void Draw()
	{
		rd2d_->BeginDraw();
		scene_->Draw();
		rd2d_->EndDraw();

		if (!shouldFade_)
			return;

		auto context = deviceResources_->ImContext().Get();
		viewportManager_->RSSetViewports(context, 1, "Default");
		scene_->Fade()->Draw(context);

	}

private:
	ID2D1RenderTarget					*rd2d_;
	InputManager						*inputManager_;
	SceneManager						*sceneManager_;
	ViewportManager						*viewportManager_;
	std::shared_ptr<DeviceResources>	deviceResources_;
public:
	eSceneID							currentSceneID_, nextSceneID_;
	std::unique_ptr<Scene>				scene_;
	bool								shouldFade_;
	std::function<bool()>				update_;
};
/**
 *	@constructor	SceneManager
 *	@brief			シーンマネージャー
 *	@param[in]		argStartSceneID		!<	スタートシーンのID
 *	@param[in]		argDeviceResources	!<	デバイスリソース
 */
SceneManager::SceneManager(eSceneID argStartSceneID, const std::shared_ptr<DeviceResources> &argDeviceResources)
	:pImpl(std::make_unique<Impl>(this, argStartSceneID, argDeviceResources))
{
}

SceneManager::~SceneManager() = default;

/**
 *	@fn			FadeChangeScene
 *	@brief		シーンの変更
 *	@param[in]	argNextSceneID	!<	次のシーンのID
 */
void SceneManager::FadeChangeScene(eSceneID argNextSceneID)
{
	pImpl->nextSceneID_ = argNextSceneID;
	pImpl->shouldFade_ = true;
	pImpl->scene_->ChangeFade();
	pImpl->update_ = [=]() { return pImpl->FadeUpdate(); };
}

/**
 *	@fn			ChangeScene
 *	@brief		シーンの変更
 *	@param[in]	argNextSceneID	!<	次のシーンのID
 */
void SceneManager::ChangeScene(eSceneID argNextSceneID)
{
	pImpl->nextSceneID_ = argNextSceneID;
	pImpl->shouldFade_ = true;
	pImpl->scene_->ChangeFade();
	pImpl->update_ = [=]() { return pImpl->ChangeScene(); };
}
/**
 *	@fn			Update
 *	@brief		更新
 *	@retval		true	!<	シーンの更新、又はフェード中
 *	@retval		false	!<	フェード終了
 */
bool SceneManager::Update()
{
	return pImpl->Update();
}
/**
 *	@fn			Draw
 *	@brief		描画
 */
void SceneManager::Draw()
{
	pImpl->Draw();
}

eSceneID SceneManager::CurrentScene() const
{
	return pImpl->currentSceneID_;
}
