#include "stdafx.h"
#include "CollisionComponent.h"
#include "Component.h"
#include "GameMode.h"
#include "GameplayGameMode.h"
#include "GameInstance.h"
#include "CollisionManager.h"
#include <app.h>
#include "TransformComponent.h"

CCollisionComponent::CCollisionComponent(CObject* inOwner, ECollisionProfile profile) : 
	CComponent(inOwner),
	m_collisionProfile(profile),
	m_offset(FVector2D{0.0f, 0.f}),
	m_lastUpdatePosition(FVector2D{ 0.0f, 0.f }),
	b_dirty(true)
{
}

void CCollisionComponent::RegisterComponent()
{
	CGameplayGameMode* p_gameplayGameMode = dynamic_cast<CGameplayGameMode*>(CGameInstance::GetCurrentGameMode());
	if (!p_gameplayGameMode) return;
	CCollisionManager* p_collisionManager = p_gameplayGameMode->GetCollisionManager();
	p_collisionManager->RegisterCollisionComponent(this);
}

void CCollisionComponent::UnRegisterComponent()
{
	CGameplayGameMode* p_gameplayGameMode = dynamic_cast<CGameplayGameMode*>(CGameInstance::GetCurrentGameMode());
	if (!p_gameplayGameMode) return;
	CCollisionManager* p_collisionManager = p_gameplayGameMode->GetCollisionManager();
	p_collisionManager->UnRegisterCollisionComponent(this);
}

void CCollisionComponent::Initialize()
{
	CComponent::Initialize();

	p_transformComponent = p_owner->GetFirstComponentOfClass<CTransformComponent>();
	if (p_transformComponent == nullptr) return;

	m_boundingBox.x = p_transformComponent->GetLocation().x + m_offset.x;
	m_boundingBox.y = p_transformComponent->GetLocation().y + m_offset.y;

	RegisterComponent();
}

void CCollisionComponent::DeInitialize()
{
	CComponent::DeInitialize();
	UnRegisterComponent();
}

void CCollisionComponent::Activate()
{
	CComponent::Activate();
	RegisterComponent();
}

void CCollisionComponent::Deactivate()
{
	CComponent::Deactivate();
	UnRegisterComponent();
}

void CCollisionComponent::Update(float deltaTime)
{
	CComponent::Update(deltaTime);
	FVector2D newLocation = FVector2D{ p_transformComponent->GetLocation() + m_offset};

	if (newLocation != m_lastUpdatePosition)
	{
		m_boundingBox.x = newLocation.x;
		m_boundingBox.y = newLocation.y;
		m_lastUpdatePosition = FVector2D{ m_boundingBox.x, m_boundingBox.y };
		b_dirty = true;
	}
}

void CCollisionComponent::Render()
{
	CComponent::Render();

	if(bDebug)
	{
		App::DrawLine(m_boundingBox.x, m_boundingBox.y, m_boundingBox.x + m_boundingBox.width, m_boundingBox.y, 1, 0, 0);
		App::DrawLine(m_boundingBox.x, m_boundingBox.y, m_boundingBox.x, m_boundingBox.y + m_boundingBox.height, 1, 0, 0);
		App::DrawLine(m_boundingBox.x, m_boundingBox.y + m_boundingBox.height, m_boundingBox.x + m_boundingBox.width, m_boundingBox.y + m_boundingBox.height, 1, 0, 0);
		App::DrawLine(m_boundingBox.x + m_boundingBox.width, m_boundingBox.y, m_boundingBox.x + m_boundingBox.width, m_boundingBox.y + m_boundingBox.height, 1, 0, 0);

		App::DrawLine(m_boundingBox.x, m_boundingBox.y - 10, m_boundingBox.x + 10.f, m_boundingBox.y - 10.f, 0, 1, 0);
	}
}

void CCollisionComponent::SetBoundingBox(FVector2D location, float width, float height)
{
	m_boundingBox.width = width;
	m_boundingBox.height = height;
	m_boundingBox.x = location.x;
	m_boundingBox.y = location.y;

	m_lastUpdatePosition = location;
}

void CCollisionComponent::SetBoxExtension(float width, float height)
{
	m_boundingBox.width = width;
	m_boundingBox.height = height;
}

bool CCollisionComponent::SetIsColliding(FCollisionData collisionData, CCollisionComponent* otherCollider)
{
	auto it = v_p_collidingWith.find(otherCollider);

	if (it == v_p_collidingWith.end()) {
		v_p_collidingWith.emplace(otherCollider, otherCollider->GetCollisionProfile());
		return true;
	}

	return false;
}

bool CCollisionComponent::StopColliding(CCollisionComponent* otherCollider)
{
	auto it = v_p_collidingWith.find(otherCollider);

	if (it != v_p_collidingWith.end()) {
		v_p_collidingWith.erase(it);
		return true;
	}

	return false;
}



bool CCollisionComponent::CheckCollisionWith(ECollisionProfile profile)
{
	for (const auto& pair : v_p_collidingWith) {
		if (pair.second == profile) {
			return true;
		}
	}

	return false;
}

std::vector<CObject*> CCollisionComponent::GetAllCollidingObjectsOfProfile(ECollisionProfile profile)
{
	std::vector<CObject*> collidingObjects;

	for (const auto& pair : v_p_collidingWith) {
		if (pair.second == profile) {
			collidingObjects.push_back(pair.first->GetOwner());
		}
	}

	return collidingObjects;
}


void CCollisionComponent::RemoveCollisionWith(ECollisionProfile profile)
{
	for (auto it = v_p_collidingWith.begin(); it != v_p_collidingWith.end(); ) {
		if (it->second == profile) {
			it = v_p_collidingWith.erase(it);
		}
		else {
			++it;
		}
	}
}

