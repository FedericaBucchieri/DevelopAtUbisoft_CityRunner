#pragma once
#include "stdafx.h"
#include "UniqueIdGenerator.h"
#include "GameEventManager.h"
#include "GameModeEvents.h"

//
// CGameInstance manages the game instance, including the current game mode, event manager, and mode switching.
// It initializes and updates the current game mode, handles mode changes, and initializes UI components for different game modes.
//

class CGameMode;

class CGameInstance
{	
public:
	void Init();
	void DeInit();
	void Update(float deltaTime);
	void Render();

	static CGameInstance* GetInstance();
	static CGameMode* GetCurrentGameMode() { return p_currentGameMode; }
	static CGameEventManager* GetGameEventManager() { return p_gameEventManager; }
	void SetCurrentGameMode(CGameMode* gameMode) { p_currentGameMode = gameMode; }
	static std::shared_ptr<CUniqueIdGenerator> GetUniqueIdGenerator() { return std::make_shared<CUniqueIdGenerator>(m_uniqueIdGenerator); }


private:
	CGameInstance();
	CGameInstance(CGameInstance& other) = delete;
	void operator=(const CGameInstance&) = delete;

	void InitializeNewGameMode();
	void ChangeGameMode();
	void InitializeMainMenuComponents();
	void InitializeGameOverComponents();

	static CGameInstance* s_istance;
	static CGameMode* p_currentGameMode;
	static CUniqueIdGenerator m_uniqueIdGenerator;
	static CGameEventManager* p_gameEventManager;
	std::function<void(FDismissGameModeEvent*)> m_handlerDismissGameMode;

	bool b_mustSwitchGameMode;
	char* m_nextGameModeID;
	int m_callbackDismiss;
};

