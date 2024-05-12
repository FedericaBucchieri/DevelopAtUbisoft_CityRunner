#include "stdafx.h"
#include "UpperPlatform.h"
#include "CollisionComponent.h"
#include "Collectible.h"
#include "Random.h"
#include "TilesComponent.h"
#include "TransformComponent.h"

CObject* CUpperPlatform::CreateUpperPlatform(FVector2D location, int inPlatformHeight, int inPlatformWidth)
{
	CObject* platform = new CObject();

	std::vector<char*> variation1;
	variation1.push_back(CGameCostants::upperTileLeftV1);
	variation1.push_back(CGameCostants::upperTileRightV1);
	variation1.push_back(CGameCostants::upperTileCenterV1);

	std::vector<char*> variation2;
	variation2.push_back(CGameCostants::upperTileLeftV2);
	variation2.push_back(CGameCostants::upperTileRightV2);
	variation2.push_back(CGameCostants::upperTileCenterV2);

	std::vector<std::vector<char*>> variations;
	variations.push_back(variation1);
	variations.push_back(variation2);

	CTransformComponent* p_transformComponent = new CTransformComponent(platform);
	p_transformComponent->SetLocation(location);
	platform->AddComponent(p_transformComponent);

	CTilesComponent* p_tilesComponent = new CTilesComponent(platform, inPlatformHeight, inPlatformWidth, variations, true);
	platform->AddComponent(p_tilesComponent);

	CCollisionComponent* p_collisionComponent = new CCollisionComponent(platform, ECollisionProfile::FLOOR);
	p_collisionComponent->SetBoundingBox(location, inPlatformWidth * CGameCostants::tileSize, inPlatformHeight * CGameCostants::tileSize/2);
	platform->AddComponent(p_collisionComponent);

	return platform;
}

