#include "stdafx.h"
#include "UIElementText.h"
#include "UIComponent.h"

CUIElementText::CUIElementText(CUIComponent& owner, FVector2D position, const char* inText) :
	CUIElement(owner, position),
	m_text(inText)
{
}

void CUIElementText::Render()
{
	App::Print(m_position.x, m_position.y, m_text.c_str(), 1.f, 1.f, 1.f, GLUT_BITMAP_HELVETICA_18);
}
