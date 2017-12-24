
#include "Obstacle.h"

#include "../../GameObject/GameObject2D.h"
#include "../Player.h"

class ObstacleManager::Impl
{
public:
	Impl()
	{
		for (int lIndex = 0; lIndex < static_cast<int>(eSide::Max); lIndex++)
		{
			obstacles_[0].reserve(1000);
			obstacles_[1].reserve(1000);
		}
	}

	~Impl() = default;

public:
	std::vector<GameObject2D*> obstacles_[static_cast<int>(eSide::Max)];
private:

};

ObstacleManager::ObstacleManager()
	:pImpl(std::make_unique<Impl>())
{
}

ObstacleManager::~ObstacleManager() = default;

void ObstacleManager::Entry(GameObject2D * argObstacle, bool argIsDelay)
{
	const int GameSide = (argIsDelay) ? 1 : 0;
	pImpl->obstacles_[GameSide].push_back(argObstacle);
}

void ObstacleManager::Entry(GameObject2D * argObstacle, int argGameSide)
{
	pImpl->obstacles_[argGameSide].push_back(argObstacle);
}

void ObstacleManager::Clear(bool argIsDelay)
{
	const int GameSide = (argIsDelay) ? 1 : 0;
	pImpl->obstacles_[GameSide].clear();
}

void ObstacleManager::Clear()
{
	for (int lIndex = 0; lIndex < static_cast<int>(eSide::Max); lIndex++)
	{
		pImpl->obstacles_[lIndex].clear();
	}
}

std::vector<GameObject2D*> ObstacleManager::Obstacles(bool argIsDelay) const
{
	const int GameSide = (argIsDelay) ? 1 : 0;
	return pImpl->obstacles_[GameSide];
}
