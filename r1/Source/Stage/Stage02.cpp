///**
// * @file   Stage02.cpp
// * @brief  Stage02の定義
// * @author Katsumi Takei
// * @date   2017 / 02 / 24
// */
//#include "Stage02.h"
//
//#include "../Camera/Camera.h"
//#include "../SolidObject/SolidObject.h"
//#include "../InputManager/InputManager.h"
//#include "../Enemy/EnemyFactory/EnemyFactory.h"
//
//#include "../Constant.h"
//#include "../Function.h"
//#include "../Utility/GraphManager.h"
//#include "../Sound/SoundManager.h"
//#include "../Utility/Debug.h"
//
//using namespace std;
//using namespace Constant;
//using namespace DirectX;
//using namespace DirectX::SimpleMath;
//
//namespace
//{
//	enum
//	{
//		SkyFoward,
//		SkyDepth,
//
//		ModelNum,
//	};
//}
//
//#pragma region CreaterAndDestroyer
//
//class Stage02::Impl
//{
//#pragma region CreaterAndDestroyer
//public:
//	Impl(Camera *camera)
//	{
//		model_[SkyFoward].reset(new SolidObject(L"Stage02.cmo", camera));
//		model_[SkyDepth].reset(new SolidObject(L"Stage02.cmo", camera));
//
//		for (int i = 0; i < ModelNum; ++i)
//		{
//			assert(model_[i] && "ModelData Loading Failed");
//		}
//	}
//
//	~Impl()
//	{
//	}
//#pragma endregion
//
//#pragma region Object
//public:
//
//	unique_ptr<SolidObject> model_[ModelNum];
//
//#pragma endregion
//};
//
//Stage02::Stage02()
//:pImpl(new Impl(camera_.get()))
//{
//	enemyFileName_ = CreatePath(EnemyDataPath, L"enemy.map");
//	InitializeBackGround();
//	InitializeEnemy();
//	count_ = (-5) * FLAME_TIME;
//	is_living_ = true;
//	mode_ = DRAW_NONE;
//	priority_ = PRIORITY_BACKGROUND;
//}
//
//Stage02::~Stage02()
//{
//}
//
//#pragma endregion
//#pragma region ProtectedFunction
//
//void Stage02::InitializeBackGround()
//{
//	//pImpl->model_[SkyFoward]->scale(0.5f);
//	//pImpl->model_[SkyDepth]->scale(0.5f);
//
//	pImpl->model_[SkyDepth]->trans(Vector3(0.f, -1.f, 157.f));
//
//}
//
//void Stage02::ScrollBack()
//{
//	const float StartDepth = -157.f;
//	const Vector3 moveSpd = Vector3(0.f, 0.f, -0.5f);
//
//	if (pImpl->model_[SkyFoward]->trans().z <= StartDepth)
//	{// ループ処理
//		Vector3 trans = Vector3(0.f, 0.f, -(pImpl->model_[SkyDepth]->trans().z + StartDepth));
//		pImpl->model_[SkyFoward]->trans(trans);
//	}
//	if (pImpl->model_[SkyDepth]->trans().z <= StartDepth)
//	{// ループ処理
//		Vector3 trans = Vector3(0.f, -1.f, -(pImpl->model_[SkyFoward]->trans().z + StartDepth));
//		pImpl->model_[SkyDepth]->trans(trans);
//	}
//	//for (int i = Big; i < Small + 1; ++i)
//	//{
//	//	if (pImpl->model_[i]->trans().z <= -50)
//	//	{
//	//		Vector3 trans = Vector3(pImpl->model_[i]->trans().x, pImpl->model_[i]->trans().y, 50.f);
//	//		pImpl->model_[i]->trans(trans);
//	//	}
//	//}
//
//	for (int i = 0; i < ModelNum; ++i)
//	{
//		Vector3 trans = pImpl->model_[i]->trans() + moveSpd;
//		pImpl->model_[i]->trans(trans);
//	}
//}
//#pragma endregion
//
//#pragma region PublicFunction
//
//void Stage02::initialize()
//{
//	SetEnemy();
//}
//
//bool Stage02::update()
//{
//#if defined(DEBUG) || defined(_DEBUG)
//	if (InputManager::instance()->IsTouched(KEY_DEBUG))
//	{
//	}
//#endif
//
//	camera_->UpDecCamera();
//	Stage::update();
//
//	for (int i = 0; i < ModelNum; ++i)
//	{
//		pImpl->model_[i]->Update();
//	}
//
//	return true;
//}
//
//void Stage02::draw()
//{
//	for (int i = 0; i < ModelNum; ++i)
//	{
//		pImpl->model_[i]->Draw();
//	}
//}
//
//void Stage02::finalize()
//{
//}
//
//#pragma endregion