#include "stdafx.h"
#include "OverlappableComponent.h"
#include "CollisionComponent.h"
#include "GameAura.h"
#include "GameMode.h"
#include "GameEventManager.h"
#include "GameInstance.h"
#include "GameplayGameMode.h"
#include "GameAuraComponent.h"
#include "CollisionEvents.h"
#include "MeshComponent.h"

COverlappableComponent::COverlappableComponent(CObject* owner, CGameAura* overlapEffect, ECollisionProfile overlappedBy, Callback onCollectCallback) :
	CComponent(owner),
	m_handlerBeginOverlap(nullptr),
	p_overlapEffect(overlapEffect),
	m_overlappedBy(overlappedBy),
	m_onCollectCallback(onCollectCallback),
	b_isOverlapped(false),
	m_afterDeathTime(0.2f),
	m_currentTime(0.f)
{
}

COverlappableComponent::COverlappableComponent(CObject* owner, CGameAura* overlapEffect, ECollisionProfile overlappedBy) :
	COverlappableComponent(owner, overlapEffect, overlappedBy, {})
{
}

void COverlappableComponent::OnBeginPlay()
{
	CComponent::OnBeginPlay();
	p_eventManager = CGameInstance::GetGameEventManager();

	p_collisionComponent = p_owner->GetFirstComponentOfClass<CCollisionComponent>();
	if (p_collisionComponent == nullptr) return;

	if (p_eventManager)
	{
		m_handlerBeginOverlap = [this](FComponentBeginOverlapEvent* event) {
			OnComponentOverlap(event->otherComponent, event->otherObject);
		};
		p_eventManager->Subscribe<FComponentBeginOverlapEvent>(p_collisionComponent->GetUniqueID(), m_handlerBeginOverlap);
	}
}

void COverlappableComponent::OnBeginDestroy()
{
	if (p_eventManager)
	{
		p_eventManager->RemoveSubscription<FComponentBeginOverlapEvent>(p_collisionComponent->GetUniqueID(), m_handlerBeginOverlap);
		p_eventManager->RemoveAllSubscriptions(m_uniqueID);
	}
	CComponent::OnBeginDestroy();
}

void COverlappableComponent::Update(float deltaTime)
{
	CComponent::Update(deltaTime);
	if (!b_isOverlapped) return;

	m_currentTime += deltaTime * 0.001f;
	if (m_currentTime > m_afterDeathTime)
	{
		m_currentTime = 0.f;
		b_isOverlapped = false;
		if (p_eventManager)
		{
			p_eventManager->Publish<FOverlappableOverlapedEvent>(m_uniqueID, new FOverlappableOverlapedEvent{ p_owner });
		}
	}

}

void COverlappableComponent::OnComponentOverlap(CCollisionComponent* otherComponent, CObject* otherObject)
{
	if (!b_isActive) return;

	if (otherComponent->GetCollisionProfile() == m_overlappedBy)
	{
		b_isActive = false;
		OnCollect(otherObject);
	}
}

void COverlappableComponent::OnCollect(CObject* collector)
{
	if (p_overlapEffect == nullptr) return;

	Deactivate();
	b_isOverlapped = true;
	m_onCollectCallback();

	CGameAuraComponent* p_auraComponentTarget = collector->GetFirstComponentOfClass<CGameAuraComponent>();
	if (!p_auraComponentTarget) return;
	p_auraComponentTarget->ActivateAura(p_overlapEffect);
}
