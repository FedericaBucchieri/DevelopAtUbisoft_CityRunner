#include "stdafx.h"
#include "CollectibleManager.h"
#include "Collectible.h"
#include "GameEventManager.h"
#include "CameraComponent.h"
#include "GameInstance.h"
#include "GameplayGameMode.h"
#include "OverlappableComponent.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "GameAura.h"

void CCollectibleManager::Init()
{
    CObject::Init();

    CGameplayGameMode* p_gameplayGameMode = dynamic_cast<CGameplayGameMode*>(CGameInstance::GetCurrentGameMode());
    if (p_gameplayGameMode == nullptr) return;

    p_cameraComponent = p_gameplayGameMode->GetCameraComponent();
    p_eventManager = CGameInstance::GetGameEventManager();

}

void CCollectibleManager::DeInit()
{
    for (int i = static_cast<int>(v_p_availableCoins.size()) - 1; i >= 0; i--)
    {
        v_p_availableCoins[i]->DeInit();
        p_eventManager->RemoveAllSubscriptions(v_p_availableCoins[i]->GetUniqueID());
    }
    v_p_availableCoins.clear();
    for (int i = static_cast<int>(v_p_inUseCoins.size()) - 1; i >= 0; i--)
    {
        v_p_inUseCoins[i]->DeInit();
        p_eventManager->RemoveAllSubscriptions(v_p_inUseCoins[i]->GetUniqueID());
    }
    v_p_inUseCoins.clear();

    for (int i = static_cast<int>(v_p_availableObstacles.size()) - 1; i >= 0; i--)
    {
        v_p_availableObstacles[i]->DeInit();
        p_eventManager->RemoveAllSubscriptions(v_p_availableObstacles[i]->GetUniqueID());
    }
    v_p_availableObstacles.clear();
    for (int i = static_cast<int>(v_p_inUseObstacles.size()) - 1; i >= 0; i--)
    {
        v_p_inUseObstacles[i]->DeInit();
        p_eventManager->RemoveAllSubscriptions(v_p_inUseObstacles[i]->GetUniqueID());
    }
    v_p_inUseObstacles.clear();

    for (int i = static_cast<int>(v_p_availableSpeedBoost.size()) - 1; i >= 0; i--)
    {
        v_p_availableSpeedBoost[i]->DeInit();
        p_eventManager->RemoveAllSubscriptions(v_p_availableSpeedBoost[i]->GetUniqueID());
    }
    v_p_availableSpeedBoost.clear();
    for (int i = static_cast<int>(v_p_inUseSpeedBoost.size()) - 1; i >= 0; i--)
    {
        v_p_inUseSpeedBoost[i]->DeInit();
        p_eventManager->RemoveAllSubscriptions(v_p_inUseSpeedBoost[i]->GetUniqueID());
    }
    v_p_inUseSpeedBoost.clear();

    for (int i = static_cast<int>(v_p_availableJumpBoost.size()) - 1; i >= 0; i--)
    {
        v_p_availableJumpBoost[i]->DeInit();
        p_eventManager->RemoveAllSubscriptions(v_p_availableJumpBoost[i]->GetUniqueID());
    }
    v_p_availableJumpBoost.clear();
    for (int i = static_cast<int>(v_p_inUseJumpBoost.size()) - 1; i >= 0; i--)
    {
        v_p_inUseJumpBoost[i]->DeInit();
        p_eventManager->RemoveAllSubscriptions(v_p_inUseJumpBoost[i]->GetUniqueID());

    }
    v_p_inUseJumpBoost.clear();

    for (int i = static_cast<int>(v_p_availableHealthCollectibles.size()) - 1; i >= 0; i--)
    {
        v_p_availableHealthCollectibles[i]->DeInit();
        p_eventManager->RemoveAllSubscriptions(v_p_availableHealthCollectibles[i]->GetUniqueID());

    }
    v_p_availableHealthCollectibles.clear();
    for (int i = static_cast<int>(v_p_inUseHealthCollectibles.size()) - 1; i >= 0; i--)
    {
        v_p_inUseHealthCollectibles[i]->DeInit();
        p_eventManager->RemoveAllSubscriptions(v_p_inUseHealthCollectibles[i]->GetUniqueID());

    }
    v_p_inUseHealthCollectibles.clear();

    CObject::DeInit();
}

void CCollectibleManager::Update(float deltaTime)
{
    CObject::Update(deltaTime);

    UpdateCollectibleLists(v_p_availableCoins, v_p_inUseCoins, deltaTime);
    UpdateCollectibleLists(v_p_availableSpeedBoost, v_p_inUseSpeedBoost, deltaTime);
    UpdateCollectibleLists(v_p_availableJumpBoost, v_p_inUseJumpBoost, deltaTime);
    UpdateCollectibleLists(v_p_availableHealthCollectibles, v_p_inUseHealthCollectibles, deltaTime);
    UpdateCollectibleLists(v_p_availableObstacles, v_p_inUseObstacles, deltaTime);
}

void CCollectibleManager::UpdateCollectibleLists(std::vector<CObject*>& availableCollectibles, std::deque<CObject*>& inUseCollectibles, float deltaTime)
{
    for (int i = static_cast<int>(inUseCollectibles.size()) - 1; i >= 0; i--)
    {
        CObject* collectible = inUseCollectibles[i];
        collectible->Update(deltaTime);

        CTransformComponent* p_transformComponent = collectible->GetFirstComponentOfClass<CTransformComponent>();
        if (p_transformComponent == nullptr) return;

        CMeshComponent* p_meshComponent = collectible->GetFirstComponentOfClass<CMeshComponent>();
        if (p_transformComponent == nullptr) return;

        if (!p_cameraComponent->IsInCameraViewFromAppSide(p_transformComponent->GetLocation(), p_meshComponent->GetMeshBoundBox().width, p_meshComponent->GetMeshBoundBox().height))
        {
            ReturnCollectible(collectible, availableCollectibles, inUseCollectibles);
        }
    }
}

void CCollectibleManager::Render()
{
    CObject::Render();

    for (int i = 0; i < static_cast<int>(v_p_inUseCoins.size()); i++)
    {
        v_p_inUseCoins[i]->Render();
    }

    for (int i = 0; i < static_cast<int>(v_p_inUseSpeedBoost.size()); i++)
    {
        v_p_inUseSpeedBoost[i]->Render();
    }

    for (int i = 0; i < static_cast<int>(v_p_inUseJumpBoost.size()); i++)
    {
        v_p_inUseJumpBoost[i]->Render();
    }

    for (int i = 0; i < static_cast<int>(v_p_inUseHealthCollectibles.size()); i++)
    {
        v_p_inUseHealthCollectibles[i]->Render();
    }

    for (int i = 0; i < static_cast<int>(v_p_inUseObstacles.size()); i++)
    {
        v_p_inUseObstacles[i]->Render();
    }
}

void CCollectibleManager::PoolRandomObject(FVector2D location)
{
    float randomObjectVariation = CRandom::GetRandomFloatInRange(0.f, 1.f);
    CGameAura* auraEffect = nullptr;
    CObject* collectible = nullptr;
    std::vector<char*> meshNames;
    FSoundData soundData = { nullptr, false, true };

    if (randomObjectVariation >= 0.0 && randomObjectVariation < 0.55f)
    {
        //Place Obstacle
        auraEffect = new CGameAura(EAuraResourceDamageMode::Flat, EAuraResource::Health, -1.f, 0.f, EAuraDurationType::Instant, 0.f, false, false);
        meshNames.push_back(CGameCostants::barrel1Mesh);
        meshNames.push_back(CGameCostants::barrel2Mesh);
        meshNames.push_back(CGameCostants::barrel3Mesh);
        soundData.soundName = CSoundData::sound_barrel;
        InitializeCollectible(location, v_p_availableObstacles, v_p_inUseObstacles, auraEffect, meshNames, 1, soundData);
    }
    else if (randomObjectVariation >= 0.55f && randomObjectVariation < 0.85f)
    {
        //Place Coins
        auraEffect = new CGameAura(EAuraResourceDamageMode::Flat, EAuraResource::Coin, 1.f, 0.f, EAuraDurationType::Instant, 0.f, false, false);
        meshNames.push_back(CGameCostants::coinMesh);
        soundData.soundName = CSoundData::sound_collectCoin;
        InitializeCollectible(location, v_p_availableCoins, v_p_inUseCoins, auraEffect, meshNames, CGameCostants::objectMeshColums, soundData);
    }
    else if (randomObjectVariation >= 0.85f && randomObjectVariation < 0.9f)
    {
        //Place speed boost
        auraEffect = new CGameAura(EAuraResourceDamageMode::MaxAmountPCT, EAuraResource::RunSpeed, 0.f, 0.5f, EAuraDurationType::Duration, 5.f, true, false);
        meshNames.push_back(CGameCostants::speedBoostMesh);
        soundData.soundName = CSoundData::sound_speedBoost;
        InitializeCollectible(location, v_p_availableSpeedBoost, v_p_inUseSpeedBoost, auraEffect, meshNames, CGameCostants::boostMeshColums, soundData);
    }
    else if (randomObjectVariation >= 0.9f && randomObjectVariation < 0.95f)
    {
        //Place jump boost
        auraEffect = new CGameAura(EAuraResourceDamageMode::MaxAmountPCT, EAuraResource::JumpHeight, 0.f, 0.5f, EAuraDurationType::Duration, 5.f, true, false);
        meshNames.push_back(CGameCostants::jumpBoostMesh);
        soundData.soundName = CSoundData::sound_jumpBoost;
        InitializeCollectible(location, v_p_availableJumpBoost, v_p_inUseJumpBoost, auraEffect, meshNames, CGameCostants::boostMeshColums, soundData);
    }
    else if (randomObjectVariation >= 0.95f && randomObjectVariation < 1.f)
    {
        //Place health Boost
        auraEffect = new CGameAura(EAuraResourceDamageMode::Flat, EAuraResource::Health, 1.f, 0.f, EAuraDurationType::Instant, 0.f, false, false);
        meshNames.push_back(CGameCostants::healthUpMesh);
        soundData.soundName = CSoundData::sound_healthUp;
        InitializeCollectible(location, v_p_availableHealthCollectibles, v_p_inUseHealthCollectibles, auraEffect, meshNames, CGameCostants::objectMeshColums, soundData);
    }
}

CObject* CCollectibleManager::RequestCollectible(std::vector<CObject*>& availableCollectibles, std::deque<CObject*>& inUseCollectibles, CGameAura* collectibleEffect, std::vector<char*> meshNames, int meshColumns, FSoundData soundData)
{
    CObject* collectible;

    if (availableCollectibles.empty())
    {
        collectible = CCollectible::CreateCollectible(collectibleEffect, meshNames, 1, meshColumns, soundData);
        collectible->Init();
    }
    else
    {
        collectible = availableCollectibles.back();
        collectible->Activate();
        availableCollectibles.pop_back();
    }

    inUseCollectibles.push_front(collectible);
    return collectible;
}

void CCollectibleManager::ReturnCollectible(CObject* toReturn, std::vector<CObject*>& availableCollectibles, std::deque<CObject*>& inUseCollectibles)
{
    if (!toReturn) return;
    if (inUseCollectibles.size() == 0) return;

    auto it = std::find(inUseCollectibles.begin(), inUseCollectibles.end(), toReturn);
    if (it != inUseCollectibles.end())
    {
        toReturn->Deactivate();
        auto handlerIt = m_handlers.find(toReturn);
        if (handlerIt != m_handlers.end()) {
            p_eventManager->RemoveSubscription<FOverlappableOverlapedEvent>(handlerIt->first->GetUniqueID(), handlerIt->second);
            m_handlers.erase(handlerIt);
        }
        inUseCollectibles.erase(it);
        availableCollectibles.push_back(toReturn);
    }
}

void CCollectibleManager::InitializeCollectible(FVector2D location, std::vector<CObject*>& availableCollectibles, std::deque<CObject*>& inUseCollectibles, CGameAura* auraEffect, std::vector<char*> meshNames, int meshColums, FSoundData soundData)
{
    CObject* collectible = RequestCollectible(availableCollectibles, inUseCollectibles, auraEffect, meshNames, meshColums, soundData);
    if (!collectible) return;

    CTransformComponent* p_transformComponent = collectible->GetFirstComponentOfClass<CTransformComponent>();
    if (p_transformComponent == nullptr) return;

    p_transformComponent->SetLocation(FVector2D{ location.x, location.y });

    if (p_eventManager)
    {
        COverlappableComponent* p_overlappableComponent = collectible->GetFirstComponentOfClass<COverlappableComponent>();
        if (p_overlappableComponent == nullptr) return;

        std::function<void(FOverlappableOverlapedEvent*)> handler = [this, &availableCollectibles, &inUseCollectibles](FOverlappableOverlapedEvent* event) {
            ReturnCollectible(event->collectibleOverlaped, availableCollectibles, inUseCollectibles);
            };
        p_eventManager->Subscribe<FOverlappableOverlapedEvent>(p_overlappableComponent->GetUniqueID(), handler);
        m_handlers.insert(std::make_pair(collectible, handler));
    }
}
