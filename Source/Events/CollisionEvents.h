#pragma once
#include "GameEvent.h"

class CCollisionComponent;
class CObject;

struct FComponentBeginOverlapEvent : FGameEvent {
    CCollisionComponent* otherComponent;
    CObject* otherObject;

    FComponentBeginOverlapEvent(CCollisionComponent* component, CObject* object)
        : otherComponent(component), otherObject(object) {}
};

struct FComponentEndOverlapEvent : FGameEvent {
    CCollisionComponent* otherComponent;
    CObject* otherObject;

    FComponentEndOverlapEvent(CCollisionComponent* component, CObject* object)
        : otherComponent(component), otherObject(object) {}
};

struct FOverlappableOverlapedEvent : FGameEvent {
    CObject* collectibleOverlaped;

    FOverlappableOverlapedEvent(CObject* overlapped) : collectibleOverlaped(overlapped) {}
};
