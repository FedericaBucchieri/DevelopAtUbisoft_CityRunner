#include "stdafx.h"
#include "FeetComponent.h"
#include "CollisionComponent.h"
#include "GameEventManager.h"
#include "MovementComponent.h"
#include "GameInstance.h"
#include "CollisionEvents.h"

CFeetComponent::CFeetComponent(CObject* owner) :
	CCollisionComponent(owner, ECollisionProfile::CHARACTER_FEET),
	p_movementComponent(nullptr),
	m_handlerBeginOverlap(nullptr),
	m_handlerEndOverlap(nullptr)
{
}

void CFeetComponent::OnBeginPlay()
{
	CComponent::OnBeginPlay();

	p_eventManager = CGameInstance::GetGameEventManager();
	if (!p_eventManager) return;

	p_movementComponent = p_owner->GetFirstComponentOfClass<CMovementComponent>();
	if (!p_movementComponent) return;

	m_handlerBeginOverlap = [this](FComponentBeginOverlapEvent* event) {
		OnBeginOverlap(event->otherComponent, event->otherObject);
		};
	p_eventManager->Subscribe<FComponentBeginOverlapEvent>(m_uniqueID, m_handlerBeginOverlap);

	m_handlerEndOverlap = [this](FComponentEndOverlapEvent* event) {
		OnEndOverlap(event->otherComponent, event->otherObject);
		};
	p_eventManager->Subscribe<FComponentEndOverlapEvent>(m_uniqueID, m_handlerEndOverlap);
}

void CFeetComponent::OnBeginDestroy()
{
	CComponent::OnBeginDestroy();
	if (p_eventManager)
	{
		p_eventManager->RemoveSubscription<FComponentBeginOverlapEvent>(m_uniqueID, m_handlerBeginOverlap);
		p_eventManager->RemoveSubscription<FComponentEndOverlapEvent>(m_uniqueID, m_handlerEndOverlap);
	}
}

void CFeetComponent::OnBeginOverlap(CCollisionComponent* otherComponent, CObject* otherObject)
{
	if (otherComponent->GetCollisionProfile() == ECollisionProfile::FLOOR)
	{
		p_movementComponent->SetIsOnGround(true);
	}
}

void CFeetComponent::OnEndOverlap(CCollisionComponent* otherComponent, CObject* otherObject)
{
	if (otherComponent->GetCollisionProfile() == ECollisionProfile::FLOOR)
	{
		p_movementComponent->SetIsOnGround(false);
	}
}