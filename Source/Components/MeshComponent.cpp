#include "stdafx.h"
#include "MeshComponent.h"
#include "Object.h"
#include "SimpleSprite.h"
#include "AnimationData.h"
#include <app.h>
#include "TransformComponent.h"

CMeshComponent::CMeshComponent(CObject* inOwner) : 
	CComponent(inOwner),
	p_sprite(nullptr),
	m_transform(),
	m_currentAnimationID(EAnimationID::IDLE)
{
}

CMeshComponent::CMeshComponent(CObject* inOwner, std::vector<char*> fileNames, int columns, int rows):
	CComponent(inOwner),
	m_transform(),
	m_currentAnimationID(EAnimationID::IDLE)
{
	int index = CRandom::GetRandomIntInRange(0, static_cast<int>(fileNames.size()));
	p_sprite = App::CreateSprite(fileNames[index], columns, rows);
}

void CMeshComponent::Initialize()
{
	p_transformComponent = p_owner->GetFirstComponentOfClass<CTransformComponent>();
	if (p_transformComponent == nullptr) return;
	m_transform = p_transformComponent->GetTransform();

	SetSpriteTransform(m_transform);
}

void CMeshComponent::CreateSpriteMesh(char* fileName, int columns, int rows)
{
	p_sprite = App::CreateSprite(fileName, columns, rows);

	SetSpriteTransform(m_transform);
}

void CMeshComponent::CreateAnimation(EAnimationID animationID, float speed, const std::vector<int>& frames)
{
	auto it = v_animations.find(animationID);

	if (it == v_animations.end())
	{
		p_sprite->CreateAnimation(animationID, speed, frames);
		v_animations.emplace(animationID, speed * static_cast<int>(frames.size()));
	}
}

void CMeshComponent::PlayAnimation(EAnimationID animationID)
{
	auto it = v_animations.find(animationID);

	if (it != v_animations.end()) {
		p_sprite->SetAnimation(animationID);
		m_currentAnimationID = animationID;
	}
}

float CMeshComponent::GetAnimationDuration(EAnimationID animationID)
{
	auto it = v_animations.find(animationID);

	if (it != v_animations.end())
	{
		return it->second;
	}

	return 0.f;
}

void CMeshComponent::Update(float deltaTime)
{
	CComponent::Update(deltaTime);
	m_transform = p_transformComponent->GetTransform();
	
	SetSpriteTransform(m_transform);
	p_sprite->Update(deltaTime);
}

void CMeshComponent::Render()
{
	CComponent::Render();
	p_sprite->Draw();

	if(bDebug)
	{
		float x;
		float y;
		p_sprite->GetPosition(x, y);
		App::DrawLine(x, y, x + p_sprite->GetWidth(), y, 0, 1, 0);
		App::DrawLine(x, y, x, y + p_sprite->GetHeight(), 0, 1, 0);
		App::DrawLine(x, y + p_sprite->GetHeight(), x + p_sprite->GetWidth(), y + p_sprite->GetHeight(), 0, 1, 0);
		App::DrawLine(x + p_sprite->GetWidth(), y, x + p_sprite->GetWidth(), y + p_sprite->GetHeight(), 0, 1, 0);
	}
}

void CMeshComponent::SetSpriteTransform(FTransform m_transform)
{
	p_sprite->SetPosition(m_transform.location.x + p_sprite->GetWidth() / 2, m_transform.location.y + p_sprite->GetHeight() / 2);
	p_sprite->SetAngle(m_transform.rotation);
	p_sprite->SetScale(m_transform.scale);
}

FMeshBoundBox CMeshComponent::GetMeshBoundBox()
{
	FMeshBoundBox BoundBox;
	BoundBox.width = p_sprite->GetWidth() * m_transform.scale;
	BoundBox.height = p_sprite->GetHeight() * m_transform.scale;
	float x;
	float y;
	p_sprite->GetPosition(x, y);
	BoundBox.x = x;
	BoundBox.y = y;
	return BoundBox;
}
