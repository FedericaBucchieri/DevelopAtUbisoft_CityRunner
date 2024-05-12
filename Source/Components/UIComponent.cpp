#include "stdafx.h"
#include "UIComponent.h"
#include "UIElement.h"
#include "GameMode.h"
#include "CameraComponent.h"
#include "GameInstance.h"

CUIComponent::CUIComponent(CObject* inOwner, FVector2D position) :
    CComponent(inOwner),
    m_position(position),
    m_initialPosition(position),
    m_cameraAttenuation(0.0f)
{
}

CUIComponent::CUIComponent(CObject* inOwner) :
    CUIComponent(inOwner, FVector2D{ 0.f, 0.f })
{
}

void CUIComponent::Initialize()
{
    InitCameraData();
    for (const auto& uiElement : v_p_uiElements)
    {
        uiElement->Init();
    }
    CComponent::Initialize();
}

void CUIComponent::InitCameraData()
{
    CGameMode* p_gameMode = CGameInstance::GetInstance()->GetCurrentGameMode();
    if (p_gameMode == nullptr) return;
    p_cameraComponent = p_gameMode->GetCameraComponent();

    m_oldCameraPosition = p_cameraComponent->GetPosition();
    m_cameraCurrentPosition = m_oldCameraPosition;
    m_cameraAttenuation = p_cameraComponent->GetCameraAttenuationMovementY();
}

void CUIComponent::DeInitialize()
{
    for (int i = static_cast<int>(v_p_uiElements.size()) - 1; i >= 0; i--)
    {
        v_p_uiElements[i]->DeInit();
    }
    v_p_uiElements.clear();

    CComponent::DeInitialize();
}

void CUIComponent::Update(float deltaTime)
{
    CComponent::Update(deltaTime);

    m_cameraCurrentPosition = p_cameraComponent->GetPosition();
    for (const auto& uiElement : v_p_uiElements)
    {
        uiElement->SetPosition(ApplyInverseCameraTraslation(uiElement->GetPosition()));
        uiElement->Update(deltaTime);
    }
    m_oldCameraPosition = p_cameraComponent->GetPosition();
}

void CUIComponent::Render()
{
    CComponent::Render();
    if (!b_isActive) return;

    for (const auto& uiElement : v_p_uiElements)
    {
        uiElement->Render();
    }
}

void CUIComponent::AddUIElement(CUIElement* component)
{
    v_p_uiElements.push_back(component);
}

void CUIComponent::RemoveUIElement(CUIElement* component)
{
    auto it = std::find(v_p_uiElements.begin(), v_p_uiElements.end(), component);

    if (it != v_p_uiElements.end())
    {
        v_p_uiElements.erase(it);
    }
}

FVector2D CUIComponent::ApplyInverseCameraTraslation(const FVector2D& worldPosition)
{
    FVector2D cameraDelta = m_cameraCurrentPosition - m_oldCameraPosition;
    FVector2D screenPosition = worldPosition + FVector2D{ cameraDelta.x, cameraDelta.y * m_cameraAttenuation };

    return screenPosition;
}