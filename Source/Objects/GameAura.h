#pragma once
#include "Object.h"
#include "AuraData.h"

//
// The CGameAura class represents a game aura with configurable properties such as damage mode, resource type, effects, and duration. 
// Auras acts on game resources to apply effect to objects like power ups or pickable.
//


class CGameAuraComponent;

class CGameAura : public CObject
{

public:
	CGameAura(EAuraResourceDamageMode damageMode, EAuraResource auraResource, float flatEffect, float ptcEffect, EAuraDurationType durationType, float duration, bool restoreValueOnEnd, bool destroyAfterUse);
	CGameAura();
	
	virtual void OnAuraStart();
	virtual void OnAuraTick(float DeltaTime);
	virtual void OnAuraEnd();
	
	void SetTarget(CGameAuraComponent* target);
	bool IsIstantAura() { return m_durationType == EAuraDurationType::Instant; }
	bool GetDestroyAfterUse() { return b_DestroyAfterUser; }
	EAuraResource GetAuraResource() { return m_auraResource; }

protected:
	virtual void ApplyDamage();
	CGameAuraComponent* p_target;

	EAuraResourceDamageMode m_damageMode;
	EAuraResource m_auraResource;
	float m_flatEffect;
	float m_ptcEffect;
	bool b_RestoreValueOnEnd;
	EAuraDurationType m_durationType;
	float m_duration;
	bool b_DestroyAfterUser;
	float m_timeSinceAuraStarted;
	int m_currentStacks;
};

