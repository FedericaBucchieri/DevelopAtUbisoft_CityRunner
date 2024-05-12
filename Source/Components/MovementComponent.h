#pragma once
#include "stdafx.h"
#include "Component.h"
#include "MovementStatus.h"
#include "GameAura.h"
#include "SoundData.h"
#include "MovementAnimationEvents.h"
#include "GameResourceEvents.h"

//
// CMovementComponent handles the movement and physic of a game object.
// It uses resources from a GameAuraComponent to determine movement parameters.
//

class CGameEventManager;
class CTransformComponent;
class CGameAuraComponent;

class CMovementComponent : public CComponent
{

public:
	CMovementComponent(CObject* owner, EMovementStatus initialStatus, std::map<EMovementStatus, FSoundData> sounds);
	CMovementComponent(CObject* owner, EMovementStatus initialStatus);
	void OnBeginPlay() override;
	void OnBeginDestroy() override;

	void Update(float deltaTime) override;
	
	void HandleCollisionMovementOnYAxis(float deltaTime, FVector2D collisionNormal, float yDifference);
	void HandleCollisionMovementOnXAxis(float deltaTime);

	void HandleJumpInput();
	void SetIsOnGround(bool state) { b_IsOnGround = state; }

	float GetLinearSpeed() { return m_linearSpeed; }
	void SetLineatSpeed(float speed) { m_linearSpeed = speed; }
	FVector2D GetForwardVector() { return m_forwardVector; }
	void SetVelocity(FVector2D inVelocity) { m_velocity = inVelocity; }
	FVector2D GetVelocity() { return m_velocity; }

	void ChangeStatus(EMovementStatus newStatus);
	void SetForwardVector(FVector2D newVector);

protected:
	void OnResourceChanged(EAuraResource resource, float newValue, float delta);
	void ChangeDirection();

	CTransformComponent* p_transformComponent;
	EMovementStatus m_movementStatus;
	CGameAuraComponent* p_gameAuraComponent;

	float m_walkSpeed;
	float m_runSpeed;
	float m_jumpSpeed;
	float m_jumpHeight;
	float m_jumpProgressPosition;
	float m_doubleJumpHeightMultiplier;
	float m_doubleJumpSpeedMultiplier;
	int m_currentJumps;
	bool b_IsOnGround;
	const float m_gravity;
	const float m_gravityMultiplier;

	float m_linearSpeed;

	FVector2D m_acceleration;
	FVector2D m_velocity;
	FVector2D m_forwardVector;

	std::function<void(FChangeDirectionEvent*)> m_handlerChangeDirection;
	std::function<void(FResourceChangedEvent*)> m_handlerResourceChanged;

	CGameEventManager* p_eventManager;
	std::map<EMovementStatus, FSoundData> v_sounds;
};

