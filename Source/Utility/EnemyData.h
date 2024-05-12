#pragma once

class CGameAura;

struct FEnemyData
{
	char* enemySprite;
	int spriteRows;
	int spritColumns;
	float perceptionRadius;
	float lives;
	float attackRadius;
	float attackInterval;
	float attackDelay;
	CGameAura* attackEffect;
};