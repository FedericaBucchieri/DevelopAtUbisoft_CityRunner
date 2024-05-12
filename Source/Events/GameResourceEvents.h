#pragma once
#include "GameEvent.h"
#include "GameAura.h"

struct FResourceChangedEvent : FGameEvent {
    EAuraResource resource;
    float newValue;
    float delta;

    FResourceChangedEvent(EAuraResource res, float value, float change) : resource(res), newValue(value), delta(change) {}
};

struct FResourceDrainedEvent : FGameEvent {
    EAuraResource resource;

    FResourceDrainedEvent(EAuraResource res) : resource(res) {}
};