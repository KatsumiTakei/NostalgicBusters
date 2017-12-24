/**
 *	@file	EnemyManager.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */

#include "EnemyManager.h"
#include "../Enemy.h"
#include "../SmallEnemy.h"
#include "../Boss.h"
#include "../../Item/Item.h"

class EnemyManager::Impl
{
public:
	Impl()
		:dataPath_(L"Resources/Data/"), canUseArchive_(false)
	{
	}
	~Impl() = default;

public:

	bool LoadEnemy(const std::wstring argFilename, Player *argPlayer)
	{
		const std::wstring Filename = dataPath_ + argFilename + L".map";

		std::vector<std::shared_ptr<Enemy>> enemyList;
		std::vector<EnemyData> enemyDataList;
		int enemyMax = 0;
		EnemyId enemyId;
		eItemId itemId;

		if (!canUseArchive_)
		{
			std::fstream fs(Filename.c_str(), std::ios::binary | std::ios::in);
			if (!fs)
				return false;

			fs.read(reinterpret_cast<char*>(&enemyMax), sizeof(int));

			for (int lEnemyIndex = 0; lEnemyIndex < enemyMax; ++lEnemyIndex)
			{
				enemyDataList.push_back(EnemyData());
				fs.read(reinterpret_cast<char*>(&enemyId), sizeof(int));
				fs.read(reinterpret_cast<char*>(&itemId), sizeof(int));
				fs.read(reinterpret_cast<char*>(&enemyDataList[lEnemyIndex]), sizeof(EnemyData));

				switch (enemyId)
				{
				case EnemyId::Small:
					enemyList.push_back(std::make_shared<SmallEnemy>(argPlayer, itemId));
					break;

				case  EnemyId::Boss:
					//EnemyListInfo.push_back(std::make_shared<Boss>(argPlayer));
					break;
				default:
					break;
				}
			}

			fs.close();
		}
		else
		{
			const char *text = Singleton<Archive>::Get()->LoadText(String::ToString(Filename).c_str());

			unsigned int readPos = 0;
			std::memcpy(&enemyMax, &text[readPos], sizeof(int));
			readPos += sizeof(int);

			for (int lEnemyIndex = 0; lEnemyIndex < enemyMax; ++lEnemyIndex)
			{
				enemyDataList.push_back(EnemyData());
				std::memcpy(&enemyId, &text[readPos], sizeof(int));
				readPos += sizeof(int);
				std::memcpy(&itemId, &text[readPos], sizeof(int));
				readPos += sizeof(int);
				std::memcpy(&enemyDataList[lEnemyIndex], &text[readPos], sizeof(EnemyData));
				readPos += sizeof(EnemyData);

				switch (enemyId)
				{
				case EnemyId::Small:
					enemyList.push_back(std::make_shared<SmallEnemy>(argPlayer, itemId));
					break;

				case  EnemyId::Boss:
				//	EnemyListInfo.push_back(std::make_shared<Boss>(argPlayer));
					break;
				default:
					break;
				}
			}
		}

		enemyList_.insert(std::make_pair(argPlayer, enemyList));
		enemyDataList_.insert(std::make_pair(argPlayer, enemyDataList));

		return true;
	}

	void DestroyActiveEnemis()
	{
		for (auto lEnemyMap : enemyList_)
		{
			for (auto lEnemy : lEnemyMap.second)
			{
				lEnemy->IsLiving(false);
			}
		}
	}

private:
	enum class EnemyId : int
	{
		Small,
		Boss,
	};
public:
	std::map<Player*, std::vector<std::shared_ptr<Enemy>>> enemyList_;
	std::map<Player*, std::vector<EnemyData>> enemyDataList_;
	std::wstring dataPath_;
	bool canUseArchive_;
};


EnemyManager::EnemyManager()
	:pImpl(std::make_unique<Impl>())
{

}
EnemyManager::~EnemyManager() = default;

std::vector<std::shared_ptr<Enemy>> EnemyManager::EnemyList(Player *argPlayer) const
{
	return pImpl->enemyList_.at(argPlayer);
}

void EnemyManager::EnemyList(Player *argPlayer, std::vector<std::shared_ptr<Enemy>> argEnemyList)
{
	pImpl->enemyList_.insert(std::make_pair(argPlayer, argEnemyList));
}

std::vector<EnemyData> EnemyManager::EnemyDataList(Player *argPlayer) const
{
	return pImpl->enemyDataList_.at(argPlayer);
}

void EnemyManager::ClearEnemyList()
{
	pImpl->enemyList_.clear();
}

void EnemyManager::DestroyActiveEnemis()
{
	pImpl->DestroyActiveEnemis();
}

std::wstring EnemyManager::DataPath()
{
	return pImpl->dataPath_;
}

void EnemyManager::DataPath(std::wstring argPath)
{
	pImpl->dataPath_ = argPath;
}

void EnemyManager::CanUseArchive(bool argCanUseArchive)
{
	pImpl->canUseArchive_ = argCanUseArchive;
}

bool EnemyManager::LoadEnemy(const std::wstring argFilename, Player *argPlayer)
{
	return pImpl->LoadEnemy(argFilename, argPlayer);
}
