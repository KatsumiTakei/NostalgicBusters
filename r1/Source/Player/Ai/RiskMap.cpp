/**
 *	@file	RiskMap.cpp
 *	@date	2017 / 11 / 13
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "RiskMap.h"
#include "RiskGrid.h"

RiskMap::RiskMap()
	:graph_(Singleton<GraphManager>::Get()->FindIndexTexture(L"Number"))
{
	for (int lIndexX = 0; lIndexX < GridMax; lIndexX++)
	{
		for (int lIndexY = 0; lIndexY < GridMax; lIndexY++)
		{
			grids_[lIndexX][lIndexY] = new RiskGrid(Math::Vector2(static_cast<float>(lIndexX), static_cast<float>(lIndexY)), GridSize, 0);
			gridSizeList[lIndexX][lIndexY] = grids_[lIndexX][lIndexY]->CornerRect();
		}
	}
	Reset();
}

RiskMap::~RiskMap()
{
	for (int lIndexX = 0; lIndexX < GridMax; lIndexX++)
	{
		for (int lIndexY = 0; lIndexY < GridMax; lIndexY++)
		{
			SafeDelete(grids_[lIndexX][lIndexY]);
		}
	}
}

void RiskMap::Reset()
{
	RefreshRiskPoint();

	for (int lIndexX = 0; lIndexX < GridMax; lIndexX++)
	{
		for (int lIndexY = 0; lIndexY < GridMax; lIndexY++)
		{
			SetAroundGrid(grids_[lIndexX][lIndexY]);
		}
	}
}

void RiskMap::RefreshRiskPoint()
{
	for (int lIndexX = 0; lIndexX < GridMax; lIndexX++)
	{
		for (int lIndexY = 0; lIndexY < GridMax; lIndexY++)
		{
			grids_[lIndexX][lIndexY]->RiskPoint(0);
			grids_[lIndexX][lIndexY]->IsHitting(false);

			if (IsEdge(lIndexX, lIndexY))
			{//	隅は危ないので危険度＋
				grids_[lIndexX][lIndexY]->PulsRiskPoint(3);

				if (IsCornersPoint(lIndexX, lIndexY))
				{//	四隅はさらに危ないので危険度＋
					grids_[lIndexX][lIndexY]->PulsRiskPoint(5);
				}
			}
		}
	}
}

void RiskMap::SetAroundGrid(RiskGrid *argGrid)const
{
	const Math::Vector2 Index = argGrid->Index();

	const int IndexX[MovePattern] = { static_cast<int>(Index.x), static_cast<int>(Index.x) + 1, static_cast<int>(Index.x) - 1 };
	const int IndexY[MovePattern] = { static_cast<int>(Index.y), static_cast<int>(Index.y) + 1, static_cast<int>(Index.y) - 1 };

	int aroundIndex = 0;

	for (int lIndexX = 0; lIndexX < MovePattern; lIndexX++)
	{
		const int GridX = IndexX[lIndexX];

		if (!IsInRange(GridX, 0, GridMax - 1))
			continue;

		for (int lIndexY = 0; lIndexY < MovePattern; lIndexY++)
		{
			if (lIndexX == 0 && lIndexY == 0)
				continue;

			const int GridY = IndexY[lIndexY];

			if (!IsInRange(GridY, 0, GridMax - 1))
				continue;

			argGrid->AroundRisk(grids_[GridX][GridY]->RiskPoint(), aroundIndex);
			aroundIndex++;
		}
	}
}

bool RiskMap::IsInGrid(Math::Vector2 argPos, int argIndexX, int argIndexY) const
{
	return IsInArea(argPos, gridSizeList[argIndexX][argIndexY]);
}

RiskGrid* RiskMap::CurrentGrid(Math::Vector2 argPos)const
{
	const Math::Vector2 Index = CurrentIndex(argPos);

	const int IndexX = static_cast<int>(Index.x);
	const int IndexY = static_cast<int>(Index.y);

	assert(IsInGrid(argPos, IndexX, IndexY) && "not in grid...");

	return grids_[IndexX][IndexY];
}

bool RiskMap::IsEdge(int argIndexX, int argIndexY)
{
	if ((argIndexX == 0) || (argIndexX == GridMax - 1) ||
		(argIndexY == 0) || (argIndexY == GridMax - 1))
	{
		return true;
	}
	return false;
}

bool RiskMap::IsEdge(Math::Vector2 argPos)
{
	const Math::Vector2 Index = CurrentIndex(argPos);
	return IsEdge(static_cast<int>(Index.x), static_cast<int>(Index.y));
}

bool RiskMap::IsCornersPoint(int argIndexX, int argIndexY)
{
	if ((argIndexX == 0 && argIndexY == 0) || (argIndexX == 0 && argIndexY == GridMax - 1) ||
		(argIndexX == GridMax - 1 && argIndexY == 0) || (argIndexX == GridMax - 1 && argIndexY == GridMax - 1))
	{
		return true;
	}
	return false;
}

bool RiskMap::IsCornersPoint(Math::Vector2 argPos)
{
	const Math::Vector2 Index = CurrentIndex(argPos);
	return IsCornersPoint(static_cast<int>(Index.x), static_cast<int>(Index.y));
}

RiskGrid * RiskMap::InObstacle(Math::Vector2 argPos)
{
	if (!IsInArea(argPos, Area))
		return nullptr;

	auto current = CurrentGrid(argPos);
	assert(current && "CurrentGrid not found...");
	current->PulsRiskPoint(1);
	current->IsHitting(true);

	return current;
}


RiskGrid* RiskMap::GetGridIndex(int argIndexX, int argIndexY)const
{
	return grids_[argIndexX][argIndexY];
}

RiskGrid* RiskMap::GetGridIndex(Math::Vector2 argIndex)const
{
	return grids_[static_cast<int>(std::floor(argIndex.x))][static_cast<int>(std::floor(argIndex.y))];
}

RiskGrid* RiskMap::SearchSafeGrid(Math::Vector2 argPos)const
{
	const Math::Vector2 Index = CurrentIndex(argPos);

	int range = 1;
	const Math::Vector2 OutsideMax = { Math::Clamp(Index.x + static_cast<float>(range), Index.x, static_cast<float>(GridMax - 1)) , Math::Clamp(Index.y + static_cast<float>(range), Index.y, static_cast<float>(GridMax - 1)) };
	const Math::Vector2 OutsideMin = { Math::Clamp(Index.x - static_cast<float>(range), 0.f,static_cast<float>(GridMax - 1)) , Math::Clamp(Index.y - static_cast<float>(range), 0.f,static_cast<float>(GridMax) - 1) };
#if 0
	range = 2;

	const Math::Vector2 Max = { Math::Clamp(Index.x + static_cast<float>(range), Index.x, GridMax - 1) , Math::Clamp(Index.y + static_cast<float>(range), Index.y, static_cast<float>(GridMax - 1)) };
	const Math::Vector2 Min = { Math::Clamp(Index.x - static_cast<float>(range), 0.f ,static_cast<float>(GridMax - 1)) , Math::Clamp(Index.y - static_cast<float>(range), 0.f ,static_cast<float>(GridMax - 1)) };

	const int Pattern = 5;

	const int IndexX[Pattern] = { static_cast<int>(Index.x), static_cast<int>(OutsideMax.x), static_cast<int>(OutsideMin.x), static_cast<int>(Max.x), static_cast<int>(Min.x) };
	const int IndexY[Pattern] = { static_cast<int>(Index.y), static_cast<int>(OutsideMax.y), static_cast<int>(OutsideMin.y), static_cast<int>(Max.y), static_cast<int>(Min.y) };
#endif

	const int IndexX[MovePattern] = { static_cast<int>(Index.x), static_cast<int>(OutsideMax.x), static_cast<int>(OutsideMin.x) };
	const int IndexY[MovePattern] = { static_cast<int>(Index.y), static_cast<int>(OutsideMax.y), static_cast<int>(OutsideMin.y) };

	int mustSafe = 9999;
	RiskGrid* safeGrid = nullptr;

	for (int lIndexX = 0; lIndexX < MovePattern; lIndexX++)
	{
		const int GridX = IndexX[lIndexX];

		for (int lIndexY = 0; lIndexY < MovePattern; lIndexY++)
		{
			const int GridY = IndexY[lIndexY];

			if (grids_[GridX][GridY]->Risk() < mustSafe)
			{
				mustSafe = grids_[GridX][GridY]->Risk();
				safeGrid = grids_[GridX][GridY];
			}
		}
	}

	return safeGrid;
}

RiskGrid * RiskMap::SearchSafeGrid(Math::Vector2 argPos, int argRange) const
{
	const Math::Vector2 Index = CurrentIndex(argPos);

	std::vector<int> IndexX, IndexY;
	IndexX.push_back(static_cast<int>(Index.x));
	IndexY.push_back(static_cast<int>(Index.y));
	for (int lIndex = argRange; 0 <= lIndex; lIndex--)
	{
		IndexX.push_back(static_cast<int>(Math::Clamp(static_cast<float>(Index.x + lIndex), static_cast<float>(Index.x), static_cast<float>(GridMax - 1))));
		IndexX.push_back(static_cast<int>(Math::Clamp(static_cast<float>(Index.x - lIndex), 0.f, static_cast<float>(Index.x - 1))));
		IndexY.push_back(static_cast<int>(Math::Clamp(static_cast<float>(Index.y + lIndex), static_cast<float>(Index.y), static_cast<float>(GridMax - 1))));
		IndexY.push_back(static_cast<int>(Math::Clamp(static_cast<float>(Index.y - lIndex), 0.f, static_cast<float>(Index.y - 1))));
	}

	int mustSafe = 9999;
	RiskGrid* safeGrid = nullptr;

	for (int lIndexX = 0, lIndexXMax = IndexX.size(); lIndexX < lIndexXMax; lIndexX++)
	{
		const int GridX = IndexX[lIndexX];

		for (int lIndexY = 0, lIndexYMax = IndexY.size(); lIndexY < lIndexYMax; lIndexY++)
		{
			const int GridY = IndexY[lIndexY];

			if (grids_[GridX][GridY]->Risk() < mustSafe)
			{
				mustSafe = grids_[GridX][GridY]->Risk();
				safeGrid = grids_[GridX][GridY];
			}
		}
	}

	return safeGrid;
}

bool RiskMap::IsInAround(RiskGrid * argSrc, RiskGrid * argTar)
{
	bool isAround = false;
	const Math::Vector2 Index = argSrc->Index();
	float range = 1.f;

	const Math::Vector2 Max = { Math::Clamp(Index.x + range, Index.x, static_cast<float>(GridMax - 1)) , Math::Clamp(Index.y + range, Index.y, static_cast<float>(GridMax - 1)) };
	const Math::Vector2 Min = { Math::Clamp(Index.x - range, 0.f,static_cast<float>(GridMax - 1)) , Math::Clamp(Index.y - range, 0.f,static_cast<float>(GridMax) - 1) };

	const int IndexX[MovePattern] = { static_cast<int>(Index.x), static_cast<int>(Max.x), static_cast<int>(Min.x) };
	const int IndexY[MovePattern] = { static_cast<int>(Index.y), static_cast<int>(Max.y), static_cast<int>(Min.y) };

	for (int lIndexX = 0; lIndexX < MovePattern; lIndexX++)
	{
		const int GridX = IndexX[lIndexX];

		for (int lIndexY = 0; lIndexY < MovePattern; lIndexY++)
		{
			if (lIndexX == 0 && lIndexY == 0)
				continue;

			const int GridY = IndexY[lIndexY];

			if (grids_[GridX][GridY] == argTar)
			{
				isAround = true;
				break;
			}
		}
	}

	return isAround;
}

bool RiskMap::IsInAroundOrSame(RiskGrid * argSrc, RiskGrid * argTar)
{
	bool isAround = false;
	const Math::Vector2 Index = argSrc->Index();
	float range = 1.f;

	const Math::Vector2 Max = { Math::Clamp(Index.x + range, Index.x, static_cast<float>(GridMax - 1)) , Math::Clamp(Index.y + range, Index.y, static_cast<float>(GridMax - 1)) };
	const Math::Vector2 Min = { Math::Clamp(Index.x - range, 0.f,static_cast<float>(GridMax - 1)) , Math::Clamp(Index.y - range, 0.f,static_cast<float>(GridMax) - 1) };

	const int IndexX[MovePattern] = { static_cast<int>(Index.x), static_cast<int>(Max.x), static_cast<int>(Min.x) };
	const int IndexY[MovePattern] = { static_cast<int>(Index.y), static_cast<int>(Max.y), static_cast<int>(Min.y) };

	const int ArgIndexX = argTar->Index().x;
	const int ArgIndexY = argTar->Index().y;

	for (int lIndexX = 0; lIndexX < MovePattern; lIndexX++)
	{
		if (IndexX[lIndexX] != ArgIndexX)
			continue;

		for (int lIndexY = 0; lIndexY < MovePattern; lIndexY++)
		{
			if (IndexY[lIndexY] != ArgIndexY)
				continue;
		
			isAround = true;
			break;
		}
	}

	return isAround;
}

Math::Vector2 RiskMap::CurrentIndex(Math::Vector2 argPos) const
{
	return argPos / GridSize;
}

void RiskMap::Draw()const
{
	for (int lIndexX = 0; lIndexX < GridMax; lIndexX++)
	{
		for (int lIndexY = 0; lIndexY < GridMax; lIndexY++)
		{
			grids_[lIndexX][lIndexY]->Draw(graph_);
		}
	}
}