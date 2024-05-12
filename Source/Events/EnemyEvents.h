#pragma once
#include "GameEvent.h"

class CObject;

struct FEnemyDeathEvent : FGameEvent {
    CObject* enemy;

    FEnemyDeathEvent(CObject* enemyObject) : enemy(enemyObject) {}
};
