#pragma once
#include "stdafx.h"
#include "Object.h"
#include <deque>
#include "EnemyData.h"
#include "EnemyEvents.h"

//
// CEnemyManager manages the creation, initialization, and updating of enemy objects in the game.
// It is responsible for placing enemies on platforms, updating their positions, and handling their activation and deactivation.
//


class CCameraComponent;
class CGameEventManager;

class CEnemyManager : public CObject
{
public: 
	void Init() override;
	void DeInit() override;
	void Update(float deltaTime) override;
	void Render() override;

	void PlaceEnemiesOnPlatform(FVector2D platformLocation, float platformWidth, float platformHeight);


protected:
	FEnemyData GetEnemyData();
	void InitializeEnemy(float minX, float maxX, CObject* enemy);
	void RequestEnemy(FVector2D location, float minX, float maxX);
	void ReturnEnemy(CObject* toReturn);

	std::vector<CObject*> v_p_availableEnemies;
	std::deque<CObject*> v_p_inUseEnemies;

	CCameraComponent* p_cameraComponent;
	CGameEventManager* p_eventManager;
	std::map < CObject*, std::function<void(FEnemyDeathEvent*)>> m_handlers;
};

