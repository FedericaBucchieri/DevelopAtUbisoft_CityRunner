#pragma once
#include "stdafx.h"
#include <functional>

//
// CComponent is the base class for all components attached to game objects.
// It provides common functionality such as initialization, update, rendering, and activation/deactivation of components.
//

class CObject;

class CComponent {

public:
    using SetupCallback = std::function<void()>;

    virtual ~CComponent();
    virtual void Initialize() {};
    virtual void DeInitialize() {};
    virtual void OnBeginPlay();
    virtual void OnBeginDestroy();

    virtual void Update(float deltaTime);
    virtual void Render();

    virtual void Activate() { b_isActive = true; };
    virtual void Deactivate() { b_isActive = false; };

    int GetUniqueID() { return m_uniqueID; }
    CObject* GetOwner() { return p_owner; }
    void SetBeginPlayCallback(SetupCallback callback) { m_beginPlayCallback = callback; }
    void SetBeginDestroyCallback(SetupCallback callback) { m_beginDestroyCallback = callback; }

protected:
    CComponent();
	CComponent(CObject* owner);

    CObject* p_owner;
    int m_uniqueID;
    bool b_isActive;

    SetupCallback m_beginPlayCallback;
    SetupCallback m_beginDestroyCallback;
};
