#include "stdafx.h"
#include "GameplayGameMode.h"
#include "BackgroundPool.h"
#include "WorldManager.h"
#include "CollisionManager.h"
#include "CameraComponent.h"
#include "GameEventManager.h"
#include "Character.h"
#include "GameInstance.h"
#include "InputComponent.h"
#include "SoundComponent.h"
#include "TransformComponent.h"

CGameplayGameMode::CGameplayGameMode() :
	b_isGameplayActive(true),
	b_isPaused(false),
	m_handlerGameOver(nullptr)
{
	p_collisionManager = new CCollisionManager();
	p_playerCharacter = CCharacter::CreateCharacter();
	p_backgroundPool = new CBackgroundPool();
	p_platformManager = new CWorldManager();
}

void CGameplayGameMode::Init()
{
	p_collisionManager->Init();
	p_playerCharacter->Init();
	p_backgroundPool->Init();
	p_platformManager->Init();

	p_soundComponent->Initialize();
	AddComponent(p_soundComponent);

	SetupPauseComponent();

	v_m_objectsInScene.push_back(p_backgroundPool);
	v_m_objectsInScene.push_back(p_platformManager);
	v_m_objectsInScene.push_back(p_collisionManager);
	v_m_objectsInScene.push_back(p_playerCharacter);

	p_playerCharacter->Deactivate();

	p_eventManager = CGameInstance::GetGameEventManager();
	if (p_eventManager)
	{
		m_handlerGameOver = [this](FGameOverEvent* event) {
			StopGameplay();
		};
		p_eventManager->Subscribe<FGameOverEvent>(p_playerCharacter->GetUniqueID(), m_handlerGameOver);

		p_inputComponent = p_playerCharacter->GetFirstComponentOfClass<CInputComponent>();
		if (p_inputComponent)
		{
			m_handlerPause = [this](FEnterPressedEvent* event){
					TogglePaused();
			};
			p_eventManager->Subscribe<FEnterPressedEvent>(p_inputComponent->GetUniqueID(), m_handlerPause);
		}
	}

	CTransformComponent* p_transformComponent = p_playerCharacter->GetFirstComponentOfClass<CTransformComponent>();
	if (p_transformComponent == nullptr) return;

	p_transformComponent->SetLocation(FVector2D{ 0.f,  p_platformManager->GetInitialGroundLevel() });
	GetCameraComponent()->SetStartingPosition(p_transformComponent->GetLocation());

	p_soundComponent->PlaySoundFromData({CSoundData::sound_background, true, true });
}

void CGameplayGameMode::DeInit()
{
	if (p_eventManager)
	{
		p_eventManager->RemoveSubscription<FGameOverEvent>(p_playerCharacter->GetUniqueID(), m_handlerGameOver);
		p_eventManager->RemoveSubscription<FEnterPressedEvent>(p_inputComponent->GetUniqueID(), m_handlerPause);
	}
	CGameMode::DeInit();
}

void CGameplayGameMode::Update(float deltaTime)
{
	if (!b_isGameplayActive) return;

	if (b_isPaused)
	{
		p_inputComponent->Update(deltaTime);
		return;
	}

	CGameMode::Update(deltaTime);

	if (!b_initialized)
	{
		b_initialized = true;
		p_playerCharacter->Activate();
	}
}

CCameraComponent* CGameplayGameMode::GetCameraComponent()
{
	return p_playerCharacter->GetFirstComponentOfClass<CCameraComponent>();
}

void CGameplayGameMode::StartGameplay()
{
	b_isGameplayActive = true;
}

void CGameplayGameMode::StopGameplay()
{
	b_isGameplayActive = false;

	CSoundComponent* p_playerSoundComponent = p_playerCharacter->GetFirstComponentOfClass<CSoundComponent>();
	if (p_playerSoundComponent)
	{
		p_playerSoundComponent->StopCurrentSound();
	}

	p_soundComponent->PlaySoundFromData(FSoundData{ CSoundData::sound_gameover, false, false });
	p_eventManager->Publish<FDismissGameModeEvent>(m_uniqueID, new FDismissGameModeEvent{ this, CGameCostants::gameoverGameMode });
}

void CGameplayGameMode::SetupPauseComponent()
{
	p_pauseComponent = UI::BuildPauseUIComponent(this);
	p_pauseComponent->Initialize();
	AddComponent(p_pauseComponent);
	p_pauseComponent->Deactivate();
}

void CGameplayGameMode::TogglePaused()
{
	b_isPaused = !b_isPaused;

	CSoundComponent* p_playerSoundComponent = p_playerCharacter->GetFirstComponentOfClass<CSoundComponent>();
	if (p_playerSoundComponent != nullptr)
	{
		b_isPaused ? p_playerSoundComponent->StopCurrentSound() : p_playerSoundComponent->PlayCurrentSound();
	}

	b_isPaused ? p_pauseComponent->Activate() : p_pauseComponent->Deactivate();
}