#include "stdafx.h"
#include "GameAura.h"
#include "GameAuraComponent.h"
#include "AuraData.h"

CGameAura::CGameAura(EAuraResourceDamageMode damageMode, EAuraResource auraResource, float flatEffect, float ptcEffect, EAuraDurationType durationType, float duration, bool restoreValueOnEnd, bool destroyAfterUse) :
	p_target(nullptr),
	m_timeSinceAuraStarted(0),
	m_currentStacks(1),
	m_damageMode(damageMode),
	m_auraResource(auraResource),
	m_flatEffect(flatEffect),
	m_ptcEffect(ptcEffect),
	m_durationType(durationType),
	m_duration(duration),
	b_RestoreValueOnEnd(restoreValueOnEnd),
	b_DestroyAfterUser(destroyAfterUse)
{
}

CGameAura::CGameAura() :
	CGameAura(EAuraResourceDamageMode::Flat, EAuraResource::UNDEF, 0.f, 0.f, EAuraDurationType::Instant, 0.f, false, false)
{
}

void CGameAura::OnAuraStart()
{
	m_timeSinceAuraStarted = 0.0f;
	ApplyDamage();

	if (m_durationType == EAuraDurationType::Instant)
	{
		OnAuraEnd();
	}
}

void CGameAura::OnAuraTick(float deltaTime)
{
	m_timeSinceAuraStarted += deltaTime * 0.001f;

	if (m_durationType == EAuraDurationType::Instant)
	{
		return;
	}

	if (m_durationType == EAuraDurationType::Duration && m_timeSinceAuraStarted > m_duration)
	{
		OnAuraEnd();
	}
}

void CGameAura::OnAuraEnd()
{
	m_timeSinceAuraStarted = 0.0f;

	if (b_RestoreValueOnEnd)
	{
		float effect = 0;
		switch (m_damageMode)
		{
		case EAuraResourceDamageMode::Flat:
			p_target->AddToCurrentValue(m_auraResource, -m_flatEffect);
			break;
		case EAuraResourceDamageMode::CurAmountPCT:
			effect = p_target->GetResourceValue(m_auraResource) * m_ptcEffect;
			p_target->AddToCurrentValue(m_auraResource, -effect);
			break;
		case EAuraResourceDamageMode::MaxAmountPCT:
			effect = p_target->GetResourceMaxValue(m_auraResource) * m_ptcEffect;
			p_target->AddToCurrentValue(m_auraResource, -effect);
			break;
		}
	}

	if (p_target)
	{
		p_target->RemoveAura(this);
	}
}

void CGameAura::SetTarget(CGameAuraComponent* target)
{
	p_target = target;
}

void CGameAura::ApplyDamage()
{
	float effect = 0;
	switch (m_damageMode)
	{
	case EAuraResourceDamageMode::Flat:
		effect = m_flatEffect;
		break;
	case EAuraResourceDamageMode::CurAmountPCT:
		effect = p_target->GetResourceValue(m_auraResource) * m_ptcEffect;
		break;
	case EAuraResourceDamageMode::MaxAmountPCT:
		effect = p_target->GetResourceMaxValue(m_auraResource) * m_ptcEffect;
		break;
	}
	p_target->AddToCurrentValue(m_auraResource, effect);
}
