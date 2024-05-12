#pragma once
#include "stdafx.h"
#include "Object.h"
#include "GameModeEvents.h"
#include "InputEvents.h"

//
// CGameMode is the base class for different game modes in the game.
// It manages objects in the scene, initializes and updates them.
//

class CCameraComponent;
class CGameEventManager;
class CInputComponent;
class CSoundComponent;

class CGameMode : public CObject
{
public:
    CGameMode(char* nexGameModeID);
    CGameMode();

    virtual ~CGameMode();
    void Init() override;
    void DeInit() override;
    void Update(float deltaTime) override;
    void Render() override;
    virtual CCameraComponent* GetCameraComponent() { return nullptr; }
    char* GetNextGameMode() { return m_nextGameModeID; }
    virtual void OnDismissGameMode();

protected:
    CGameEventManager* p_eventManager;
    std::vector<CObject*> v_m_objectsInScene;
    CInputComponent* p_inputComponent;
    CSoundComponent* p_soundComponent;

    std::function<void(FDismissGameModeEvent*)> m_handlerDismissGameMode;
    std::function<void(FEnterPressedEvent*)> m_handlerPause;
    char* m_nextGameModeID;
    bool b_isPaused;
};
