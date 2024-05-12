#pragma once
#include "GameMode.h"
#include "SoundData.h"
#include "InputEvents.h"

//
// CMenuGameMode represents the game mode for menu screens.
// It manages the UI component, input events, and background sound.
//

class CCameraComponent;

class CMenuGameMode : public CGameMode
{
public:
	CMenuGameMode(char* nextGameModeID, FSoundData backgroundSound);
	void Init() override;
	void DeInit() override;
	virtual CCameraComponent* GetCameraComponent() { return p_staticCameraComponent; }

protected:
	CCameraComponent* p_staticCameraComponent;
	FSoundData m_backgroundSound;
	std::function<void(FEnterPressedEvent*)> m_handlerEnterPressed;
};

