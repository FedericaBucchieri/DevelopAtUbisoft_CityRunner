#pragma once

enum class EAuraResource
{
	UNDEF,
	Health,
	RunSpeed,
	WalkSpeed,
	JumpHeight,
	Coin,
	Invulnerability,
};


enum class EAuraDurationType
{
	Instant,
	Duration,
	Permanent,
};

enum class EAuraResourceDamageMode
{
	UNDEF,
	Flat,
	CurAmountPCT,
	MaxAmountPCT,
};