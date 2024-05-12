#include "stdafx.h"
#include "FloorPlatform.h"
#include "CollisionComponent.h"
#include <random>
#include "Collectible.h"
#include "UpperPlatform.h"
#include "GameCostants.h"
#include "SimpleSprite.h"
#include "WorldManager.h"
#include "Random.h"
#include "TilesComponent.h"
#include "TransformComponent.h"


CObject* CFloorPlatform::CreatePlatform(FVector2D location, int inPlatformHeight, int inPlatformWidth)
{
	CObject* platform = new CObject();

	std::vector<char*> variation1;
	variation1.push_back(CGameCostants::floorTileLeftBorder);
	variation1.push_back(CGameCostants::floorTileRightCorner);
	variation1.push_back(CGameCostants::floorTileCenterTop);
	variation1.push_back(CGameCostants::floorTileLeftCorner);
	variation1.push_back(CGameCostants::floorTileRightBorder);
	variation1.push_back(CGameCostants::floorTileCenterBottom);

	std::vector<std::vector<char*>> variations;
	variations.push_back(variation1);

	CTransformComponent* p_transformComponent = new CTransformComponent(platform);
	p_transformComponent->SetLocation(location);
	platform->AddComponent(p_transformComponent);

	CTilesComponent* p_tilesComponent = new CTilesComponent(platform, inPlatformHeight, inPlatformWidth, variations, false);
	platform->AddComponent(p_tilesComponent);

	CCollisionComponent* p_collisionComponent = new CCollisionComponent(platform, ECollisionProfile::FLOOR);
	p_collisionComponent->SetBoundingBox(location, inPlatformWidth * CGameCostants::tileSize, inPlatformHeight * CGameCostants::tileSize);
	platform->AddComponent(p_collisionComponent);

	return platform;
}
