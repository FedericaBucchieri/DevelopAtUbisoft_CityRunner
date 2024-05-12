#pragma once
#include "stdafx.h"
#include "Component.h"

//
// The CTransformComponent represents the transformation of an object in the game.
// Keeps track of the object's position, rotation, scale, and forward vector.
//


class CTransformComponent : public CComponent
{
public:
	CTransformComponent(CObject* object);

	void Update(float deltaTime) override;
	void SetInitialLocation(FVector2D initLocation);

	FTransform GetTransform() const { return m_transform; }
	void SetTransform(FTransform newTransform) { m_transform = newTransform; }

	FTransform GetPreviousTransform() const { return m_previousTransform; }
	void SetPreviousTransform(FTransform newPrevious) { m_previousTransform = newPrevious; }

	FVector2D GetLocation() const { return m_transform.location; }
	void SetLocation(FVector2D newLocation) { m_transform.location = newLocation; }

	float GetRotation() const { return m_transform.rotation; }
	void SetRotation(float newRotation) { m_transform.rotation = newRotation; }

	float GetScale() const { return m_transform.scale; }
	void SetScale(float newScale) { m_transform.scale = newScale; }

	FVector2D GetForwardVector() const { return m_forwardVector; }
	void SetForwardVector(FVector2D newVector) { m_forwardVector = newVector; }

protected:
	FTransform m_transform;
	FVector2D m_forwardVector;
	FTransform m_previousTransform;
};

