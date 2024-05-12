#pragma once

#include "stdafx.h"
#include "Object.h"
#include <deque>
#include "SoundData.h"

//
// CWorldManager handles the initialization, updating, and rendering of the elements in the game world.
// It specifically manages floor and upper platforms, and other managers like the collectible manager and the enemy manager.
//

class CGameCamera;
class CEnemyManager;
class CCollectibleManager;
class CTilesComponent;
class CTransformComponent;

class CWorldManager : public CObject
{
public:
	void Init() override;
	void DeInit() override;
	void Update(float deltaTime) override;
	void Render() override;
	float GetInitialGroundLevel();

private:
	void InitializePlatforms();
	void GetPlatform(CObject* oldPlatform);
	void CreateUpperPlatforms(CObject* floorPlatform);
	void PlaceRandomObjectsOnPlatform(CObject* platform);
	void PlaceRandomObjectsOnUpperPlatform(CObject* platform);
	void UpdateFloorPlatform(float deltaTime);
	void UpdateUpperPlatform(float deltaTime);
	void ReturnPlatform(std::deque<CObject*>& inUsePlatforms);

	std::deque<CObject*> v_p_inUsePlatforms;
	std::deque<CObject*> v_p_inUseUpperPlatforms;

	CCameraComponent* p_cameraComponent;
	FVector2D m_position = FVector2D{ 0, 0 };

	CObject* p_firstFloorPlatform;
	CTransformComponent* p_transformableFloorPlatform;
	CTilesComponent* p_tilesFloorPlatform;

	CObject* p_firstUpperPlatform;
	CTransformComponent* p_transformableUpperPlatform;
	CTilesComponent* p_tilesUpperPlatform;

	CCollectibleManager* p_collectibleManager;
	CEnemyManager* p_enemyManager;
};
