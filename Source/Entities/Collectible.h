#pragma once
#include "stdafx.h"
#include "Object.h"
#include "SoundData.h"

//
// The CCollectible namespace contains a function for creating collectible objects in the game.
//

class CGameAura;

namespace CCollectible
{
	CObject* CreateCollectible(CGameAura* aura, std::vector<char*> inMeshNames, int inRows, int inColumns, FSoundData soundData);
}
