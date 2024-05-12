#include "stdafx.h"
#include "GameInstance.h"
#include "GameMode.h"
#include "GameplayGameMode.h"
#include "MenuGameMode.h"
#include "UIElementSprite.h"
#include "UIElementText.h"
#include "UIComponent.h"
#include "GameEventManager.h"
#include <stdexcept>
#include "GameEvent.h"

CGameInstance* CGameInstance::s_istance = nullptr;
CGameMode* CGameInstance::p_currentGameMode = nullptr;
CUniqueIdGenerator CGameInstance::m_uniqueIdGenerator;
CGameEventManager* CGameInstance::p_gameEventManager = nullptr;

CGameInstance::CGameInstance() : 
	b_mustSwitchGameMode(false),
	m_callbackDismiss(0),
	m_nextGameModeID("")
{
}

CGameInstance* CGameInstance::GetInstance()
{
	if (s_istance == nullptr) {
		s_istance = new CGameInstance();
	}
	return s_istance;
}

void CGameInstance::Init()
{
	p_gameEventManager = new CGameEventManager();
	p_currentGameMode = new CMenuGameMode(CGameCostants::gameplayGameMode, {CSoundData::sound_mainMenu, true, false});
	InitializeMainMenuComponents();
	InitializeNewGameMode();
}

void CGameInstance::InitializeNewGameMode()
{
	p_currentGameMode->Init();

	m_handlerDismissGameMode = [this](FDismissGameModeEvent* event) {
		if (event->gameMode != p_currentGameMode) return;
		b_mustSwitchGameMode = true;
		m_nextGameModeID = event->newGameModeID;
	};

	p_gameEventManager->Subscribe<FDismissGameModeEvent>(p_currentGameMode->GetUniqueID(), m_handlerDismissGameMode);
}

void CGameInstance::DeInit()
{
	p_gameEventManager->RemoveSubscription<FDismissGameModeEvent>(p_currentGameMode->GetUniqueID(), m_handlerDismissGameMode);

	delete p_currentGameMode;
}

void CGameInstance::Update(float deltaTime)
{
	if (b_mustSwitchGameMode)
	{
		b_mustSwitchGameMode = false;
		ChangeGameMode();
		return;
	}

	p_currentGameMode->Update(deltaTime);
}

void CGameInstance::Render()
{
	p_currentGameMode->Render();
}

void CGameInstance::ChangeGameMode()
{
	p_currentGameMode->DeInit();
	delete p_currentGameMode;

	if (m_nextGameModeID == CGameCostants::mainMenuGameMode)
	{
		p_currentGameMode = new CMenuGameMode(CGameCostants::gameplayGameMode, {CSoundData::sound_mainMenu, true, false});
		InitializeMainMenuComponents();
	}
	else if (m_nextGameModeID == CGameCostants::gameplayGameMode)
	{
		p_currentGameMode = new CGameplayGameMode();
	}
	else if (m_nextGameModeID == CGameCostants::gameoverGameMode)
	{
		p_currentGameMode = new CMenuGameMode(CGameCostants::mainMenuGameMode, {nullptr, false, false});
		InitializeGameOverComponents();
	}
	InitializeNewGameMode();
}

void CGameInstance::InitializeGameOverComponents()
{
	CUIComponent* p_backgroundComponent = new CUIComponent(p_currentGameMode, FVector2D{ 0.f, 0.f });
	if (p_backgroundComponent == nullptr) return;

	CUIElementSprite* p_backgroundSpriteComponent = new CUIElementSprite(*p_backgroundComponent, FVector2D{ APP_VIRTUAL_WIDTH / 2, -APP_VIRTUAL_HEIGHT/8 - 20 }, CGameCostants::gameOverBackgroudMesh, 1, 1);
	p_backgroundComponent->AddUIElement(p_backgroundSpriteComponent);

	CUIElementText* p_inputTextComponent = new CUIElementText(*p_backgroundComponent, FVector2D{ 150.f, -100.f }, CGameCostants::goBackToMainMenuText);
	p_backgroundComponent->AddUIElement(p_inputTextComponent);

	p_currentGameMode->AddComponent(p_backgroundComponent);
}
void CGameInstance::InitializeMainMenuComponents()
{
	CUIComponent* p_backgroundComponent = new CUIComponent(p_currentGameMode, FVector2D{ 0.f, 0.f });
	if (p_backgroundComponent == nullptr) return;

	CUIElementSprite* p_backgroundSpriteComponent = new CUIElementSprite(*p_backgroundComponent, FVector2D{ APP_VIRTUAL_WIDTH / 2, -APP_VIRTUAL_HEIGHT / 8 - 20 }, CGameCostants::mainMenuBackgroundMesh, 1, 1);
	p_backgroundComponent->AddUIElement(p_backgroundSpriteComponent);

	CUIElementText* p_inputTextComponent = new CUIElementText(*p_backgroundComponent, FVector2D{ -90.f, 498.f }, CGameCostants::startText);
	p_backgroundComponent->AddUIElement(p_inputTextComponent);

	p_currentGameMode->AddComponent(p_backgroundComponent);
}