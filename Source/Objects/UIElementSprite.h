#pragma once
#include "stdafx.h"
#include "UIElement.h"
#include <vector>
#include <memory>

//
// CUIElementSprite extends the functionality of CUIElement and represents a UI element with a graphical sprite.
// The class encapsulates a unique sprite object and handles initialization, updates, and rendering accordingly.
//


class CSimpleSprite;

class CUIElementSprite : public CUIElement
{

public:
	CUIElementSprite(CUIComponent& owner, FVector2D position, const char* fileName, int columns, int rows);

	void DeInit() override;
	void Update(float deltaTime) override;
	void Render() override;

	void ToggleVisibility() { b_isVisible = !b_isVisible; }
	void SetVisibility(bool isVisible) { b_isVisible = isVisible; }

protected:
	std::unique_ptr<CSimpleSprite> p_sprite;
	bool b_isVisible = true;
};

