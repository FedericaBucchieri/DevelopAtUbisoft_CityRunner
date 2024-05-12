#include "stdafx.h"
#include "CameraComponent.h"
#include "Character.h"
#include "main.h"
#include "TransformComponent.h"

CCameraComponent::CCameraComponent(CObject* owner) :
	CComponent(owner),
	m_position(0.f, 0.f),
	m_zoom(1.f),
	m_rotationAngle(0.f),
	m_cameraAttenuationMovementY(0.3f)
{
}

CCameraComponent::CCameraComponent() :
	CComponent(nullptr),
	m_position(0.f, 0.f),
	m_zoom(1.f),
	m_rotationAngle(0.f),
	m_cameraAttenuationMovementY(0.3f)
{
	FCameraData::UpdateCameraData(m_position, m_data, m_zoom);
}

void CCameraComponent::Initialize()
{
	if (p_owner)
	{
		p_transformComponent = p_owner->GetFirstComponentOfClass<CTransformComponent>();
		if (p_transformComponent == nullptr) return;

		SetPosition(p_transformComponent->GetLocation());
	}
	CComponent::Initialize();
}

void CCameraComponent::SetStartingPosition(FVector2D position)
{
	SetPosition(position);
	FCameraData::UpdateCameraData(position, m_data, m_zoom);
}

void CCameraComponent::Update(float deltaTime)
{
	CComponent::Update(deltaTime);
	if (p_owner != nullptr)
	{
		FVector2D newPosition = p_transformComponent->GetLocation();
		SetPosition(newPosition);
		FCameraData::UpdateCameraData(newPosition, m_data, m_zoom);
	}
}

void CCameraComponent::Render()
{
	CComponent::Render();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float positionX = m_position.x;
	float positionY = m_position.y;
	APP_VIRTUAL_TO_NATIVE_COORDS(positionX, positionY);

	if (m_rotationAngle == 0.f)
	{
		glTranslatef(-positionX, -positionY * m_cameraAttenuationMovementY, 0.0f);
	}
	else if (m_rotationAngle == 180.f)
	{
		glTranslatef(positionX, positionY * m_cameraAttenuationMovementY, 0.0f);
	}
	glRotatef(m_rotationAngle, 0.0f, 0.0f, 1.0f);
	glScalef(m_zoom, m_zoom, m_zoom);

}

bool CCameraComponent::IsInCameraView(FVector2D location, float width, float height)
{
	if (location.x + width > m_data.left && location.x < m_data.right && location.y + height < m_data.top && location.y > m_data.bottom) {
		return true; 
	}

	return false;
}

bool CCameraComponent::IsInCameraViewFromAppSide(FVector2D location, float width, float height)
{

	if (m_rotationAngle == 0.f)
	{
		if (location.x + width > m_data.left) 
		{
			return true;
		}
	}
	else if(m_rotationAngle == 180.f)
	{
		m_data.left = -m_position.x - (m_data.screenWidth / 2);
		m_data.right = -m_position.x + (m_data.screenWidth / 2);

		location.x = -location.x;

		if (location.x - width < m_data.right)
		{
			return true;
		}
	}

	return false;
}
