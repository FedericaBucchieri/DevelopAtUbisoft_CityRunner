#include "stdafx.h"
#include "MenuGameMode.h"
#include "GameInstance.h"
#include "UniqueIdGenerator.h"
#include "CameraComponent.h"
#include "GameEventManager.h"
#include "InputComponent.h"
#include "SoundData.h"
#include "SoundComponent.h"
#include "InputEvents.h"

CMenuGameMode::CMenuGameMode(char* nextGameModeID, FSoundData backgroundSound) :
	CGameMode(nextGameModeID),
	p_staticCameraComponent(nullptr),
	m_backgroundSound(backgroundSound)
{
}

void CMenuGameMode::Init()
{
	p_staticCameraComponent = new CCameraComponent();
	AddComponent(p_staticCameraComponent);

	p_inputComponent = new CInputComponent();
	AddComponent(p_inputComponent);

	CGameMode::Init();

	p_eventManager = CGameInstance::GetGameEventManager();
	if (p_eventManager)
	{
		m_handlerEnterPressed = [this](FEnterPressedEvent* event) {
			OnDismissGameMode();
		};
		p_eventManager->Subscribe<FEnterPressedEvent>(p_inputComponent->GetUniqueID(), m_handlerEnterPressed);
	}

	if (p_soundComponent)
	{
		p_soundComponent->PlaySoundFromData(m_backgroundSound);
	}
}

void CMenuGameMode::DeInit()
{
	if (p_soundComponent)
	{
		p_soundComponent->StopCurrentSound();
	}
	p_eventManager->RemoveSubscription<FDismissGameModeEvent>(p_inputComponent->GetUniqueID(), m_handlerDismissGameMode);
	CGameMode::DeInit();
}

