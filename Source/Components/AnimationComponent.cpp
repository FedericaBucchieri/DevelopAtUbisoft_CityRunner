#include "stdafx.h"
#include "AnimationComponent.h"
#include "MovementComponent.h"
#include "MeshComponent.h"
#include "Object.h"
#include "GameMode.h"
#include "GameEventManager.h"
#include "MovementStatus.h"
#include "GameInstance.h"
#include "GameMode.h"
#include "GameplayGameMode.h"
#include "AnimationData.h"
#include "TransformComponent.h"
#include "MovementAnimationEvents.h"

CAnimationComponent::CAnimationComponent(CObject* owner) :
	CComponent(owner),
	p_meshComponent(nullptr),
	m_handlerMovementStatusChanged(nullptr),
	b_singleAnimationPlaying(0),
	m_currentAnimation(EAnimationID::IDLE),
	m_previousAnimation(EAnimationID::IDLE),
	p_eventManager(nullptr),
	m_currentAnimationDuration(0.f),
	m_currentTime(0.f)
{
}

void CAnimationComponent::OnBeginPlay()
{
	CComponent::OnBeginPlay();

	CMovementComponent* p_movementComponent = p_owner->GetFirstComponentOfClass<CMovementComponent>();
	if(p_movementComponent == nullptr) return;
	p_meshComponent = p_owner->GetFirstComponentOfClass<CMeshComponent>();
	if (p_meshComponent == nullptr) return;
	p_transformComponent = p_owner->GetFirstComponentOfClass<CTransformComponent>();
	if (p_transformComponent == nullptr) return;

	p_eventManager = CGameInstance::GetGameEventManager();
	if (p_eventManager)
	{
		m_handlerMovementStatusChanged = [this](FMovementStatusChangedEvent* event) {
			UpdateAnimation(event->newStatus);
			};
		p_eventManager->Subscribe<FMovementStatusChangedEvent>(p_movementComponent->GetUniqueID(), m_handlerMovementStatusChanged);
	}
}

void CAnimationComponent::OnBeginDestroy()
{
	CMovementComponent* p_movementComponent = p_owner->GetFirstComponentOfClass<CMovementComponent>();
	if (p_eventManager != nullptr && p_movementComponent != nullptr)
	{
		p_eventManager->RemoveSubscription<FMovementStatusChangedEvent>(p_movementComponent->GetUniqueID(), m_handlerMovementStatusChanged);
		p_eventManager->RemoveAllSubscriptions(m_uniqueID);
	}
	CComponent::OnBeginDestroy();
}

bool CAnimationComponent::CanUpdateStatus()
{
	if (!b_isActive) return false;
	if (p_meshComponent == nullptr) return false;
	if (m_currentAnimation == EAnimationID::HIT) return false;
	if (b_singleAnimationPlaying) return false;

	return true;
}

void CAnimationComponent::UpdatePreviousAnimation(EMovementStatus movementStatus)
{
	bool b_inverse = p_transformComponent->GetForwardVector().x == -1;
	switch (movementStatus)
	{
	case IDLESTATUS:
		m_previousAnimation = b_inverse ? EAnimationID::IDLELEFT : EAnimationID::IDLE;
		break;
	case RUNNING:
		m_previousAnimation = EAnimationID::RUN;
		break;
	case JUMPING:
		m_previousAnimation = EAnimationID::JUMP;
		break;
	case DOUBLEJUMPING:
		m_previousAnimation = EAnimationID::DOUBLEJUMP;
		break;
	case FALLING:
		m_previousAnimation = EAnimationID::DOUBLEJUMP;
		break;
	default:
		break;
	}
}

void CAnimationComponent::UpdateAnimation(EMovementStatus movementStatus)
{
	if (!CanUpdateStatus())
	{
		UpdatePreviousAnimation(movementStatus);
		return;
	}

	m_previousAnimation = p_meshComponent->GetCurrentAnimation();
	bool b_inverse = p_transformComponent->GetForwardVector().x == -1;

	switch (movementStatus)
	{
	case IDLESTATUS:
		m_currentAnimation = b_inverse ? EAnimationID::IDLELEFT :EAnimationID::IDLE;
		break;
	case RUNNING:
		m_currentAnimation = EAnimationID::RUN;
		break;
	case JUMPING:
		m_currentAnimation = EAnimationID::JUMP;
		break;
	case DOUBLEJUMPING:
		m_currentAnimation = EAnimationID::DOUBLEJUMP;
		break;
	case FALLING:
		m_currentAnimation = (m_previousAnimation == EAnimationID::DOUBLEJUMP) ? EAnimationID::DOUBLEJUMP : EAnimationID::JUMP;
		break;
	default:
		break;
	}

	PlayAnimation(m_currentAnimation);
}

void CAnimationComponent::PlayAnimationOnce(EAnimationID ID)
{
	if (b_singleAnimationPlaying) return;
	if (m_previousAnimation == ID) return;

	b_singleAnimationPlaying = true;

	m_previousAnimation = m_currentAnimation;
	m_currentAnimation = ID;
	m_currentAnimationDuration = p_meshComponent->GetAnimationDuration(ID);
	PlayAnimation(m_currentAnimation);
}

void CAnimationComponent::Update(float deltaTime)
{
	CComponent::Update(deltaTime);

	if (!b_singleAnimationPlaying) return;

	m_currentTime += deltaTime * 0.001f;
	if (m_currentTime >= m_currentAnimationDuration)
	{
		p_eventManager->Publish<FAnimationEndEvent>(m_uniqueID, new FAnimationEndEvent{ m_currentAnimation });
		if (m_endAnimationCallback)
		{
			m_endAnimationCallback(m_currentAnimation);
		}

		PlayAnimation(m_previousAnimation);
		m_currentAnimation = m_previousAnimation;
		m_currentTime = 0.f;
		b_singleAnimationPlaying = false;
	}
}

void CAnimationComponent::Render()
{
	if (b_debug)
	{
		App::Print(p_transformComponent->GetLocation().x, p_transformComponent->GetLocation().y + 200, b_singleAnimationPlaying ? "animation" : "");
	}
}

void CAnimationComponent::ResetStatus()
{
	m_currentTime = 0.f;
	m_currentAnimationDuration = 0.f;
	b_singleAnimationPlaying = false;
}

void CAnimationComponent::ToggleInvincibility()
{
	if (m_currentAnimation != EAnimationID::HIT)
	{
		m_previousAnimation = m_currentAnimation;
		m_currentAnimation = EAnimationID::HIT;
	}
	else
	{
		m_currentAnimation = m_previousAnimation;
	}

	PlayAnimation(m_currentAnimation);
	ResetStatus();
}

void CAnimationComponent::PlayAnimation(EAnimationID ID)
{
	p_meshComponent->PlayAnimation(ID);
	if (p_eventManager)
	{
		p_eventManager->Publish<FAnimationStartEvent>(m_uniqueID, new FAnimationStartEvent{ ID });
	}
	if (m_startAnimationCallback)
	{
		m_startAnimationCallback(ID);
	}
}