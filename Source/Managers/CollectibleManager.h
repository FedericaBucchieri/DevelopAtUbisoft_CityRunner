#pragma once
#include "stdafx.h"
#include "Object.h"
#include <deque>
#include "SoundData.h"
#include "CollisionComponent.h"
#include "CollisionEvents.h"

//
// CCollectibleManager orchestrates the management of various collectibles in the game, including coins, obstacles, speed boosts, jump boosts, and health collectibles. 
// It handles the initialization, update, rendering, and pooling of these collectibles based on their visibility in the game world. 
//


class CGameEventManager;
class CGameAura;
class CCameraComponent;

class CCollectibleManager : public CObject
{
public:
	void Init() override;
	void DeInit() override;
	void Update(float deltaTime) override;
	void Render() override;

	void PoolRandomObject(FVector2D location);

protected:
	CObject* RequestCollectible(std::vector<CObject*>& availableCollectibles, std::deque<CObject*>& inUseCollectibles, CGameAura* collectibleEffect, std::vector<char*> meshNames, int meshColumns, FSoundData soundData);
	void ReturnCollectible(CObject* toReturn, std::vector<CObject*>& availableCollectibles, std::deque<CObject*>& inUseCollectibles);
	void UpdateCollectibleLists(std::vector<CObject*>& availableCollectibles, std::deque<CObject*>& inUseCollectibles, float deltaTime);
	void InitializeCollectible(FVector2D location, std::vector<CObject*>& availableCollectibles, std::deque<CObject*>& inUseCollectibles, CGameAura* auraEffect, std::vector<char*> meshNames, int meshColums, FSoundData soundData);

	CCameraComponent* p_cameraComponent;
	
	std::vector<CObject*> v_p_availableCoins;
	std::deque<CObject*> v_p_inUseCoins;
	std::vector<CObject*> v_p_availableSpeedBoost;
	std::deque<CObject*> v_p_inUseSpeedBoost;
	std::vector<CObject*> v_p_availableJumpBoost;
	std::deque<CObject*> v_p_inUseJumpBoost;
	std::vector<CObject*> v_p_availableHealthCollectibles;
	std::deque<CObject*> v_p_inUseHealthCollectibles;
	std::vector<CObject*> v_p_availableObstacles;
	std::deque<CObject*> v_p_inUseObstacles;

	CGameEventManager* p_eventManager;
	std::map<CObject*, std::function<void(FOverlappableOverlapedEvent*)>> m_handlers;
};

