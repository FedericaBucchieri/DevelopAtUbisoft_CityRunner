#pragma once
#include "stdafx.h"
#include "Component.h"
#include "SimpleSound.h"
#include "SoundData.h"
#include "SoundEvents.h"

//
// CSoundComponent is responsible for handling sound-related functionality for a game object.
// It plays, stops, and updates sounds based on events and conditions.
//

class CGameEventManager;

class CSoundComponent : public CComponent
{

public:
	CSoundComponent(CObject* owner);

	void OnBeginPlay() override;
	void OnBeginDestroy() override;
	void PlaySoundFromData(FSoundData soundData);
	void StopCurrentSound();
	void PlayCurrentSound();
	void Update(float deltaTime) override;

protected:
	FSoundData m_currentSound;
	FSoundData m_previousSound;

	CGameEventManager* p_eventManager;
	std::function<void(FPlaySoundEvent*)> m_handlerSound;
};

