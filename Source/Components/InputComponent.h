#pragma once
#include "Component.h"

//
// CInputComponent handles input-related functionality for a game object.
// It checks for specific input events and publishes corresponding events using the game's event manager.
//

class CObject;
class CGameEventManager;

class CInputComponent :public CComponent
{

public:
	CInputComponent(CObject* inOwner);
	CInputComponent();

	void Initialize() override;
	void DeInitialize() override;
	void Update(float deltaTime) override;

	bool GetArrowUp() const { return b_arrowUp; }

protected:
	bool b_arrowUp;
	bool b_enterPressed;
	bool b_spacePressed;

	CGameEventManager* p_eventManager;
};