#pragma once
#include "Component.h"

//
// CCameraComponent is responsible for managing the camera in the game world.
//

class CTransformComponent;

class CCameraComponent : public CComponent
{
public:
    CCameraComponent(CObject* owner);
    CCameraComponent();

    void Initialize() override;
    void Update(float deltaTime);
    void Render();

    
    FVector2D GetPosition() const { return m_position;}
    void SetPosition(FVector2D newPosition) { m_position = newPosition; }
    float GetZoom() const { return m_zoom;}
    void SetZoom(float newZoom) { m_zoom = newZoom;}
    void SetStartingPosition(FVector2D position);
    FCameraData GetCameraData() { return m_data; }
    float GetCameraAttenuationMovementY() { return m_cameraAttenuationMovementY; }

    void RotateCameraUpsideDown() { m_rotationAngle == 0.f ? m_rotationAngle = 180.f : m_rotationAngle = 0.f; }
    bool IsInCameraView(FVector2D location, float width, float height);
    bool IsInCameraViewFromAppSide(FVector2D location, float width, float height);

private:
    CTransformComponent* p_transformComponent;
    FVector2D m_position;
    float m_zoom;
    float m_rotationAngle;
    float m_cameraAttenuationMovementY;
    FCameraData m_data;
};
