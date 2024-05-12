#pragma once
#include "Component.h"

//
// CMinHeightCheckComponent checks if the object's Y-coordinate falls below a minimum threshold.
// If it does, it triggers a game over event.
//

class CTransformComponent;
class CGameEventManager;

class CMinHeightCheckComponent : public CComponent
{
public: 
	CMinHeightCheckComponent(CObject* owner, float minY);
	void Initialize() override;
	void DeInitialize() override;
	void Update(float deltaTime);

protected:
	CGameEventManager* p_eventManager;
	CTransformComponent* p_transformComponent;
	float m_minY;
};

