#pragma once
#include "stdafx.h"
#include "Component.h"
#include "CollisionEvents.h"

//
// COverlappableComponent is responsible for handling overlap events with other objects.
// It checks for collisions with a specified collision profile and triggers an effect (GameAura) when overlapped.
//

class CGameAura;
class CGameEventManager;

class COverlappableComponent : public CComponent
{

public:
	using Callback = std::function<void()>;

	COverlappableComponent(CObject* owner, CGameAura* overlapEffect, ECollisionProfile overlappedBy, Callback onCollectCallback);
	COverlappableComponent(CObject* owner, CGameAura* overlapEffect, ECollisionProfile overlappedBy);
	void OnBeginPlay() override;
	void OnBeginDestroy() override;
	void Update(float deltaTime) override;
	void OnComponentOverlap(CCollisionComponent* otherComponent, CObject* otherObject);
	void OnCollect(CObject* collector);

protected:
	CGameAura* p_overlapEffect;
	CCollisionComponent* p_collisionComponent;
	ECollisionProfile m_overlappedBy;

	std::function<void(FComponentBeginOverlapEvent*)> m_handlerBeginOverlap;
	CGameEventManager* p_eventManager;
	Callback m_onCollectCallback;

	bool b_isOverlapped;
	float m_afterDeathTime;
	float m_currentTime;
};

