///**
// * @file   GameObject.cpp
// * @brief  GameObjectの定義
// * @author Katsumi Takei
// * @date   2017 / 01 / 20
// */
//
//#include "GameObject.h"
//
//#include "Collision/Collision.h"
//
//#include "../GraphicManager/Graphic.h"
//#include "Sound/SoundManager.h"
//#include "Utility/Gauge/CircleGauge.h"
//#include "Utility/Debug.h"
//
//using namespace std;
//
//list<GameObject*> GameObject::objList;
//
//#pragma region CreaterAndDestroyer
//
//GameObject::GameObject()
//:collisionShape_(pos.x, pos.y, radius_),isMaxPowerState_(false)
//,isBonusGet_(false),type_(ITEM_POWER), id_(OBJECT_NONE)
//{
//	priority_ = PRIORITY_PLAYSCREEN;
//	mode_ = DRAW_ALPHA;
//}
//#pragma endregion
//#pragma region PrivateFunction
//
//#pragma endregion
//#pragma region Collision
//
//void GameObject::ClearObjectList()
//{
//	objList.clear();
//}
//
//void GameObject::ObjectOnObject()
//{
//	for (list<GameObject*>::iterator it = objList.begin(); it != objList.end(); ++it)
//	{
//		GameObject *leftTask = *it;
//		// オブジェクトが死んでたら判定を飛ばす
//		if (!leftTask->IsLiving()) continue;
//
//		for (list<GameObject*>::iterator itr = objList.begin(); itr != objList.end(); ++itr)
//		{
//			GameObject *rightTask = *itr;
//			if (!rightTask->IsLiving()) continue;
//
//			if (CircleToCircle(leftTask->collisionShape_, rightTask->collisionShape_))
//			{// 円vs円で当たれば
//				leftTask->flexibleCollision(rightTask);
//			}
//		}
//	}
//
//	for (list<GameObject*>::iterator it = objList.begin(); it != objList.end();)
//	{
//		GameObject *task = *it;
//
//		if (!task->IsLiving())
//		{
//			it = objList.erase(it);
//			continue;
//		}
//		++it;
//	}
//}
//#pragma endregion
//#pragma region PublicFunction
//
//void GameObject::Reset()
//{
//	objList.push_back(this);
//	collisionShape_ = CircleShape(pos.x, pos.y, radius_);
//}
//
//bool GameObject::Update()
//{
//	collisionShape_ = CircleShape(pos.x, pos.y, radius_);
//
//	return true;
//}
//
//void GameObject::Draw()
//{
//#if defined(DEBUG) || defined(_DEBUG)
//	CircleGauge gauge;
//	gauge.x = pos.x;
//	gauge.y = pos.y;
//	gauge.radius(radius_);
//	gauge.value = 1;
//	gauge.gaugeType(GAUGE_FILL);
//
//	gauge.Draw(GraphManager::instance()->renderTarget());
//
//#endif 
//}
//#pragma endregion