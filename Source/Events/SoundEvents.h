#pragma once
#include "GameEvent.h"
#include "SoundData.h"

struct FPlaySoundEvent : FGameEvent {
    FSoundData soundData;

    FPlaySoundEvent(FSoundData sound) : soundData(sound) {}
};