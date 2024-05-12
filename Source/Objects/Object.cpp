#include "stdafx.h"
#include "Object.h"
#include "UniqueIdGenerator.h"
#include "GameMode.h"
#include "GameInstance.h"

CObject::~CObject()
{
	for (int i = static_cast<int>(v_p_components.size()) - 1; i >= 0; i--)
	{
		if(v_p_components[i] != nullptr)
			delete v_p_components[i];
	}
	v_p_components.clear();
}

void CObject::Init()
{
	m_uniqueID = CGameInstance::GetInstance()->GetUniqueIdGenerator()->GetNextId();
	for (CComponent* component : v_p_components) {
		component->Initialize();
	}
	OnBeginPlay();
}

void CObject::DeInit()
{
	int i = 0;
	for (CComponent* component : v_p_components) {
		i++;
		component->DeInitialize();
	}
 	OnBeginDestroy();
}

void CObject::OnBeginPlay()
{
	for (CComponent* component : v_p_components) {
		component->OnBeginPlay();
	}
}

void CObject::OnBeginDestroy()
{
	for (CComponent* component : v_p_components) {
		component->OnBeginDestroy();
	}
	CGameInstance::GetInstance()->GetUniqueIdGenerator()->RecycleId(m_uniqueID);
}

void CObject::Activate()
{
	b_isActive = true;
	for (CComponent* component : v_p_components) {
		component->Activate();
	}
}

void CObject::Deactivate()
{
	b_isActive = false;
	for (CComponent* component : v_p_components) {
		component->Deactivate();
	}
}

void CObject::Update(float deltaTime)
{
	if (!b_isActive) return;

	for (CComponent* component : v_p_components) {
		component->Update(deltaTime);
	}
}

void CObject::Render()
{
	if (!b_isActive) return;
	for (CComponent* component : v_p_components) {
		component->Render();
	}
}

void CObject::AddComponent(CComponent* component)
{
	v_p_components.push_back(component);
	AddComponentToMap(component);
}

void CObject::RemoveComponent(CComponent* component)
{
	auto it = std::find(v_p_components.begin(), v_p_components.end(), component);

	if (it != v_p_components.end()) {
		v_p_components.erase(it);
		RemoveComponentFromMap(component);
	}
}