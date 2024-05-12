#pragma once
#include "Object.h"
#include "stdafx.h"
#include "GameAura.h"
#include "InputEvents.h"

//
// The CCharacter namespace contains functions related to creating and managing the player character.
//

class CMeshComponent;
class CGameAuraComponent;
class CAttackComponent;
class CAnimationComponent;
class CMovementComponent;
class CInputComponent;
class CGameEventManager;

namespace CCharacter
{
	CObject* CreateCharacter();
	CMeshComponent* SetupMeshComponent(CObject* character);
	void OnResourceChanged(CGameAuraComponent* gameAuraComponent, CAttackComponent* attackComponent, CAnimationComponent* animationComponent, EAuraResource resource, float newValue, float delta);
	void OnResourceDrained(CMeshComponent* meshComponent, CGameEventManager* eventManager, CObject* character, EAuraResource resource);
	void SetInputComponentState(CInputComponent* inputComponent, CGameEventManager* eventManager, CAttackComponent* attackComponent, CMovementComponent* movementComponent, bool bIsActive);
	void OnAttack(CAttackComponent* attackComponent);
	void OnJump(CAttackComponent* attackComponent, CMovementComponent* movementComponent);

	extern bool b_isInvulnerable;
	extern std::function<void(FArrowUpEvent*)> m_handlerArrowUp;
	extern std::function<void(FSpacePressedEvent*)> m_handlerSpacePressed;
};

