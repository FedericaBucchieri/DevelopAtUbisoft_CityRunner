#pragma once
#include "stdafx.h"
#include "CollisionComponent.h"
#include "CollisionEvents.h"

//
// CFeetComponent is a collision component that is specific for representing the feet of an object/character in the game.
// It specifies if an object is touching the ground or not.
//

class CMovementComponent;
class CGameEventManager;

class CFeetComponent : public CCollisionComponent
{
public:
	CFeetComponent(CObject* owner);
	void OnBeginPlay() override;
	void OnBeginDestroy() override;

protected:
	void OnBeginOverlap(CCollisionComponent* otherComponent, CObject* otherObject);
	void OnEndOverlap(CCollisionComponent* otherComponent, CObject* otherObject);

	CMovementComponent* p_movementComponent;
	CGameEventManager* p_eventManager;
	
	std::function<void(FComponentBeginOverlapEvent*)> m_handlerBeginOverlap;
	std::function<void(FComponentEndOverlapEvent*)> m_handlerEndOverlap;
};