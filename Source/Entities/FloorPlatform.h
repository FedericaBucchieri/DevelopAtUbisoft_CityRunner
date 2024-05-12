#pragma once
#include "Object.h"
#include <deque>
#include "CollisionComponent.h"

//
// The CFloorPlatform namespace contains a function for creating floor platform objects in the game.
//

namespace CFloorPlatform
{
	CObject* CreatePlatform(FVector2D location, int inPlatformHeight, int inPlatformWidth);
}