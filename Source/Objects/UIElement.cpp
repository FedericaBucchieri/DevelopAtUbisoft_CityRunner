#include "stdafx.h"
#include "UIElement.h"
#include "UIComponent.h"

CUIElement::CUIElement(CUIComponent& owner) :
    m_position(owner.GetPosition()),
    m_offsetFromOwner(FVector2D{ 0.f, 0.f }),
    p_owner(owner)
{
}

CUIElement::CUIElement(CUIComponent& owner, const FVector2D& offsetFromOwner) :
    p_owner(owner),
    m_offsetFromOwner(offsetFromOwner)
{
    m_position = p_owner.GetPosition() + m_offsetFromOwner;
}







