#include "stdafx.h"
#include "GameMode.h"
#include "GameInstance.h"
#include "UniqueIdGenerator.h"
#include "CameraComponent.h"
#include "InputComponent.h"
#include "SoundComponent.h"
#include "GameEventManager.h"
#include "GameEvent.h"
#include "GameModeEvents.h"

CGameMode::CGameMode(char* nexGameModeID) :
	m_nextGameModeID(nexGameModeID),
	p_eventManager(nullptr),
	m_handlerDismissGameMode(nullptr),
	m_handlerPause(nullptr)
{
	m_uniqueID = CGameInstance::GetInstance()->GetUniqueIdGenerator()->GetNextId();
	b_isActive = true;
	p_soundComponent = new CSoundComponent(this);
}

CGameMode::CGameMode() :
	CGameMode("")
{
}

CGameMode::~CGameMode()
{
	for (int i = static_cast<int>(v_m_objectsInScene.size()) -1 ; i >= 0; i--)
	{
		delete v_m_objectsInScene[i];
	}

	CGameInstance::GetInstance()->GetUniqueIdGenerator()->RecycleId(m_uniqueID);
}

void CGameMode::Init()
{
	p_eventManager = CGameInstance::GetGameEventManager();

	if (p_soundComponent)
	{
		AddComponent(p_soundComponent);
	}

	CObject::Init();

	for (CObject* object : v_m_objectsInScene)
	{
		object->Init();
	}
}

void CGameMode::DeInit()
{
	if (p_eventManager)
	{
		p_eventManager->RemoveAllSubscriptions(m_uniqueID);
	}

	for (CObject* object : v_m_objectsInScene)
	{
		object->DeInit();
	}
	CObject::DeInit();
}

void CGameMode::Update(float deltaTime)
{
	CObject::Update(deltaTime);
	for (CObject* object : v_m_objectsInScene)
	{
		object->Update(deltaTime);
	}
}

void CGameMode::Render()
{
	for (CObject* object : v_m_objectsInScene)
	{
		object->Render();
	}
	CObject::Render();
}

void CGameMode::OnDismissGameMode()
{
	p_eventManager->Publish<FDismissGameModeEvent>(m_uniqueID, new FDismissGameModeEvent{this, m_nextGameModeID});
}

