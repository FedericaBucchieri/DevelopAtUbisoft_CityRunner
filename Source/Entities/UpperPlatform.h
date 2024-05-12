#pragma once
#include "Object.h"
#include "FloorPlatform.h"

//
// The CUpperPlatform namespace contains a function for creating upper platform objects in the game.
//

namespace CUpperPlatform
{
	CObject* CreateUpperPlatform(FVector2D location, int inPlatformHeight, int inPlatformWidth);
}