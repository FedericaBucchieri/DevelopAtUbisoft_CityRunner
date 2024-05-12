#pragma once
#include "Object.h"
#include "CollisionComponent.h"
#include "CollisionEvents.h"

//
// CCollisionManager handles collision detection and response. It uses collision presets to define the responses between different collision profiles.
// The manager maintains a list of collision components and updates them to check for collisions during the game loop.
//


class CGameEventManager;

class CCollisionManager : public CObject
{
public:
	void Init() override;
	void DeInit() override;
	void Update(float deltaTime) override;

	void RegisterCollisionComponent(CCollisionComponent* component);
	void UnRegisterCollisionComponent(CCollisionComponent* component);

private:
	void CheckCollision(CCollisionComponent* componentA, CCollisionComponent* componentB, float deltaTime);
	std::vector<FVector2D> CalculateContactPoints(const FMeshBoundBox boxA, const FMeshBoundBox boxB);
	FVector2D CalculateCollisionNormal(const FMeshBoundBox boxA, const FMeshBoundBox boxB);
	float CalculateOverlapDepth(const FMeshBoundBox boxA, const FMeshBoundBox boxB);
	FVector2D CalculateCollisionImpulse(const FCollisionData& collisionData);
	ECollisionResponse FindCollisionResponse(CCollisionComponent* componentA, CCollisionComponent* componentB);

	void HandleStartCollision(FCollisionData collisionData, float deltaTime);
	void HandleEndCollision(CCollisionComponent* componentA, CCollisionComponent* componentB);
	void BlockOnCollision(FCollisionData collisionData, float deltaTime);

	std::vector<FCollisionPreset> m_collisionPresets;
	std::vector<CCollisionComponent*> v_p_components;
	CGameEventManager* p_eventManager;
};

