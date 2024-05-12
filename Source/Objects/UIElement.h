#pragma once
#include "Object.h"
#include <memory>

//
// CUIElement is a class representing a user interface element within the game.
// It manages properties like position and offset in conjunction with a UI component.
//


class CUIComponent;

class CUIElement : public CObject
{
public:
	CUIElement(CUIComponent& owner);
	CUIElement(CUIComponent& owner, const FVector2D& offsetFromOwner);

	void SetPosition(FVector2D newPosition) { m_position = newPosition; }
	FVector2D GetPosition() { return m_position; }

protected:
	FVector2D m_position;
	FVector2D m_offsetFromOwner;
	CUIComponent& p_owner;
};

