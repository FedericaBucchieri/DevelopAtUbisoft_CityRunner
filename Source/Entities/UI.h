#pragma once
#include "UIComponent.h"
#include "Object.h"

//
// The UI namespace contains a function for creating ui elements in the game.
//

namespace UI
{
	CUIComponent* BuildPlayerBackgroundComponent(CObject* owner);
	CUIComponent* BuildPlayerScoreComponent(CObject* owner);
	CUIComponent* BuildPlayerStatusComponent(CObject* owner);
	CUIComponent* BuildPlayerLivesComponent(CObject* owner);
	CUIComponent* BuildPauseUIComponent(CObject* owner);
	void UpdateLivesWidgetComponentsVisibility(CUIComponent* uiComponent, int startIdx, int endIdx, bool visibility);

	static const float s_lateralMargin = 100.f;
	static const float s_topMargin = 200.f;
	static const float s_widgetTopOffset = 15.f;
	static const float s_widgetLivesLateralOffset = 60.f;
	static const float s_widgetStatusLateralOffset = 50.f;
};

