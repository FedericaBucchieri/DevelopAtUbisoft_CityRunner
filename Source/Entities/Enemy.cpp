#include "stdafx.h"
#include "Enemy.h"
#include "GameInstance.h"
#include "MeshComponent.h"
#include "AnimationComponent.h"
#include "CollisionComponent.h"
#include "GameAuraComponent.h"
#include "MovementComponent.h"
#include "GameEventManager.h"
#include "GameAura.h"
#include "PerceptionComponent.h"
#include "EnemyData.h"
#include "AttackComponent.h"
#include "SoundComponent.h"
#include "SoundData.h"
#include "TransformComponent.h"
#include "EnemyEvents.h"

CObject* CEnemy::CreateEnemy(FVector2D location, FEnemyData enemyData)
{
	CObject* enemy = new CObject();
	CGameEventManager* p_eventManager = CGameInstance::GetGameEventManager();

	CTransformComponent* p_transformComponent = new CTransformComponent(enemy);
	p_transformComponent->SetLocation(location);
	enemy->AddComponent(p_transformComponent);

	CMeshComponent* p_meshComponent = new CMeshComponent(enemy);
	p_meshComponent->CreateSpriteMesh(enemyData.enemySprite, enemyData.spriteRows, enemyData.spritColumns);
	p_meshComponent->CreateAnimation(EAnimationID::IDLE, 0.1f, { 0, 1, 2, 3, 4, 5 });
	p_meshComponent->CreateAnimation(EAnimationID::ATTACK, 0.1f, { 6, 7, 8, 9, 10, 11 });
	p_meshComponent->CreateAnimation(EAnimationID::DIE, 0.1f, { 12, 13, 14, 15, 16, 17 });
	p_meshComponent->CreateAnimation(EAnimationID::HIT, 0.1f, { 18, 19 });
	p_meshComponent->CreateAnimation(EAnimationID::IDLELEFT, 0.1f, { 35, 34, 33, 32, 31, 30 });
	p_meshComponent->CreateAnimation(EAnimationID::ATTACKLEFT, 0.1f, { 41, 40, 39, 38, 37, 36 });
	p_meshComponent->CreateAnimation(EAnimationID::DIELEFT, 0.1f, { 47, 46 ,45 ,44 , 43, 42 });
	p_meshComponent->CreateAnimation(EAnimationID::HITLEFT, 0.1f, { 49, 48 });
	p_meshComponent->PlayAnimation(EAnimationID::IDLE);
	enemy->AddComponent(p_meshComponent);	

	CCollisionComponent* p_collisionComponent = new CCollisionComponent(enemy, ECollisionProfile::ENEMY);
	p_collisionComponent->SetBeginPlayCallback([p_collisionComponent, p_meshComponent]() {
		p_collisionComponent->SetBoundingBox(p_meshComponent->GetMeshBoundBox());
		});
	enemy->AddComponent(p_collisionComponent);

	CMovementComponent* p_movementComponent = new CMovementComponent(enemy, EMovementStatus::IDLESTATUS);
	enemy->AddComponent(p_movementComponent);

	CCollisionComponent* m_perceptionCollisionComponent = new CCollisionComponent(enemy, ECollisionProfile::PERCEPTION);
	enemy->AddComponent(m_perceptionCollisionComponent);

	CPerceptionComponent* p_perceptionComponent = new CPerceptionComponent(enemy, enemyData, m_perceptionCollisionComponent);
	enemy->AddComponent(p_perceptionComponent);

	CAttackComponent* p_attackComponent = new CAttackComponent(enemy, { ECollisionProfile::CHARACTER }, enemyData.attackEffect, { CSoundData::sound_attackPunch, false, false });
	enemy->AddComponent(p_attackComponent);

	CAnimationComponent* p_animationComponent = new CAnimationComponent(enemy);
	p_animationComponent->SetStartAnimationCallback([p_perceptionComponent, p_attackComponent](EAnimationID ID) {
		if (ID == EAnimationID::DIE)
		{
			p_perceptionComponent->ResetEffect();
			p_attackComponent->EndAttack();
		}
		});
	p_animationComponent->SetEndAnimationCallback([p_eventManager, enemy ](EAnimationID ID) {
		if (ID == EAnimationID::DIE)
		{
			OnEnemyDeath(p_eventManager, enemy);
		}
		});
	enemy->AddComponent(p_animationComponent);

	CSoundComponent* p_soundComponent = new CSoundComponent(enemy);
	enemy->AddComponent(p_soundComponent);

	CGameAuraComponent* p_gameAuraComponent = new CGameAuraComponent(enemy);
	p_gameAuraComponent->SetBeginPlayCallback([enemyData, p_gameAuraComponent]() {
		p_gameAuraComponent->SetResourceMaxValue(EAuraResource::Health, enemyData.lives);
		p_gameAuraComponent->SetResourceValue(EAuraResource::Health, enemyData.lives);
		});
	p_gameAuraComponent->SetResourceDrainedCallback([enemy, p_animationComponent](EAuraResource resource, float newValue, float delta) {
		OnResourceDrained(p_animationComponent, resource);
		});
	enemy->AddComponent(p_gameAuraComponent);

	return enemy;
}

void CEnemy::OnResourceDrained(CAnimationComponent* animationComponent, EAuraResource resource)
{
	switch (resource)
	{
	case EAuraResource::Health:
		animationComponent->PlayAnimationOnce(EAnimationID::DIE);
	}
}

void CEnemy::OnEnemyDeath(CGameEventManager* eventManager, CObject* enemy)
{
	CSoundComponent* p_soundComponent = enemy->GetFirstComponentOfClass<CSoundComponent>();
	if (p_soundComponent)
	{
		p_soundComponent->StopCurrentSound();
	}

	if (eventManager)
	{
		eventManager->Publish<FEnemyDeathEvent>(enemy->GetUniqueID(), new FEnemyDeathEvent{ enemy });
	}
}