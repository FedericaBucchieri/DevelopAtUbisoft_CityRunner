#pragma once
#include "Component.h"
#include <vector>
#include <memory>

//
// CUIComponent represents a UI elements within the game.
//

class CObject;
class CUIElement;
class CCameraComponent;

class CUIComponent : public CComponent
{
public:
	CUIComponent(CObject* inOwner, FVector2D position);
	CUIComponent(CObject* inOwner);

	void Initialize() override;
	void InitCameraData();
	void DeInitialize() override;
	void Update(float deltaTime) override;
	void Render() override;

	void AddUIElement(CUIElement* component);
	void RemoveUIElement(CUIElement* component);
	FVector2D ApplyInverseCameraTraslation(const FVector2D& worldPosition);
	std::vector<CUIElement*> GetUIElements() { return v_p_uiElements; }

	FVector2D GetPosition() { return m_position; }
	void SetPosition(FVector2D newPosition) { m_position = newPosition; }
	FVector2D GetInitialPosition() { return m_initialPosition; }

protected:
	std::vector<CUIElement*> v_p_uiElements;
	FVector2D m_position;
	FVector2D m_initialPosition;

	CCameraComponent* p_cameraComponent;
	FVector2D m_oldCameraPosition;
	FVector2D m_cameraCurrentPosition;
	float m_cameraAttenuation;
};

