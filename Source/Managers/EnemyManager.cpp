#include "stdafx.h"
#include "EnemyManager.h"
#include "Enemy.h"
#include "GameAura.h"
#include "EnemyData.h"
#include "GameEventManager.h"
#include "GameInstance.h"
#include "GameplayGameMode.h"
#include "CameraComponent.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "PerceptionComponent.h"
#include "EnemyEvents.h"

void CEnemyManager::Init()
{
    CObject::Init();

    CGameplayGameMode* p_gameplayGameMode = dynamic_cast<CGameplayGameMode*>(CGameInstance::GetCurrentGameMode());
    if (p_gameplayGameMode == nullptr) return;

    p_cameraComponent = p_gameplayGameMode->GetCameraComponent();
    p_eventManager = CGameInstance::GetGameEventManager();

}

void CEnemyManager::DeInit()
{
    for (int i = static_cast<int>(v_p_availableEnemies.size()) - 1; i >= 0; i--)
    {
        v_p_availableEnemies[i]->DeInit();
        p_eventManager->RemoveAllSubscriptions(v_p_availableEnemies[i]->GetUniqueID());
    }
    v_p_availableEnemies.clear();

    for (int i = static_cast<int>(v_p_inUseEnemies.size()) - 1; i >= 0; i--)
    {
        v_p_inUseEnemies[i]->DeInit();
        p_eventManager->RemoveAllSubscriptions(v_p_inUseEnemies[i]->GetUniqueID());
    }
    v_p_inUseEnemies.clear();

    CObject::DeInit();
}

void CEnemyManager::Update(float deltaTime)
{
    CObject::Update(deltaTime);

    for (int i = static_cast<int>(v_p_inUseEnemies.size()) - 1; i >= 0; i--)
    {
        CObject* enemy = v_p_inUseEnemies[i];
        enemy->Update(deltaTime);

        CTransformComponent* p_transformComponent = enemy->GetFirstComponentOfClass<CTransformComponent>();
        if (p_transformComponent == nullptr) return;

        CMeshComponent* p_meshComponent = enemy->GetFirstComponentOfClass<CMeshComponent>();
        if (p_transformComponent == nullptr) return;

        if (!p_cameraComponent->IsInCameraViewFromAppSide(p_transformComponent->GetLocation(), p_meshComponent->GetMeshBoundBox().width, p_meshComponent->GetMeshBoundBox().height))
        {
            ReturnEnemy(enemy);
        }
    }
}

void CEnemyManager::Render()
{
    CObject::Render();

    for (int i = 0; i < static_cast<int>(v_p_inUseEnemies.size()); i++)
    {
        v_p_inUseEnemies[i]->Render();
    }
}

void CEnemyManager::PlaceEnemiesOnPlatform(FVector2D platformLocation, float platformWidth, float platformHeight)
{
    int numEnemies = CRandom::GetRandomIntInRange(1, CGameCostants::max_enemies_per_platform);

    for (int i = 0; i < numEnemies; i++)
    {
        float minX = platformLocation.x;
        float maxX = minX + platformWidth;
        float safetyThreshold = 100.f;
        float x = CRandom::GetRandomFloatInRange(minX + safetyThreshold, maxX - safetyThreshold);
        FVector2D location = FVector2D{ x, platformLocation.y + platformHeight + safetyThreshold * 1.5f};

        RequestEnemy(location, minX, maxX);
    }
}

FEnemyData CEnemyManager::GetEnemyData()
{
    FEnemyData data;
    data.lives = 1.f;
    data.attackEffect = new CGameAura(EAuraResourceDamageMode::Flat, EAuraResource::Health, -1.f, 0.f, EAuraDurationType::Instant, 0.f, false, false);
    data.perceptionRadius = 300.f;
    data.attackInterval = 1.f;
    data.attackRadius = 50.f;
    data.attackDelay = 0.1f;
    data.enemySprite = CGameCostants::enemyWithBathMesh;
    data.spriteRows = 6;
    data.spritColumns = 10;

    return data;
}

void CEnemyManager::RequestEnemy(FVector2D location, float minX, float maxX)
{
    CObject* enemy = nullptr;
    FEnemyData data = GetEnemyData();

    if (v_p_availableEnemies.empty())
    {
        enemy = CEnemy::CreateEnemy(location, data);
        enemy->Init();
        InitializeEnemy(minX, maxX, enemy);
    }
    else
    {
        enemy = v_p_availableEnemies.back();

        CTransformComponent* p_transformComponent = enemy->GetFirstComponentOfClass<CTransformComponent>();
        if (p_transformComponent != nullptr) {
            p_transformComponent->SetLocation(location);
        }

        enemy->Activate();
        InitializeEnemy(minX, maxX, enemy);
        v_p_availableEnemies.pop_back();
    }


    v_p_inUseEnemies.push_front(enemy);
}


void CEnemyManager::InitializeEnemy(float minX, float maxX, CObject* enemy)
{
    CPerceptionComponent* p_perceptionComponent = enemy->GetFirstComponentOfClass<CPerceptionComponent>();
    if (p_perceptionComponent != nullptr)
    {
        p_perceptionComponent->SetMaxX(maxX);
        p_perceptionComponent->SetMinX(minX);
    }

    if (p_eventManager)
    {
        std::function<void(FEnemyDeathEvent*)> handler = [this](FEnemyDeathEvent* event) {
            ReturnEnemy(event->enemy);
            };
        p_eventManager->Subscribe<FEnemyDeathEvent>(enemy->GetUniqueID(), handler);
        m_handlers.insert(std::make_pair(enemy, handler));
    }
}

void CEnemyManager::ReturnEnemy(CObject* toReturn)
{
    if (!toReturn) return;
    if (v_p_inUseEnemies.size() == 0) return;

    auto it = std::find(v_p_inUseEnemies.begin(), v_p_inUseEnemies.end(), toReturn);
    if (it != v_p_inUseEnemies.end())
    {
        toReturn->Deactivate();

        auto handlerIt = m_handlers.find(toReturn);
        if (handlerIt != m_handlers.end()) 
        {
            p_eventManager->RemoveSubscription<FEnemyDeathEvent>(handlerIt->first->GetUniqueID(), handlerIt->second);
            m_handlers.erase(handlerIt);
        }

        v_p_inUseEnemies.erase(it);
        v_p_availableEnemies.push_back(toReturn);
    }
}