#include "stdafx.h"
#include "WorldManager.h"
#include <random>
#include <string>
#include <vector>
#include <deque>
#include "CameraComponent.h"
#include "FloorPlatform.h"
#include "GameMode.h"
#include "Collectible.h"
#include "Random.h"
#include "OverlappableComponent.h"
#include "GameInstance.h"
#include "GameplayGameMode.h"
#include "Enemy.h"
#include "SoundData.h"
#include "EnemyManager.h"
#include "CollectibleManager.h"
#include "TilesComponent.h"
#include "TransformComponent.h"
#include "UpperPlatformData.h"
#include "UpperPlatform.h"
#include "GameEventManager.h"

void CWorldManager::Init()
{
    CGameplayGameMode* p_gameplayGameMode = dynamic_cast<CGameplayGameMode*>(CGameInstance::GetCurrentGameMode());
    if (p_gameplayGameMode == nullptr) return;
    p_cameraComponent = p_gameplayGameMode->GetCameraComponent();
  
    p_enemyManager = new CEnemyManager();
    p_enemyManager->Init();
    p_collectibleManager = new CCollectibleManager();
    p_collectibleManager->Init();

    InitializePlatforms();
    CObject::Init();
}

void CWorldManager::InitializePlatforms()
{
    m_position = FVector2D{ CGameCostants::tileSize / 2, -CGameCostants::tileSize + CGameCostants::groundLevel };
    for (int i = 0; i < 3; i++)
    {
        GetPlatform(i == 0 ? nullptr : v_p_inUsePlatforms[i - 1]);
    }
}

void CWorldManager::DeInit()
{
    for (int i = static_cast<int>(v_p_inUsePlatforms.size()) - 1; i >= 0; i--)
    {
        v_p_inUsePlatforms[i]->DeInit();
    }
    v_p_inUsePlatforms.clear();

    p_collectibleManager->DeInit();
    p_enemyManager->DeInit();

    CObject::DeInit();
}

void CWorldManager::GetPlatform(CObject* oldPlatform)
{
    int height = CRandom::GetRandomIntInRange(1, CGameCostants::max_platform_height);
    int width = CRandom::GetRandomIntInRange(CGameCostants::min_platform_width, CGameCostants::max_platform_width);

	if(oldPlatform)
    {
        CTilesComponent* p_tilesComponent = oldPlatform->GetFirstComponentOfClass<CTilesComponent>();
        if (p_tilesComponent == nullptr) return;

        int oldHeight = p_tilesComponent->GetPlatformHeight();
        if(std::abs(height - oldHeight) > CGameCostants::max_platform_height_difference)
        {
            height = height > oldHeight ? height - CGameCostants::max_platform_height_difference : oldHeight - CGameCostants::max_platform_height_difference;
        }
        else if(height == p_tilesComponent->GetPlatformHeight() && height != 1.f)
        {
            height < 2 ? height++ : height--;
        }
    }

    CObject* platform = CFloorPlatform::CreatePlatform(m_position, oldPlatform != nullptr ? height : 1, width);
    platform->Init();

    CTilesComponent* p_tilesComponent = platform->GetFirstComponentOfClass<CTilesComponent>();
    if (p_tilesComponent == nullptr) return;

    if(oldPlatform != nullptr)
    {
        PlaceRandomObjectsOnPlatform(platform);
        p_enemyManager->PlaceEnemiesOnPlatform(m_position, p_tilesComponent->GetPlatformTilesTotalWidth(), p_tilesComponent->GetPlatformTilesTotalHeight());
        CreateUpperPlatforms(platform);
    }

    v_p_inUsePlatforms.push_front(platform);
    m_position = m_position + FVector2D{ p_tilesComponent->GetPlatformTilesTotalWidth() + CGameCostants::platform_distance, 0.f};
}

void CWorldManager::ReturnPlatform(std::deque<CObject*>& inUsePlatforms)
{
    CObject* platform = inUsePlatforms.back();
    platform->DeInit();
    inUsePlatforms.pop_back();
}

void CWorldManager::CreateUpperPlatforms(CObject* floorPlatform)
{
    CTilesComponent* p_tilesComponent = floorPlatform->GetFirstComponentOfClass<CTilesComponent>();
    if (p_tilesComponent == nullptr) return;

    int availableSpace = p_tilesComponent->GetPlatformWidth();

    std::vector<FUpperPlatformBuildData> builds;
    int allPlatformWidth = 0;

    for (int i = 0; i < CGameCostants::max_upper_platform_number; i++)
    {
        FUpperPlatformBuildData buildData;

        if (i < CGameCostants::max_upper_platform_number - 1)
        {
            buildData.width = CRandom::GetRandomIntInRange(CGameCostants::min_upper_platform_width, availableSpace - (CGameCostants::max_upper_platform_number - 1 - i) * CGameCostants::min_upper_platform_width);
        }
        else
        {
            buildData.width = availableSpace;
        }

        if (buildData.width > CGameCostants::max_upper_platform_width)
        {
            buildData.width = CGameCostants::max_upper_platform_width;
        }

        buildData.beforeSpace = CRandom::GetRandomIntInRange(CGameCostants::min_space_between_upper_platforms, CGameCostants::max_space_between_upper_platforms);
        buildData.totalSpace = buildData.width + buildData.beforeSpace;
        availableSpace -= buildData.totalSpace;
        builds.push_back(buildData);
    }

    CTransformComponent* p_transformComponent = floorPlatform->GetFirstComponentOfClass<CTransformComponent>();
    if (p_transformComponent == nullptr) return;

    FVector2D location = FVector2D{ p_transformComponent->GetLocation().x, p_transformComponent->GetLocation().y + p_tilesComponent->GetPlatformTilesTotalHeight()};

    for (int i = 0; i < builds.size(); i++)
    {
        location.x += builds[i].beforeSpace * CGameCostants::tileSize;
        float yVariation = CRandom::GetRandomBool() ? CGameCostants::upper_platform_height_gap : CGameCostants::upper_platform_height_gap * 1.5f;

        CObject* upperPlatform = CUpperPlatform::CreateUpperPlatform(FVector2D{ location.x, location.y + yVariation }, 1, builds[i].width);
        upperPlatform->Init();
        v_p_inUseUpperPlatforms.push_front(upperPlatform);
        PlaceRandomObjectsOnUpperPlatform(upperPlatform);
        location.x += builds[i].width * CGameCostants::tileSize;
    }
}

void CWorldManager::Update(float deltaTime)
{
    UpdateFloorPlatform(deltaTime);
    UpdateUpperPlatform(deltaTime);

    p_collectibleManager->Update(deltaTime);
    p_enemyManager->Update(deltaTime);
}

void CWorldManager::UpdateFloorPlatform(float deltaTime)
{
    if (p_firstFloorPlatform == nullptr)
    {
        p_firstFloorPlatform = v_p_inUsePlatforms.back();
        p_transformableFloorPlatform = p_firstFloorPlatform->GetFirstComponentOfClass<CTransformComponent>();
        p_tilesFloorPlatform = p_firstFloorPlatform->GetFirstComponentOfClass<CTilesComponent>();
    }
    
    if (p_firstFloorPlatform && p_transformableFloorPlatform && p_tilesFloorPlatform)
    {
        if (!p_cameraComponent->IsInCameraViewFromAppSide(p_transformableFloorPlatform->GetLocation(), p_tilesFloorPlatform->GetPlatformTilesTotalWidth(), p_tilesFloorPlatform->GetPlatformTilesTotalHeight()))
        {
            p_firstFloorPlatform = nullptr;
            GetPlatform(v_p_inUsePlatforms.back());
            ReturnPlatform(v_p_inUsePlatforms);
        }
    }
}

void CWorldManager::UpdateUpperPlatform(float deltaTime)
{
    if (p_firstUpperPlatform == nullptr)
    {
        p_firstUpperPlatform = v_p_inUseUpperPlatforms.back();
        p_transformableUpperPlatform = p_firstUpperPlatform->GetFirstComponentOfClass<CTransformComponent>();
        p_tilesUpperPlatform = p_firstUpperPlatform->GetFirstComponentOfClass<CTilesComponent>();
    }

    if (p_firstUpperPlatform && p_transformableUpperPlatform && p_tilesUpperPlatform)
    {
        if (!p_cameraComponent->IsInCameraViewFromAppSide(p_transformableUpperPlatform->GetLocation(), p_tilesUpperPlatform->GetPlatformTilesTotalWidth(), p_tilesUpperPlatform->GetPlatformTilesTotalHeight()))
        {
            ReturnPlatform(v_p_inUseUpperPlatforms);
            p_firstUpperPlatform = nullptr;
        }
    }
}

void CWorldManager::Render()
{
    for(int i = 0; i < static_cast<int>(v_p_inUsePlatforms.size()); i++)
    {
        v_p_inUsePlatforms[i]->Render();
    }

    for (int i = 0; i < static_cast<int>(v_p_inUseUpperPlatforms.size()); i++)
    {
        v_p_inUseUpperPlatforms[i]->Render();
    }

    p_collectibleManager->Render();
    p_enemyManager->Render();
}

float CWorldManager::GetInitialGroundLevel()
{
	if(!v_p_inUsePlatforms.empty())
	{
        CObject* platform = v_p_inUsePlatforms.back();

        CTransformComponent* p_transformComponent = platform->GetFirstComponentOfClass<CTransformComponent>();
        if (p_transformComponent == nullptr) return 0;

        CTilesComponent* p_tilesComponent = platform->GetFirstComponentOfClass<CTilesComponent>();
        if (p_tilesComponent == nullptr) return 0;

        return p_transformComponent->GetLocation().y + p_tilesComponent->GetPlatformTilesTotalHeight() + CGameCostants::tileSize * 2;
	}

    return 0;
}

void CWorldManager::PlaceRandomObjectsOnPlatform(CObject* platform)
{
    CTilesComponent* p_tilesComponent = platform->GetFirstComponentOfClass<CTilesComponent>();
    if (p_tilesComponent == nullptr) return;

    CTransformComponent* p_transformComponent = platform->GetFirstComponentOfClass<CTransformComponent>();
    if (p_transformComponent == nullptr) return;
    FVector2D platformLocation = p_transformComponent->GetLocation();

    float availableWidth = platformLocation.x + p_tilesComponent->GetPlatformTilesTotalWidth();

    float initialOffset = CRandom::GetRandomFloatInRange(CGameCostants::tileSize, CGameCostants::tileSize * (p_tilesComponent->GetPlatformWidth() / 4));
    FVector2D position = FVector2D{ platformLocation.x + initialOffset, platformLocation.y + p_tilesComponent->GetPlatformTilesTotalHeight() };

    int objectsNum = CRandom::GetRandomIntInRange(CGameCostants::min_object_per_floor, CGameCostants::max_object_per_floor);
    for (int i = 0; i < objectsNum && position.x <= availableWidth; i++)
    {
        float pattern = CRandom::GetRandomFloatInRange(0.f, 1.f);
        if (pattern <= 0.5f)
        {
            p_collectibleManager->PoolRandomObject(position);
            position = position + FVector2D{ CRandom::GetRandomFloatInRange(CGameCostants::tileSize * 3, CGameCostants::tileSize * 6), 0 };
        }
        else if (pattern > 0.5f)
        {
            p_collectibleManager->PoolRandomObject(position);
            position = position + FVector2D{ CRandom::GetRandomFloatInRange(CGameCostants::tileSize * 3, CGameCostants::tileSize * 10), 0 };
        }
    }
}

void CWorldManager::PlaceRandomObjectsOnUpperPlatform(CObject* platform)
{
    CTilesComponent* p_tilesComponent = platform->GetFirstComponentOfClass<CTilesComponent>();
    if (p_tilesComponent == nullptr) return;

    CTransformComponent* p_transformComponent = platform->GetFirstComponentOfClass<CTransformComponent>();
    if (p_transformComponent == nullptr) return;
    FVector2D platformLocation = p_transformComponent->GetLocation();

    float initialOffset = CRandom::GetRandomFloatInRange(0, CGameCostants::tileSize * 3);
    FVector2D position = FVector2D{ platformLocation.x + initialOffset, platformLocation.y + CGameCostants::tileSize / 2 };
    float availableWidth = platformLocation.x + p_tilesComponent->GetPlatformTilesTotalWidth();
    int objectsNum = CRandom::GetRandomIntInRange(1, CGameCostants::max_object_per_upper);

    for (int i = 0; i < objectsNum && position.x <= availableWidth; i++)
    {
        float pattern = CRandom::GetRandomFloatInRange(0.f, 1.f);
        if (pattern <= 0.5f)
        {
            p_collectibleManager->PoolRandomObject(position);
            position = position + FVector2D{ CRandom::GetRandomFloatInRange(CGameCostants::tileSize, CGameCostants::tileSize * 2), 0 };
        }
        else if (pattern > 0.5f)
        {
            p_collectibleManager->PoolRandomObject(position);
            position = position + FVector2D{ CRandom::GetRandomFloatInRange(CGameCostants::tileSize, CGameCostants::tileSize * 4), 0 };
        }
    }
}