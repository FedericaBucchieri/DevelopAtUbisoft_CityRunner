#pragma once
#include "stdafx.h"
#include "UIElement.h"
#include <string>

//
// CUIElementText extends the functionality of CUIElement and represents a text-based UI element.
// The class encapsulates a string representing the text and handles rendering accordingly.
//


class CUIElementText : public CUIElement
{

public: 
	CUIElementText(CUIComponent& owner, FVector2D position, const char* inText);

	void Render() override;

	void SetText(const char* newText) { m_text = newText; }
	std::string GetText() { return m_text; }

protected:
	std::string m_text;
};

