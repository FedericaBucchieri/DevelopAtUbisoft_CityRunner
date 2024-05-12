#pragma once
#include "GameEvent.h"

class CGameMode;


struct FDismissGameModeEvent : public FGameEvent {
    CGameMode* gameMode;
    char* newGameModeID;

    FDismissGameModeEvent(CGameMode* gm, char* id)
        : gameMode(gm), newGameModeID(id)
    {
    }
};

struct FGameOverEvent : FGameEvent {};
