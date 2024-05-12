#pragma once
#include "GameEvent.h"
#include "MovementStatus.h"
#include "AnimationData.h"

struct FMovementStatusChangedEvent : FGameEvent {
    EMovementStatus newStatus;

    FMovementStatusChangedEvent(EMovementStatus status) : newStatus(status) {}
};


struct FAnimationEndEvent : FGameEvent {
    EAnimationID ID;

    FAnimationEndEvent(EAnimationID id) :  ID(id) {}
};

struct FAnimationStartEvent : FGameEvent {
    EAnimationID ID;

    FAnimationStartEvent(EAnimationID id) : ID(id) {}
};

struct FChangeDirectionEvent : FGameEvent {};