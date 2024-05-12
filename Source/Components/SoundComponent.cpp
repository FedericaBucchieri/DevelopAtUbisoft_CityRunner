#include "app.h"
#include "stdafx.h"
#include "SoundComponent.h"
#include "Object.h"
#include "Component.h"
#include "GameEventManager.h"
#include "GameInstance.h"
#include "SoundData.h"
#include "SoundEvents.h"

CSoundComponent::CSoundComponent(CObject* owner) :
	CComponent(owner),
	p_eventManager(nullptr),
	m_handlerSound(nullptr)
{
}

void CSoundComponent::OnBeginPlay()
{
	CComponent::OnBeginPlay();

	p_eventManager = CGameInstance::GetGameEventManager();
	if (p_eventManager)
	{
		m_handlerSound = [this](FPlaySoundEvent* event) {
			PlaySoundFromData(event->soundData);
			};
		p_eventManager->Subscribe<FPlaySoundEvent>(p_owner->GetUniqueID(), m_handlerSound);
	}
}

void CSoundComponent::OnBeginDestroy()
{
	StopCurrentSound();
	if (p_eventManager)
	{
		p_eventManager->RemoveSubscription<FPlaySoundEvent>(p_owner->GetUniqueID(), m_handlerSound);
	}

	CComponent::OnBeginDestroy();
}

void CSoundComponent::PlaySoundFromData(FSoundData soundData)
{
	if (m_currentSound.soundName == soundData.soundName) return;

	if (App::IsSoundPlaying(m_currentSound.soundName))
	{
		App::StopSound(m_currentSound.soundName);
	}

	if (soundData.soundName == nullptr) return;

	m_previousSound = m_currentSound;
	m_currentSound = soundData;

	App::PlaySoundW(soundData.soundName, soundData.looping);

	if (!soundData.looping)
	{
		m_currentSound.soundName = nullptr;
	}
}

void CSoundComponent::StopCurrentSound()
{
	if (App::IsSoundPlaying(m_currentSound.soundName))
	{
		App::StopSound(m_currentSound.soundName);
	}
}

void CSoundComponent::PlayCurrentSound()
{
	if (m_currentSound.soundName == nullptr) return;

	if (!App::IsSoundPlaying(m_currentSound.soundName) )
	{
		App::PlaySoundW(m_currentSound.soundName, m_currentSound.looping);
	}
}

void CSoundComponent::Update(float deltaTime)
{
	CComponent::Update(deltaTime);

	if (!App::IsSoundPlaying(m_currentSound.soundName) && m_previousSound.soundName != nullptr && m_currentSound.playPreviousOnEnd)
	{
		App::PlaySoundW(m_previousSound.soundName, true);
		m_currentSound = m_previousSound;
	}
}