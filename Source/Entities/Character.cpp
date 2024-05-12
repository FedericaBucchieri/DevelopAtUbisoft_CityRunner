#include "stdafx.h"
#include "Object.h"
#include "GameAura.h"
#include "CollisionComponent.h"
#include "InputComponent.h"
#include "MeshComponent.h"
#include "GameAuraComponent.h"
#include <GameCostants.h>
#include "GameInstance.h"
#include "CameraComponent.h"
#include "MovementStatus.h"
#include "MovementComponent.h"
#include "AnimationComponent.h"
#include "GameCostants.h"
#include "GameMode.h"
#include "GameplayGameMode.h"
#include "AttackComponent.h"
#include "FeetComponent.h"
#include "SoundComponent.h"
#include "SoundData.h"
#include "TransformComponent.h"
#include "Character.h"
#include "MinHeightCheckComponent.h"
#include "InputEvents.h"
#include "GameEventManager.h"
#include "GameModeEvents.h"

namespace CCharacter
{
	std::function<void(FArrowUpEvent*)> m_handlerArrowUp;
	std::function<void(FSpacePressedEvent*)> m_handlerSpacePressed;
	bool b_isInvulnerable;


	CObject* CCharacter::CreateCharacter()
	{
		CObject* character = new CObject();
		CCharacter::b_isInvulnerable = false;

		CGameEventManager* p_eventManager = CGameInstance::GetGameEventManager();
		if (!p_eventManager) return nullptr;

		CCameraComponent* p_cameraComponent = new CCameraComponent(character);
		CTransformComponent* p_transformComponent = new CTransformComponent(character);
		CMinHeightCheckComponent* p_checkHeightComponent = new CMinHeightCheckComponent(character, CGameCostants::deathLevel);
		CMeshComponent* p_meshComponent = CCharacter::SetupMeshComponent(character);

		CCollisionComponent* p_collisionComponent = new CCollisionComponent(character, ECollisionProfile::CHARACTER);
		p_collisionComponent->SetBeginPlayCallback([p_collisionComponent, p_meshComponent]() {
			p_collisionComponent->SetBoundingBox(p_meshComponent->GetMeshBoundBox());
			});

		CFeetComponent* p_feetCollisionComponent = new CFeetComponent(character);
		p_feetCollisionComponent->SetBeginPlayCallback([p_feetCollisionComponent, p_meshComponent]() {
			p_feetCollisionComponent->SetBoxExtension(p_meshComponent->GetMeshBoundBox().width - CGameCostants::feet_safety_threshold, CGameCostants::characterFeetHeight);
			p_feetCollisionComponent->SetColliderOffset(FVector2D{ 0, -CGameCostants::characterFeetHeight });
			});


		std::map<EMovementStatus, FSoundData> movementSounds = {
			{EMovementStatus::RUNNING, FSoundData{CSoundData::sound_run, true, false}},
			{EMovementStatus::JUMPING, FSoundData{CSoundData::sound_jump, false, false}},
			{EMovementStatus::DOUBLEJUMPING, FSoundData{CSoundData::sound_doubleJump, false, false}}
		};
		CMovementComponent* p_movementComponent = new CMovementComponent(character, EMovementStatus::RUNNING, movementSounds);
		CAttackComponent* p_attackComponent = new CAttackComponent(character, { ECollisionProfile::ENEMY }, new CGameAura(EAuraResourceDamageMode::Flat, EAuraResource::Health, -1.f, 0.f, EAuraDurationType::Instant, 0.f, false, false), { CSoundData::sound_attackKnife, false, true });
		CSoundComponent* p_soundComponent = new CSoundComponent(character);
		CAnimationComponent* p_animationComponent = new CAnimationComponent(character);

		CGameAuraComponent* p_gameAuraComponent = new CGameAuraComponent(character);
		p_gameAuraComponent->SetResourceChangedCallback([p_gameAuraComponent, p_attackComponent, p_animationComponent](EAuraResource resource, float newValue, float delta) {
			OnResourceChanged(p_gameAuraComponent, p_attackComponent, p_animationComponent, resource, newValue, delta);
			});
		p_gameAuraComponent->SetResourceDrainedCallback([character, p_meshComponent, p_eventManager](EAuraResource resource, float newValue, float delta) {
			OnResourceDrained(p_meshComponent, p_eventManager, character, resource);
			});
		p_gameAuraComponent->Initialize();


		CInputComponent* p_inputComponent = new CInputComponent(character);
		p_inputComponent->SetBeginPlayCallback([p_inputComponent, p_eventManager, p_attackComponent, p_movementComponent]() {
			SetInputComponentState(p_inputComponent, p_eventManager, p_attackComponent, p_movementComponent, true);
			});
		p_inputComponent->SetBeginDestroyCallback([p_inputComponent, p_eventManager] {

			if (CCharacter::m_handlerArrowUp)
			{
				p_eventManager->RemoveSubscription<FArrowUpEvent>(p_inputComponent->GetUniqueID(), CCharacter::m_handlerArrowUp);
			}

			if (CCharacter::m_handlerSpacePressed)
			{
				p_eventManager->RemoveSubscription<FSpacePressedEvent>(p_inputComponent->GetUniqueID(), CCharacter::m_handlerSpacePressed);
			}
		});

		character->AddComponent(p_inputComponent);
		character->AddComponent(p_transformComponent);
		character->AddComponent(p_cameraComponent);
		character->AddComponent(p_collisionComponent);
		character->AddComponent(p_feetCollisionComponent);
		character->AddComponent(p_movementComponent);
		character->AddComponent(p_checkHeightComponent);
		character->AddComponent(p_gameAuraComponent);
		character->AddComponent(p_animationComponent);
		character->AddComponent(p_soundComponent);
		character->AddComponent(p_attackComponent);

		UI::BuildPlayerBackgroundComponent(character);
		CUIComponent* p_scoreComponent = UI::BuildPlayerScoreComponent(character);
		CUIComponent* p_statusComponent = UI::BuildPlayerStatusComponent(character);
		CUIComponent* p_livesComponent = UI::BuildPlayerLivesComponent(character);

		return character;
	}

	CMeshComponent* CCharacter::SetupMeshComponent(CObject* character)
	{
		CMeshComponent* p_meshComponent = new CMeshComponent(character);
		p_meshComponent->CreateSpriteMesh(CGameCostants::characterMesh, 8, 11);
		p_meshComponent->CreateAnimation(EAnimationID::RUN, 0.1f, { 8, 9, 10, 11, 12, 13 });
		p_meshComponent->CreateAnimation(EAnimationID::IDLE, 1.f, { 0, 1, 2, 3 });
		p_meshComponent->CreateAnimation(EAnimationID::JUMP, 0.5f, { 16, 17, 18, 19 });
		p_meshComponent->CreateAnimation(EAnimationID::DOUBLEJUMP, 0.1f, { 24, 25, 26, 27, 28, 29 });
		p_meshComponent->CreateAnimation(EAnimationID::HIT, 0.1f, { 72, 73 });
		p_meshComponent->CreateAnimation(EAnimationID::DIE, 0.1f, { 80, 81, 82, 83, 84, 85 });
		p_meshComponent->CreateAnimation(EAnimationID::ATTACK, 0.1f, { 56, 57, 58, 59, 60, 61 });
		p_meshComponent->PlayAnimation(EAnimationID::RUN);
		character->AddComponent(p_meshComponent);

		return p_meshComponent;
	}


	void CCharacter::SetInputComponentState(CInputComponent* inputComponent, CGameEventManager* eventManager, CAttackComponent* attackComponent, CMovementComponent* movementComponent, bool bIsActive)
	{
		if (bIsActive)
		{
			CCharacter::m_handlerArrowUp = [attackComponent, movementComponent](FArrowUpEvent* event) {
				OnJump(attackComponent, movementComponent);
				};
			eventManager->Subscribe<FArrowUpEvent>(inputComponent->GetUniqueID(), CCharacter::m_handlerArrowUp);

			CCharacter::m_handlerSpacePressed = [attackComponent](FSpacePressedEvent* event) {
				OnAttack(attackComponent);
			};
			eventManager->Subscribe<FSpacePressedEvent>(inputComponent->GetUniqueID(), CCharacter::m_handlerSpacePressed);
		}
		else
		{
			eventManager->RemoveSubscription<FArrowUpEvent>(inputComponent->GetUniqueID(), CCharacter::m_handlerArrowUp);
			eventManager->RemoveSubscription<FSpacePressedEvent>(inputComponent->GetUniqueID(), CCharacter::m_handlerSpacePressed);
		}
	}

	void CCharacter::OnResourceChanged(CGameAuraComponent* gameAuraComponent, CAttackComponent* attackComponent, CAnimationComponent* animationComponent, EAuraResource resource, float newValue, float delta)
	{
		switch (resource)
		{
		case EAuraResource::Health:
			if (gameAuraComponent->GetResourceValue(EAuraResource::Health) > 0 && delta < 0)
			{
				CGameAura* invulnerabilityAura = new CGameAura(EAuraResourceDamageMode::Flat, EAuraResource::Invulnerability, 1.f, 0.f, EAuraDurationType::Duration, 5.f, true, true);
				gameAuraComponent->ActivateAura(invulnerabilityAura);
			}
			break;
		case EAuraResource::Invulnerability:
			CCharacter::b_isInvulnerable = !CCharacter::b_isInvulnerable;
			animationComponent->ToggleInvincibility();
			attackComponent->EndAttack();
			break;
		}
	}

	void CCharacter::OnResourceDrained(CMeshComponent* meshComponent, CGameEventManager* eventManager, CObject* character, EAuraResource resource)
	{
		switch (resource)
		{
		case EAuraResource::Health:
			meshComponent->PlayAnimation(EAnimationID::DIE);
			if (eventManager)
			{
				eventManager->Publish<FGameOverEvent>(character->GetUniqueID(), new FGameOverEvent{});
			}
			break;
		}
	}

	void CCharacter::OnJump(CAttackComponent* attackComponent, CMovementComponent* movementComponent)
	{
		if (attackComponent->GetIsAttacking()) return;
		movementComponent->HandleJumpInput();
	}

	void CCharacter::OnAttack(CAttackComponent* attackComponent)
	{
		if (attackComponent->GetIsAttacking()) return;
		if (CCharacter::b_isInvulnerable) return;

		attackComponent->StartAttack();
	}
}