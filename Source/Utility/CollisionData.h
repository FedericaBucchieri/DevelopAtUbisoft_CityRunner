#pragma once

#include <map>
#include <vector>
#include "Vector2D.h"

class CObject;
class CCollisionComponent;

enum ECollisionProfile
{
	NOCOLLISION,
	CHARACTER,
	ENEMY,
	OBSTACLE,
	COLLECTIBLE,
	FLOOR,
	CHARACTER_FEET,
	PERCEPTION
};

enum ECollisionResponse
{
	NONE,
	OVERLAP,
	BLOCK,
};

struct FCollisionPreset
{
	ECollisionProfile collisionProfile;
	std::map<ECollisionProfile, ECollisionResponse> collisionResponseMap;
};

struct FCollisionData
{
	bool bIsColliding = false;
	CObject* objectA;
	CObject* objectB;

	CCollisionComponent* collisionComponentA;
	CCollisionComponent* collisionComponentB;

	std::vector<FVector2D> contactPoints;
	FVector2D normal;
	float overlapDepth;

	ECollisionResponse collisionResponse;
};