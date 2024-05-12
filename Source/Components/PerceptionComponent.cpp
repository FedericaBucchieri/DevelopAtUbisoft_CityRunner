#include "stdafx.h"
#include "PerceptionComponent.h"
#include "GameInstance.h"
#include "CollisionComponent.h"
#include "GameAuraComponent.h"
#include "AttackComponent.h"
#include "EnemyData.h"
#include "TransformComponent.h"
#include "GameEventManager.h"
#include "CollisionEvents.h"

const float CPerceptionComponent::s_thresholdX = 50;

CPerceptionComponent::CPerceptionComponent(CObject* owner, FEnemyData data, CCollisionComponent* perceptionCollision) :
	CComponent(owner),
	m_collisionComponent(perceptionCollision),
	m_minX(0.f),
	m_maxX(0.f),
	m_radius(data.perceptionRadius),
	m_attackRadius(data.attackRadius),
	m_timeSinceLastAttack(0.f),
	m_attackInterval(data.attackInterval),
	m_timeSinceDelayStart(0.f),
	m_attackDelay(data.attackDelay),
	m_handlerBeginOverlap(nullptr),
	m_handlerEndOverlap(nullptr)
{
	m_speedAura = new CGameAura(EAuraResourceDamageMode::MaxAmountPCT, EAuraResource::WalkSpeed, 0.f, 0.5f, EAuraDurationType::Duration, 999999.f, true, false);
}

void CPerceptionComponent::Initialize()
{
	p_transformComponent = p_owner->GetFirstComponentOfClass<CTransformComponent>();
	if (p_transformComponent == nullptr) return;

	m_collisionComponent->SetBoundingBox(p_transformComponent->GetLocation(), m_radius, m_radius/2);
	m_collisionComponent->SetColliderOffset(FVector2D{ -m_radius/2, 0 });

	CComponent::Initialize();
}

void CPerceptionComponent::OnBeginPlay()
{
	CComponent::OnBeginPlay();
	if (m_collisionComponent == nullptr) return;

	p_gameAuraComponent = p_owner->GetFirstComponentOfClass<CGameAuraComponent>();
	if (!p_gameAuraComponent) return;

	p_attackComponent = p_owner->GetFirstComponentOfClass<CAttackComponent>();
	if (!p_attackComponent) return;

	p_eventManager = CGameInstance::GetGameEventManager();
	if (p_eventManager)
	{
		m_handlerBeginOverlap = [this](FComponentBeginOverlapEvent* event)
			{
				OnComponentBeginOverlap(event->otherComponent, event->otherObject);
			};
		p_eventManager->Subscribe<FComponentBeginOverlapEvent>(m_collisionComponent->GetUniqueID(), m_handlerBeginOverlap);

		m_handlerEndOverlap = [this](FComponentEndOverlapEvent* event)
			{
				OnComponentEndOverlap(event->otherComponent, event->otherObject);
			};
		p_eventManager->Subscribe<FComponentEndOverlapEvent>(m_collisionComponent->GetUniqueID(), m_handlerEndOverlap);
	}
}

void CPerceptionComponent::OnBeginDestroy()
{
	if (m_collisionComponent == nullptr) return;
	if (p_eventManager)
	{
		p_eventManager->RemoveSubscription<FComponentBeginOverlapEvent>(m_collisionComponent->GetUniqueID(), m_handlerBeginOverlap);
		p_eventManager->RemoveSubscription<FComponentEndOverlapEvent>(m_collisionComponent->GetUniqueID(), m_handlerEndOverlap);
		p_eventManager->RemoveAllSubscriptions(m_uniqueID);
	}

	if (m_speedAura)
	{
		p_gameAuraComponent->EndAura(m_speedAura);
	}
}

void CPerceptionComponent::OnComponentBeginOverlap(CCollisionComponent* otherComponent, CObject* otherObject)
{
	if (!b_isActive) return;
	if (otherComponent->GetCollisionProfile() == ECollisionProfile::CHARACTER)
	{
		p_currentTarget = otherObject;
		p_gameAuraComponent->ActivateAura(m_speedAura);
	}
}

void CPerceptionComponent::OnComponentEndOverlap(CCollisionComponent* otherComponent, CObject* otherObject)
{
	if (p_currentTarget == otherObject)
	{
		ResetEffect();
	}
}

void CPerceptionComponent::ResetEffect()
{
	p_gameAuraComponent->EndAura(m_speedAura);
	p_currentTarget = nullptr;
}

void CPerceptionComponent::Update(float deltaTime)
{
	CComponent::Update(deltaTime);

	bool triggerAlert = false;
	float ownerX = p_transformComponent->GetLocation().x;
	float ownerForwardX = p_transformComponent->GetForwardVector().x;

	if (ownerForwardX == -1 && ownerX - s_thresholdX <= m_minX && p_currentTarget == nullptr)
	{
		triggerAlert = true;
	}
	else if (ownerForwardX == 1 && ownerX + s_thresholdX >= m_maxX)
	{
		triggerAlert = true;
	}

	if (p_currentTarget != nullptr && !triggerAlert)
	{
		CTransformComponent* p_transformComponent = p_currentTarget->GetFirstComponentOfClass<CTransformComponent>();
		if (p_transformComponent == nullptr) return;

		float targetX = p_transformComponent->GetLocation().x;
		float distance = abs(targetX - ownerX);
		CheckAttackConditions(deltaTime, distance);

		if ((ownerForwardX == -1 && targetX > ownerX && targetX + s_thresholdX < m_maxX) || (ownerForwardX == 1 && targetX < ownerX && targetX + s_thresholdX < m_maxX))
		{
			triggerAlert = true;
		}

	}

	if (triggerAlert)
	{
		p_eventManager->Publish<FChangeDirectionEvent>(p_owner->GetUniqueID(), new FChangeDirectionEvent{});
	}
}

void CPerceptionComponent::Deactivate()
{
	CComponent::Deactivate();

	ResetEffect();
	m_timeSinceLastAttack = 0.f;
	m_timeSinceDelayStart = 0.0f;
	p_attackComponent->EndAttack();
}

void CPerceptionComponent::CheckAttackConditions(float deltaTime, float distance)
{
	m_timeSinceLastAttack += deltaTime * 0.001f;

	if (distance < m_attackRadius && m_timeSinceLastAttack > m_attackInterval)
	{
		m_timeSinceDelayStart += deltaTime * 0.001f;

		if (m_timeSinceDelayStart > m_attackDelay)
		{
			p_attackComponent->StartAttack();
			m_timeSinceLastAttack = 0.f;
			m_timeSinceDelayStart = 0.0f;
		}
	}
	else
	{
		m_timeSinceDelayStart = 0.0f;
	}
}