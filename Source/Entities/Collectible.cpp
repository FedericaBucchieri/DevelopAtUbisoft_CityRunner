#include "stdafx.h"
#include "Collectible.h"
#include "CollisionComponent.h"
#include "MeshComponent.h"
#include "GameAura.h"
#include "GameMode.h"
#include "OverlappableComponent.h"
#include "SoundComponent.h"
#include "SoundData.h"
#include "TransformComponent.h"

CObject* CCollectible::CreateCollectible(CGameAura* aura, std::vector<char*> inMeshNames, int inRows, int inColumns, FSoundData soundData)
{
	CObject* collectible = new CObject();

	int index = CRandom::GetRandomIntInRange(0, static_cast<int>(inMeshNames.size()) - 1);
	char* m_meshName = inMeshNames[index];

	CTransformComponent* p_transformComponent = new CTransformComponent(collectible);
	collectible->AddComponent(p_transformComponent);

	CMeshComponent* p_meshComponent = new CMeshComponent(collectible);
	p_meshComponent->CreateSpriteMesh(m_meshName, inColumns, inRows);
	if (inColumns > 1)
	{
		p_meshComponent->CreateAnimation(EAnimationID::IDLE, 0.1f, { 0,1,2,3,4,5 });
		p_meshComponent->PlayAnimation(EAnimationID::IDLE);
	}
	collectible->AddComponent(p_meshComponent);

	CCollisionComponent* p_collisionComponent = new CCollisionComponent(collectible, ECollisionProfile::COLLECTIBLE);
	p_collisionComponent->SetBeginPlayCallback([p_collisionComponent, p_meshComponent]() {
		p_collisionComponent->SetBoundingBox(p_meshComponent->GetMeshBoundBox());
		});
	collectible->AddComponent(p_collisionComponent);

	CSoundComponent* p_soundComponent = new CSoundComponent(collectible);
	collectible->AddComponent(p_soundComponent);

	COverlappableComponent* p_overlappableComponent = new COverlappableComponent(collectible, aura, ECollisionProfile::CHARACTER, [soundData, p_soundComponent]() {
		if (soundData.soundName == nullptr) return;
		p_soundComponent->PlaySoundFromData(soundData);
		});
	collectible->AddComponent(p_overlappableComponent);

	return collectible;
}
