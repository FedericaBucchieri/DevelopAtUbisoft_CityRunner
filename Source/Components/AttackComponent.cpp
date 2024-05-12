#include "stdafx.h"
#include "AttackComponent.h"
#include "GameInstance.h"
#include "GameEventManager.h"
#include "AnimationComponent.h"
#include "GameAuraComponent.h"
#include "Object.h"
#include "CollisionComponent.h"
#include "SoundData.h"
#include "TransformComponent.h"
#include "MovementAnimationEvents.h"
#include "CollisionEvents.h"
#include "SoundEvents.h"

CAttackComponent::CAttackComponent(CObject* owner, std::vector<ECollisionProfile> attackableProfiles, CGameAura* attackEffect, FSoundData soundData) :
	CComponent(owner),
	b_isAttacking(false),
	p_animationComponent(nullptr),
	m_attackEffect(attackEffect),
	p_eventManager(nullptr),
	m_handlerBeginOverlap(nullptr),
	m_handlerAnimationEnd(nullptr),
	v_attackableProfiles(attackableProfiles),
	m_soundData(soundData),
	m_timeBeforeAttack(0.5f),
	m_currentTimeBeforeAttack(0.f)
{
}

void CAttackComponent::OnBeginPlay()
{
	CComponent::OnBeginPlay();

	p_animationComponent = p_owner->GetFirstComponentOfClass<CAnimationComponent>();
	if (!p_animationComponent) return;

	p_transformComponent = p_owner->GetFirstComponentOfClass<CTransformComponent>();
	if (p_transformComponent == nullptr) return;

	p_eventManager = CGameInstance::GetGameEventManager();
	if (p_eventManager)
	{
		p_collisionComponent = p_owner->GetFirstComponentOfClass<CCollisionComponent>();
		if (!p_collisionComponent) return;

		m_handlerBeginOverlap = [this](FComponentBeginOverlapEvent* event) {
			CheckAttackStatus(event->otherComponent, event->otherObject);
			};
		p_eventManager->Subscribe<FComponentBeginOverlapEvent>(p_collisionComponent->GetUniqueID(), m_handlerBeginOverlap);

		m_handlerAnimationEnd = [this](FAnimationEndEvent* event){
			if (event->ID == EAnimationID::ATTACK || event->ID == EAnimationID::ATTACKLEFT)
			{
				EndAttack();
			}
		};
		p_eventManager->Subscribe<FAnimationEndEvent>(p_animationComponent->GetUniqueID(), m_handlerAnimationEnd);
	}
}

void CAttackComponent::Render()
{
	if (b_debug)
	{
		App::Print(p_transformComponent->GetLocation().x, p_transformComponent->GetLocation().y + 300, b_isAttacking ? "attack" : "");
	}
}

void CAttackComponent::Update(float deltaTime)
{
	CComponent::Update(deltaTime);

	if (!b_isAttacking) return;

	m_currentTimeBeforeAttack += deltaTime * 0.001f;
	if (m_currentTimeBeforeAttack > m_timeBeforeAttack)
	{
		CheckAlreadyCollidingActors();
		m_currentTimeBeforeAttack = 0.f;
	}
}

void CAttackComponent::CheckAlreadyCollidingActors()
{
	for (ECollisionProfile profile : v_attackableProfiles)
	{
		std::vector<CObject*> victims = p_collisionComponent->GetAllCollidingObjectsOfProfile(profile);

		for (int i = 0; i < static_cast<int>(victims.size()); i++)
		{
			ApplyAttack(victims[i]);
		}
	}
	b_isAttacking = false;
}

void CAttackComponent::OnBeginDestroy()
{
	if (p_eventManager != nullptr && m_handlerBeginOverlap)
	{
		p_eventManager->RemoveSubscription<FComponentBeginOverlapEvent>(p_collisionComponent->GetUniqueID(), m_handlerBeginOverlap);
		p_eventManager->RemoveAllSubscriptions(m_uniqueID);
	}

	p_animationComponent = nullptr;
	p_collisionComponent = nullptr;

	if (m_attackEffect)
	{
		m_attackEffect->OnAuraEnd();
		delete m_attackEffect;
	}
}

void CAttackComponent::StartAttack()
{
	b_isAttacking = true;
	if (p_transformComponent->GetForwardVector().x > 0)
	{
		p_animationComponent->PlayAnimationOnce(EAnimationID::ATTACK);
	}
	else
	{
		p_animationComponent->PlayAnimationOnce(EAnimationID::ATTACKLEFT);
	}

	if (p_eventManager && m_soundData.soundName != nullptr)
	{
		p_eventManager->Publish<FPlaySoundEvent>(p_owner->GetUniqueID(), new FPlaySoundEvent{ m_soundData });
	}
}

void CAttackComponent::EndAttack()
{
	b_isAttacking = false;
}

void CAttackComponent::CheckAttackStatus(CCollisionComponent* otherComponent, CObject* otherObject)
{
	if (!b_isAttacking) return;
	if (!otherObject) return;

	auto it = std::find(v_attackableProfiles.begin(), v_attackableProfiles.end(), otherComponent->GetCollisionProfile());
	if (it == v_attackableProfiles.end()) return;

	ApplyAttack(otherObject);
}

void CAttackComponent::ApplyAttack(CObject* otherObject)
{
	if (!b_isAttacking) return;

	CGameAuraComponent* p_otherAuraComponent = otherObject->GetFirstComponentOfClass<CGameAuraComponent>();
	if (!p_otherAuraComponent) return;
	p_otherAuraComponent->ActivateAura(m_attackEffect);
}