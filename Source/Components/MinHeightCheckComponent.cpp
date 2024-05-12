#include "stdafx.h"
#include "MinHeightCheckComponent.h"
#include "Component.h"
#include "TransformComponent.h"
#include "GameInstance.h"
#include "GameEventManager.h"
#include "GameModeEvents.h"

CMinHeightCheckComponent::CMinHeightCheckComponent(CObject* owner, float minY) :
	CComponent(owner),
	m_minY(minY)
{
}

void CMinHeightCheckComponent::Initialize()
{
	p_transformComponent = p_owner->GetFirstComponentOfClass<CTransformComponent>();
	if (p_transformComponent == nullptr) return;

	p_eventManager = CGameInstance::GetGameEventManager();
	if (p_eventManager == nullptr) return;

	CComponent::Initialize();
}

void CMinHeightCheckComponent::DeInitialize()
{
	if (p_eventManager)
	{
		p_eventManager->RemoveAllSubscriptions(m_uniqueID);
	}
}

void CMinHeightCheckComponent::Update(float deltaTime)
{
	CComponent::Update(deltaTime);

	if (b_isActive && p_transformComponent->GetLocation().y <= m_minY)
	{
		if (p_eventManager)
		{
			p_eventManager->Publish<FGameOverEvent>(p_owner->GetUniqueID(), new FGameOverEvent{});
			b_isActive = false;
			return;
		}
	}
}
