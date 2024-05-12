#include "stdafx.h"
#include "UIElementSprite.h"
#include "UIComponent.h"

CUIElementSprite::CUIElementSprite(CUIComponent& owner, FVector2D position, const char* fileName, int columns, int rows) :
	CUIElement(owner, position)
{
	p_sprite = std::make_unique<CSimpleSprite>(fileName, columns, rows);
	p_sprite->SetPosition(m_position.x - p_sprite->GetWidth()/2, m_position.y + p_sprite->GetHeight() / 2);
}

void CUIElementSprite::DeInit()
{
	p_sprite.release();
	CObject::DeInit();
}

void CUIElementSprite::Update(float deltaTime)
{
	CUIElement::Update(deltaTime);
	p_sprite->SetPosition(m_position.x - p_sprite->GetWidth() / 2, m_position.y + p_sprite->GetHeight()/2);
}

void CUIElementSprite::Render()
{
	if (b_isVisible)
	{
		p_sprite->Draw();
	}
}
