#include "stdafx.h"
#include "TransformComponent.h"
#include "stdafx.h"
#include "Component.h"
#include "Object.h"
#include "MeshComponent.h"

CTransformComponent::CTransformComponent(CObject* object)
	: CComponent(object)
{
	m_forwardVector = { 1.f, 1.f };
}

void CTransformComponent::SetInitialLocation(FVector2D initLocation)
{
	SetLocation(initLocation);
	SetPreviousTransform(m_transform);
}


void CTransformComponent::Update(float deltaTime)
{
	CComponent::Update(deltaTime);
	
	SetPreviousTransform(m_transform);
}
