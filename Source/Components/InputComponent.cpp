#include "stdafx.h"
#include "app.h"
#include "InputComponent.h"
#include "GameEventManager.h"
#include "GameInstance.h"
#include "InputEvents.h"

CInputComponent::CInputComponent(CObject* inOwner) :
	CComponent(inOwner),
	b_arrowUp(false),
	b_enterPressed(false),
	p_eventManager(nullptr)
{
}

CInputComponent::CInputComponent() :
	CInputComponent(nullptr)
{
}

void CInputComponent::Initialize()
{
	p_eventManager = CGameInstance::GetGameEventManager();
	CComponent::Initialize();
}

void CInputComponent::DeInitialize()
{
	if (p_eventManager)
	{
		p_eventManager->RemoveAllSubscriptions(m_uniqueID);
	}
	CComponent::DeInitialize();
}

void CInputComponent::Update(float deltaTime)
{
	CComponent::Update(deltaTime);
	if (b_arrowUp = App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_UP, true))
	{
		p_eventManager->Publish<FArrowUpEvent>(m_uniqueID, new FArrowUpEvent{});
	}

	if (b_enterPressed = App::GetController().CheckButton(XINPUT_GAMEPAD_START, true))
	{
		p_eventManager->Publish<FEnterPressedEvent>(m_uniqueID, new FEnterPressedEvent{});
	}

	if (b_spacePressed = App::GetController().CheckButton(VK_PAD_A, true))
	{
		p_eventManager->Publish<FSpacePressedEvent>(m_uniqueID, new FSpacePressedEvent{});
	}
}