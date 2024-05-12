#pragma once
#include "GameMode.h"
#include "GameModeEvents.h"

//
// CGameplayGameMode manages the gameplay environment, including the player character, platforms, collisions, and background.
// It handles the initialization, update, and events related to the gameplay, including pausing and game over.
//

class CCameraComponent;
class CCollisionManager;
class CObject;
class CGameEventManager;
class CWorldManager;
class CBackgroundPool;
class CUIComponent;

class CGameplayGameMode : public CGameMode
{
public:
	CGameplayGameMode();
	void Init() override;
	void DeInit() override;
	void Update(float deltaTime) override;

	CCameraComponent* GetCameraComponent() override;
	CCollisionManager* GetCollisionManager() { return p_collisionManager; }
	CObject* GetPlayerCharacter() { return p_playerCharacter; }

protected:
	void StartGameplay();
	void StopGameplay();
	void SetupPauseComponent();
	void TogglePaused();


	CCollisionManager* p_collisionManager;
	CObject* p_playerCharacter;
	CWorldManager* p_platformManager;
	CBackgroundPool* p_backgroundPool;
	CUIComponent* p_pauseComponent;

	bool b_isGameplayActive;
	std::function<void(FGameOverEvent*)> m_handlerGameOver;
	bool b_isPaused;
	bool b_initialized;
};

