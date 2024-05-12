#pragma once
#include "Component.h"
#include "SoundData.h"
#include "MovementAnimationEvents.h"
#include "CollisionEvents.h"

//
// CAttackComponent manages attacks for a game object.
//

class CAnimationComponent;
class CCollisionComponent;
class CGameAura;
class CGameEventManager;
class CTransformComponent;

//
// CAttackComponent orchestrates attacks for a game object.
//

class CAttackComponent : public CComponent
{
public:
	CAttackComponent(CObject* owner, std::vector<ECollisionProfile> attackableProfiles,  CGameAura* attackEffect, FSoundData soundData);

	void OnBeginPlay() override;
	void OnBeginDestroy() override;
	void StartAttack();
	void EndAttack();
	void Render() override;
	void Update(float deltaTime) override;
	bool GetIsAttacking() { return b_isAttacking; }

protected:
	void CheckAlreadyCollidingActors();
	void CheckAttackStatus(CCollisionComponent* otherComponent, CObject* otherObject);
	void ApplyAttack(CObject* otherObject);

	CAnimationComponent* p_animationComponent;
	CCollisionComponent* p_collisionComponent;
	CTransformComponent* p_transformComponent;
	CGameAura* m_attackEffect;
	CGameEventManager* p_eventManager;
	std::vector<ECollisionProfile> v_attackableProfiles;
	FSoundData m_soundData;
	bool b_isAttacking;
	std::function<void(FComponentBeginOverlapEvent*)> m_handlerBeginOverlap;
	std::function<void(FAnimationEndEvent*)> m_handlerAnimationEnd;
	bool b_debug = false;
	float m_currentTimeBeforeAttack;
	float m_timeBeforeAttack;
};

