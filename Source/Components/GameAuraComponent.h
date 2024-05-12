#pragma once
#include "stdafx.h"
#include "Component.h"
#include "GameAura.h"
#include "GameResourceEvents.h"
#include <map>

//
// CGameAuraComponent is responsible for managing auras for a game object.
// It tracks resources, activates, and deactivates auras, and handles resource-related events.
//

class CGameEventManager;

class CGameAuraComponent : public CComponent
{

public:
	using ResourceCallback = std::function<void(EAuraResource, float, float)>;
	CGameAuraComponent(CObject* inOwner);

	void Initialize() override;
	void DeInitialize() override;

	void Update(float deltaTime) override;

	void SetResourceValue(EAuraResource resource, float value);
	float GetResourceValue(EAuraResource resource);

	void SetResourceMaxValue(EAuraResource resource, float maxValue);
	float GetResourceMaxValue(EAuraResource resource);

	void SetResourceToMax(EAuraResource resource);
	void AddToCurrentValue(EAuraResource resource, float value);

	void SetResourceChangedCallback(ResourceCallback callback) { m_resourceChangedCallback = callback; }
	void SetResourceDrainedCallback(ResourceCallback callback) { m_resourceDrainedCallback = callback; }

	bool IsAuraActive(CGameAura* aura);
	void ActivateAura(CGameAura* aura);
	void EndAura(CGameAura* aura);
	void RemoveAura(CGameAura* aura);
	bool CanActivateAura(CGameAura* aura);

protected:
	std::map<EAuraResource, float> m_resources;
	std::map<EAuraResource, float> m_maxResources;
	std::vector<CGameAura*> v_p_activeAuras;

	ResourceCallback m_resourceChangedCallback;
	ResourceCallback m_resourceDrainedCallback;

	CGameEventManager* p_eventManager;
};
