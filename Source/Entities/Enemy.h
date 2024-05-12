#pragma once
#include "stdafx.h"
#include "Object.h"
#include "EnemyData.h"
#include "GameAura.h"

//
// The CEnemy namespace contains a function for creating enemy objects in the game.
//

class CAnimationComponent;
class CGameEventManager;

namespace CEnemy
{
	CObject* CreateEnemy(FVector2D location, FEnemyData enemyData);
	void OnResourceDrained(CAnimationComponent* animationComponent, EAuraResource resource);
	void OnEnemyDeath(CGameEventManager* eventManager, CObject* enemy);
}
