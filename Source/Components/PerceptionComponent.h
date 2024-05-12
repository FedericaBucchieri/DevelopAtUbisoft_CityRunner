#pragma once
#include "stdafx.h"
#include "Component.h"
#include "EnemyData.h"
#include "CollisionData.h"
#include "CollisionEvents.h"

//
// CPerceptionComponent is responsible for handling enemy perception and triggering actions based on detected targets.
// It checks for overlaps with specified collision profiles, activates an effect (GameAura), and triggers attacks based on certain conditions.
//

class CActor;
class CGameEventManager;
class CCollisionComponent;
class CGameAuraComponent;
class CGameAura;
class CAttackComponent;
class CTransformComponent;

class CPerceptionComponent : public CComponent
{
public: 
	CPerceptionComponent(CObject* owner, FEnemyData data, CCollisionComponent* perceptionCollision);
	void Initialize() override;
	void OnBeginPlay() override;
	void OnBeginDestroy() override;
	void Update(float deltaTime) override;
	void Deactivate() override;
	void SetMinX(float newValue) { m_minX = newValue; }
	void SetMaxX(float newValue) { m_maxX = newValue; }
	void ResetEffect();

protected:
	void OnComponentBeginOverlap(CCollisionComponent* otherComponent, CObject* otherObject);
	void OnComponentEndOverlap(CCollisionComponent* otherComponent, CObject* otherObject);
	void CheckAttackConditions(float deltaTime, float distance);

	float m_minX;
	float m_maxX;
	float m_radius;
	float m_attackRadius;
	float m_attackInterval;
	float m_attackDelay;
	static const float s_thresholdX;
	CCollisionComponent* m_collisionComponent;
	CGameAuraComponent* p_gameAuraComponent;
	CAttackComponent* p_attackComponent;
	CTransformComponent* p_transformComponent;
	CGameAura* m_speedAura;
	CObject* p_currentTarget;
	CGameEventManager* p_eventManager;

	std::function<void(FComponentBeginOverlapEvent*)> m_handlerBeginOverlap;
	std::function<void(FComponentEndOverlapEvent*)> m_handlerEndOverlap;

	float m_timeSinceDelayStart;
	float m_timeSinceLastAttack;
};

