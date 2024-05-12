#include "stdafx.h"
#include "MovementComponent.h"
#include "GameAuraComponent.h"
#include "Object.h"
#include "MovementStatus.h"
#include "GameEventManager.h"
#include "GameMode.h"
#include "GameInstance.h"
#include "GameplayGameMode.h"
#include "SoundData.h"
#include "TransformComponent.h"
#include "MovementAnimationEvents.h"
#include "GameResourceEvents.h"
#include "SoundEvents.h"

CMovementComponent::CMovementComponent(CObject* owner, EMovementStatus initialStatus, std::map<EMovementStatus, FSoundData> sounds) :
	CComponent(owner),
	p_eventManager(nullptr),
	m_walkSpeed(50.f),
	m_runSpeed(150.f),
	m_jumpHeight(10.f),
	m_jumpSpeed(250.f),
	m_jumpProgressPosition(0.f),
	m_doubleJumpHeightMultiplier(1.f),
	m_doubleJumpSpeedMultiplier(1.3f),
	m_currentJumps(0),
	b_IsOnGround(false),
	m_handlerResourceChanged(nullptr),
	m_acceleration(FVector2D{ 0.f, 0.f }),
	m_linearSpeed(0.f),
	m_velocity(FVector2D{ 0.f, 0.f }),
	m_movementStatus(initialStatus),
	m_handlerChangeDirection(nullptr),
	m_gravity(9.81f),
	m_gravityMultiplier(50.f),
	v_sounds(sounds)
{
}

CMovementComponent::CMovementComponent(CObject* owner, EMovementStatus initialStatus) :
	CMovementComponent(owner, initialStatus, {})
{
}


void CMovementComponent::OnBeginPlay()
{
	CComponent::OnBeginPlay();

	p_transformComponent = p_owner->GetFirstComponentOfClass<CTransformComponent>();
	if (p_transformComponent == nullptr) return;
	m_forwardVector = p_transformComponent->GetForwardVector();

	p_gameAuraComponent = p_owner->GetFirstComponentOfClass<CGameAuraComponent>();
	if (p_gameAuraComponent == nullptr) return;

	m_runSpeed = p_gameAuraComponent->GetResourceValue(EAuraResource::RunSpeed);
	m_walkSpeed = p_gameAuraComponent->GetResourceValue(EAuraResource::WalkSpeed);
	m_jumpHeight = p_gameAuraComponent->GetResourceValue(EAuraResource::JumpHeight);

	p_eventManager = CGameInstance::GetGameEventManager();
	if (p_eventManager)
	{
		m_handlerResourceChanged = [this](FResourceChangedEvent* event) {
			OnResourceChanged(event->resource, event->newValue, event->delta);
			};
		p_eventManager->Subscribe<FResourceChangedEvent>(p_gameAuraComponent->GetUniqueID(), m_handlerResourceChanged);

		m_handlerChangeDirection = [this](FChangeDirectionEvent* event) {
			ChangeDirection();
			};
		p_eventManager->Subscribe<FChangeDirectionEvent>(p_owner->GetUniqueID(), m_handlerChangeDirection);
	}
}

void CMovementComponent::OnBeginDestroy()
{
	if (p_eventManager)
	{
		p_eventManager->RemoveSubscription<FResourceChangedEvent>(p_gameAuraComponent->GetUniqueID(), m_handlerResourceChanged);
		p_eventManager->RemoveAllSubscriptions(m_uniqueID);
	}
	CComponent::OnBeginDestroy();
}

void CMovementComponent::OnResourceChanged(EAuraResource resource, float newValue, float delta)
{
	switch (resource)
	{
	case EAuraResource::RunSpeed:
		m_runSpeed = newValue;
		break;
	case EAuraResource::WalkSpeed:
		m_walkSpeed = newValue;
		break;
	case EAuraResource::JumpHeight:
		m_jumpHeight = newValue;
		break;
	}
}

void CMovementComponent::Update(float deltaTime)
{
	CComponent::Update(deltaTime);

	float jumpDelta = 0.f;
	m_acceleration.y = -m_gravity * m_gravityMultiplier;

	switch (m_movementStatus)
	{
	case EMovementStatus::IDLESTATUS:
		m_linearSpeed = m_walkSpeed;
		break;

	case EMovementStatus::RUNNING:
		m_currentJumps = 0;
		m_linearSpeed = m_runSpeed;

		if (!b_IsOnGround)
		{
			ChangeStatus(EMovementStatus::FALLING);
			p_eventManager->Publish<FPlaySoundEvent>(p_owner->GetUniqueID(), new FPlaySoundEvent{ FSoundData{"", false, false} });
		}
		break;

	case EMovementStatus::JUMPING:
		jumpDelta = m_velocity.y * deltaTime * 0.001f;
		m_jumpProgressPosition += jumpDelta;

		if (m_jumpProgressPosition >= m_jumpHeight)
		{
			ChangeStatus(EMovementStatus::FALLING);
		}

		if (b_IsOnGround)
		{
			ChangeStatus(EMovementStatus::RUNNING);
			m_jumpProgressPosition = 0;
		}

		break;
	case EMovementStatus::FALLING:
		if (b_IsOnGround)
		{
			ChangeStatus(EMovementStatus::RUNNING);
			m_jumpProgressPosition = 0;
		}
		break;

	case EMovementStatus::DOUBLEJUMPING:
		jumpDelta = m_velocity.y * deltaTime * 0.001f;
		m_jumpProgressPosition += jumpDelta;

		if (m_jumpProgressPosition >= m_jumpHeight)
		{
			ChangeStatus(EMovementStatus::FALLING);
			m_jumpHeight /= m_doubleJumpHeightMultiplier;
			m_jumpSpeed /= m_doubleJumpSpeedMultiplier;
		}

		if (b_IsOnGround)
		{
			ChangeStatus(EMovementStatus::RUNNING);
			m_jumpProgressPosition = 0;
		}

		break;
	default:
		break;
	}

	m_velocity.x = m_linearSpeed;
	m_velocity = m_velocity + m_acceleration * deltaTime * 0.001f;

	FVector2D delta = m_velocity * deltaTime * 0.001f * m_forwardVector;
	FVector2D newLocation = FVector2D{ p_transformComponent->GetLocation().x + delta.x, p_transformComponent->GetLocation().y + delta.y + jumpDelta };
	p_transformComponent->SetLocation(newLocation);
}

void CMovementComponent::HandleCollisionMovementOnYAxis(float deltaTime, FVector2D collisionNormal, float yDifference)
{
	FVector2D delta = m_velocity * deltaTime * 0.001f;
	float sign = collisionNormal.y > 0.f ? 1.f : -1.f;
	FVector2D newLocation = FVector2D{ p_transformComponent->GetLocation().x, p_transformComponent->GetLocation().y - delta.y + (yDifference * sign) };
	p_transformComponent->SetLocation(newLocation);

	m_velocity = m_velocity * FVector2D { 1, 0 };
}

void CMovementComponent::HandleCollisionMovementOnXAxis(float deltaTime)
{
	FVector2D newLocation = FVector2D{ p_transformComponent->GetPreviousTransform().location.x, p_transformComponent->GetLocation().y };
	p_transformComponent->SetLocation(newLocation);

	m_velocity = m_velocity * FVector2D{ 0, 1 };
}

void CMovementComponent::HandleJumpInput()
{
	if (m_movementStatus == EMovementStatus::RUNNING)
	{
		ChangeStatus(EMovementStatus::JUMPING);
		b_IsOnGround = false;
		m_jumpProgressPosition = 0.f;
		m_velocity.y = m_jumpSpeed;
		p_transformComponent->SetLocation(FVector2D{ p_transformComponent->GetLocation().x, p_transformComponent->GetLocation().y + 2.f });
		m_currentJumps++;
	}
	else if ((m_movementStatus == EMovementStatus::JUMPING || m_movementStatus == EMovementStatus::FALLING) && m_currentJumps == 1)
	{
		m_jumpHeight *= m_doubleJumpHeightMultiplier;
		m_jumpSpeed *= m_doubleJumpSpeedMultiplier;
		m_velocity.y = m_jumpSpeed;
		m_currentJumps++;
		ChangeStatus(EMovementStatus::DOUBLEJUMPING);
	}
}

void CMovementComponent::ChangeStatus(EMovementStatus newStatus)
{
	m_movementStatus = newStatus;
	if (p_eventManager)
	{
		p_eventManager->Publish<FMovementStatusChangedEvent>(m_uniqueID, new FMovementStatusChangedEvent{ newStatus });
		p_eventManager->Publish<FPlaySoundEvent>(p_owner->GetUniqueID(), new FPlaySoundEvent{ v_sounds[newStatus] });
	}
}

void CMovementComponent::SetForwardVector(FVector2D newVector)
{
	m_forwardVector = newVector;
	p_transformComponent->SetForwardVector(m_forwardVector);
}

void CMovementComponent::ChangeDirection()
{
	SetForwardVector(FVector2D{ m_forwardVector.x * -1, m_forwardVector.y });
	ChangeStatus(m_movementStatus);
}