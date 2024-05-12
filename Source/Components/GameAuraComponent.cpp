#include "stdafx.h"
#include "GameAuraComponent.h"
#include "GameMode.h"
#include "GameInstance.h"
#include "GameplayGameMode.h"
#include <map>
#include "GameEventManager.h"
#include "GameResourceEvents.h"

CGameAuraComponent::CGameAuraComponent(CObject* inOwner) :
	CComponent(inOwner)
{
}

void CGameAuraComponent::Initialize()
{
	m_maxResources.insert(std::make_pair(EAuraResource::Health, 5.f));
	m_maxResources.insert(std::make_pair(EAuraResource::RunSpeed, 300.f));
	m_maxResources.insert(std::make_pair(EAuraResource::WalkSpeed, 130.f));
	m_maxResources.insert(std::make_pair(EAuraResource::JumpHeight, 80.f));
	m_maxResources.insert(std::make_pair(EAuraResource::Coin, 0.f));
	m_maxResources.insert(std::make_pair(EAuraResource::Invulnerability, 1.f));

	m_resources.insert(std::make_pair(EAuraResource::Health, 3.f));
	m_resources.insert(std::make_pair(EAuraResource::RunSpeed, 150.f));
	m_resources.insert(std::make_pair(EAuraResource::WalkSpeed, 80.f));
	m_resources.insert(std::make_pair(EAuraResource::JumpHeight, 30.f));
	m_resources.insert(std::make_pair(EAuraResource::Coin, 0.f));
	m_resources.insert(std::make_pair(EAuraResource::Invulnerability, 0.f));

	p_eventManager = CGameInstance::GetGameEventManager();
	if (!p_eventManager) return;

	CComponent::Initialize();
}

void CGameAuraComponent::DeInitialize()
{
	if (p_eventManager)
	{
		p_eventManager->RemoveAllSubscriptions(m_uniqueID);
	}

	v_p_activeAuras.clear();
	m_maxResources.clear();
	m_resources.clear();
	CComponent::DeInitialize();
}

void CGameAuraComponent::Update(float deltaTime)
{
	CComponent::Update(deltaTime);

	for (auto* aura : v_p_activeAuras)
	{
		aura->OnAuraTick(deltaTime);
	}
}

void CGameAuraComponent::SetResourceValue(EAuraResource resource, float value)
{
	if (m_resources.find(resource) != m_resources.end()) {
		if (m_maxResources[resource] < value) return;
		m_resources[resource] = value;
	}
}

float CGameAuraComponent::GetResourceValue(EAuraResource resource)
{
	if (m_resources.find(resource) != m_resources.end()) {
		return m_resources[resource];
	}
	return 0;
}

void CGameAuraComponent::SetResourceMaxValue(EAuraResource resource, float maxValue)
{
	if (m_maxResources.find(resource) != m_maxResources.end()) {
		m_maxResources[resource] = maxValue;
	}
}

float CGameAuraComponent::GetResourceMaxValue(EAuraResource resource)
{
	if (m_maxResources.find(resource) != m_maxResources.end()) {
		return m_maxResources[resource];
	}
	return 0;
}

void CGameAuraComponent::SetResourceToMax(EAuraResource resource)
{
	if (m_resources.find(resource) != m_resources.end())
	{
		m_resources[resource] = m_maxResources[resource];
	}
}

void CGameAuraComponent::AddToCurrentValue(EAuraResource resource, float value)
{
	if (GetResourceValue(EAuraResource::Invulnerability) != 0.f && resource == EAuraResource::Health)
	{
		return;
	}

	float newValue = m_resources[resource] + value;
	float delta = value;

	if (newValue > m_maxResources[resource] && m_maxResources[resource] != 0)
	{
		newValue = m_maxResources[resource];
		delta = newValue - m_resources[resource];
	}
	else if (newValue <= 0)
	{
		newValue = 0;
		delta = m_resources[resource];

		if (p_eventManager)
		{
			p_eventManager->Publish<FResourceDrainedEvent>(m_uniqueID, new FResourceDrainedEvent{ resource });
		}
		if (m_resourceDrainedCallback)
		{
			m_resourceDrainedCallback(resource, newValue, delta);
		}
	}

	m_resources[resource] = newValue;

	if (m_resourceChangedCallback)
	{
		m_resourceChangedCallback(resource, newValue, delta);
	}

	if (p_eventManager != nullptr)
	{
		p_eventManager->Publish<FResourceChangedEvent>(m_uniqueID, new FResourceChangedEvent{ resource, newValue, delta});
	}
}

bool CGameAuraComponent::IsAuraActive(CGameAura* aura)
{
	auto it = std::find(v_p_activeAuras.begin(), v_p_activeAuras.end(), aura);

	if (it != v_p_activeAuras.end()) {
		return true;
	}
	else
	{
		return false;
	}
}

void CGameAuraComponent::ActivateAura(CGameAura* aura)
{
	if (CanActivateAura(aura))
	{
		aura->SetTarget(this);
		v_p_activeAuras.push_back(aura);
		aura->OnAuraStart();
	}
}

void CGameAuraComponent::EndAura(CGameAura* aura)
{
	if (!IsAuraActive(aura)) return;

	aura->OnAuraEnd();
}

void CGameAuraComponent::RemoveAura(CGameAura* aura)
{
	auto it = std::find(v_p_activeAuras.begin(), v_p_activeAuras.end(), aura);

	if (it != v_p_activeAuras.end())
	{
		v_p_activeAuras.erase(it);
	}

	if (aura->GetDestroyAfterUse())
	{
		delete aura;
	}
}

bool CGameAuraComponent::CanActivateAura(CGameAura* aura)
{
	if (aura == nullptr) return false;

	for (int i = 0; i < v_p_activeAuras.size(); i++)
	{
		if (v_p_activeAuras[i] == aura)
			return false;

		if (v_p_activeAuras[i]->GetAuraResource() == aura->GetAuraResource())
			return false;

	}

	return true;
}

