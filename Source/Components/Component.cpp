#include "stdafx.h"
#include "Component.h"
#include "UniqueIdGenerator.h"
#include "GameInstance.h"

CComponent::CComponent() :
	p_owner(nullptr),
	b_isActive(true)
{
	m_uniqueID = CGameInstance::GetInstance()->GetUniqueIdGenerator()->GetNextId();
}

CComponent::CComponent(CObject* owner) :
	p_owner(owner),
	b_isActive(true)
{
	m_uniqueID = CGameInstance::GetInstance()->GetUniqueIdGenerator()->GetNextId();
}

CComponent::~CComponent()
{
	CGameInstance::GetInstance()->GetUniqueIdGenerator()->RecycleId(m_uniqueID);
}

void CComponent::OnBeginPlay()
{
	if (m_beginPlayCallback)
	{
		m_beginPlayCallback();
	}
}

void CComponent::OnBeginDestroy()
{
	if (m_beginDestroyCallback)
	{
		m_beginDestroyCallback();
	}
}

void CComponent::Update(float deltaTime)
{
	if (!b_isActive) return;
}

void CComponent::Render()
{
	if (!b_isActive) return;
}

