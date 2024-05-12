#include "stdafx.h"
#include "CollisionManager.h"
#include "CollisionComponent.h"
#include "MovementComponent.h"
#include "GameEventManager.h"
#include "GameMode.h"
#include "GameInstance.h"
#include "GameplayGameMode.h"
#include "CollisionEvents.h"

void CCollisionManager::Init()
{
    FCollisionPreset CharacterPreset;
    CharacterPreset.collisionProfile = CHARACTER;
    CharacterPreset.collisionResponseMap.insert(std::make_pair(CHARACTER, BLOCK));
    CharacterPreset.collisionResponseMap.insert(std::make_pair(ENEMY, OVERLAP));
    CharacterPreset.collisionResponseMap.insert(std::make_pair(OBSTACLE, OVERLAP));
    CharacterPreset.collisionResponseMap.insert(std::make_pair(COLLECTIBLE, OVERLAP));
    CharacterPreset.collisionResponseMap.insert(std::make_pair(FLOOR, BLOCK));
    CharacterPreset.collisionResponseMap.insert(std::make_pair(PERCEPTION, OVERLAP));
    m_collisionPresets.push_back(CharacterPreset);

    FCollisionPreset EnemyPreset;
    EnemyPreset.collisionProfile = ENEMY;
    EnemyPreset.collisionResponseMap.insert(std::make_pair(CHARACTER, OVERLAP));
    EnemyPreset.collisionResponseMap.insert(std::make_pair(ENEMY, OVERLAP));
    EnemyPreset.collisionResponseMap.insert(std::make_pair(OBSTACLE, OVERLAP));
    EnemyPreset.collisionResponseMap.insert(std::make_pair(COLLECTIBLE, OVERLAP));
    EnemyPreset.collisionResponseMap.insert(std::make_pair(FLOOR, BLOCK));
    m_collisionPresets.push_back(EnemyPreset);

    FCollisionPreset ObstaclePreset;
    ObstaclePreset.collisionProfile = OBSTACLE;
    ObstaclePreset.collisionResponseMap.insert(std::make_pair(CHARACTER, OVERLAP));
    ObstaclePreset.collisionResponseMap.insert(std::make_pair(ENEMY, BLOCK));
    ObstaclePreset.collisionResponseMap.insert(std::make_pair(OBSTACLE, BLOCK));
    ObstaclePreset.collisionResponseMap.insert(std::make_pair(COLLECTIBLE, BLOCK));
    ObstaclePreset.collisionResponseMap.insert(std::make_pair(FLOOR, BLOCK));
    m_collisionPresets.push_back(ObstaclePreset);

    FCollisionPreset CollectiblePreset;
    CollectiblePreset.collisionProfile = COLLECTIBLE;
    CollectiblePreset.collisionResponseMap.insert(std::make_pair(CHARACTER, OVERLAP));
    CollectiblePreset.collisionResponseMap.insert(std::make_pair(ENEMY, OVERLAP));
    CollectiblePreset.collisionResponseMap.insert(std::make_pair(OBSTACLE, BLOCK));
    CollectiblePreset.collisionResponseMap.insert(std::make_pair(COLLECTIBLE, BLOCK));
    m_collisionPresets.push_back(CollectiblePreset);

    FCollisionPreset FloorPreset;
    FloorPreset.collisionProfile = FLOOR;
    FloorPreset.collisionResponseMap.insert(std::make_pair(CHARACTER, BLOCK));
    FloorPreset.collisionResponseMap.insert(std::make_pair(CHARACTER_FEET, OVERLAP));
    FloorPreset.collisionResponseMap.insert(std::make_pair(ENEMY, BLOCK));
    FloorPreset.collisionResponseMap.insert(std::make_pair(OBSTACLE, BLOCK));
    m_collisionPresets.push_back(FloorPreset);

    FCollisionPreset CharacterFeetPreset;
    CharacterFeetPreset.collisionProfile = CHARACTER_FEET;
    CharacterFeetPreset.collisionResponseMap.insert(std::make_pair(FLOOR, OVERLAP));
    m_collisionPresets.push_back(CharacterFeetPreset);

    FCollisionPreset PerceptionPreset;
    PerceptionPreset.collisionProfile = PERCEPTION;
    PerceptionPreset.collisionResponseMap.insert(std::make_pair(CHARACTER, OVERLAP));
    m_collisionPresets.push_back(PerceptionPreset);

    p_eventManager = CGameInstance::GetGameEventManager();

    CObject::Init();
}

void CCollisionManager::DeInit()
{
    m_collisionPresets.clear();
    CObject::DeInit();
}

void CCollisionManager::RegisterCollisionComponent(CCollisionComponent* component)
{
    if (std::find(v_p_components.begin(), v_p_components.end(), component) == v_p_components.end()) 
    {
        v_p_components.push_back(component);
    }
}

void CCollisionManager::UnRegisterCollisionComponent(CCollisionComponent* component)
{
    auto it = std::find(v_p_components.begin(), v_p_components.end(), component);

    if (it != v_p_components.end()) {
        v_p_components.erase(it);
    }
    
}


void CCollisionManager::Update(float deltaTime)
{
    for (int i = 0; i < v_p_components.size(); ++i)
    {
        for (int j = i + 1; j < v_p_components.size(); ++j)
        {
            CCollisionComponent* componentA = v_p_components[i];
            CCollisionComponent* componentB = v_p_components[j];

            if (componentA->GetIsDirty() || componentB->GetIsDirty())
            {
                CheckCollision(componentA, componentB, deltaTime);
            }
        }
    }
}

void CCollisionManager::CheckCollision(CCollisionComponent* componentA, CCollisionComponent* componentB, float deltaTime)
{
    FMeshBoundBox boxA = componentA->GetBoundingBox();
    FMeshBoundBox boxB = componentB->GetBoundingBox();
 
    bool overlapX = FMeshBoundBox::GetOverlapX(boxA, boxB);
    bool overlapY = FMeshBoundBox::GetOverlapY(boxA, boxB);

    FCollisionData collisionData;
    if (overlapX && overlapY) 
    {
	    collisionData.collisionResponse = FindCollisionResponse(componentA, componentB);
	    collisionData.collisionComponentA = componentA;
	    collisionData.collisionComponentB = componentB;
	    collisionData.objectA = componentA->GetOwner();
	    collisionData.objectB = componentB->GetOwner();
	    collisionData.contactPoints = CalculateContactPoints(boxA, boxB);
	    collisionData.normal = CalculateCollisionNormal(boxA, boxB);
	    collisionData.overlapDepth = CalculateOverlapDepth(boxA, boxB);
        HandleStartCollision(collisionData, deltaTime);
    }
    else
    {
        HandleEndCollision(componentA, componentB);
    }
}

ECollisionResponse CCollisionManager::FindCollisionResponse(CCollisionComponent* componentA, CCollisionComponent* componentB)
{
    ECollisionProfile profileA = componentA->GetCollisionProfile();
    ECollisionProfile profileB = componentB->GetCollisionProfile();

    ECollisionResponse collisionResponse = ECollisionResponse::NONE;

    for (const FCollisionPreset& preset : m_collisionPresets) {
        if (preset.collisionProfile == profileA) {

            auto responseIt = preset.collisionResponseMap.find(profileB);
            if (responseIt != preset.collisionResponseMap.end()) {
                collisionResponse = responseIt->second;
                break;
            }
        }
    }

    return collisionResponse;
}

std::vector<FVector2D> CCollisionManager::CalculateContactPoints(const FMeshBoundBox boxA, const FMeshBoundBox boxB)
{
	std::vector<FVector2D> contactPoints;

    float leftA = boxA.x;
    float rightA = boxA.x + boxA.width;
    float topA = boxA.y + boxA.height;
    float bottomA = boxA.y;

    float leftB = boxB.x;
    float rightB = boxB.x + boxB.width;
    float topB = boxB.y + boxB.height;
    float bottomB = boxB.y;

    if (rightA >= leftB && leftA <= rightB) 
    {
        if (topA >= bottomB && bottomA <= topB) 
        {
            contactPoints.push_back(FVector2D{ max(leftA, leftB), max(bottomA, bottomB) });
            contactPoints.push_back(FVector2D{ min(rightA, rightB), min(topA, topB) });
        }
    }

    return contactPoints;
}

FVector2D CCollisionManager::CalculateCollisionNormal(const FMeshBoundBox boxA, const FMeshBoundBox boxB)
{
    FVector2D centerA = { boxA.x + boxA.width / 2, boxA.y + boxA.height / 2 };
    FVector2D centerB = { boxB.x + boxB.width / 2, boxB.y + boxB.height / 2 };

    FVector2D normal = { centerA.x - centerB.x, centerA.y - centerB.y };
    float length = sqrtf(normal.x * normal.x + normal.y * normal.y); 
    if (length > 0)
    {
        normal.x /= length;
        normal.y /= length;
    }

    return normal;
}

float CCollisionManager::CalculateOverlapDepth(const FMeshBoundBox boxA, const FMeshBoundBox boxB)
{
    float overlapX = min(boxA.x + boxA.width, boxB.x + boxB.width) - max(boxA.x, boxB.x);
    float overlapY = min(boxA.y + boxA.height, boxB.y + boxB.height) - max(boxA.y, boxB.y);

    return min(overlapX, overlapY);
}

FVector2D CCollisionManager::CalculateCollisionImpulse(const FCollisionData& collisionData)
{
    FVector2D impulse = { 0.0f, 0.0f };

    if (collisionData.overlapDepth > 0.0f) 
    {
        FVector2D collisionNormal = collisionData.normal;

        impulse.x = collisionNormal.x * collisionData.overlapDepth;
        impulse.y = collisionNormal.y * collisionData.overlapDepth;
    }

    return impulse;
}

void CCollisionManager::HandleStartCollision(FCollisionData collisionData, float deltaTime)
{
    if (!collisionData.objectA || !collisionData.objectB) return;

    CCollisionComponent* componentA = collisionData.collisionComponentA;
    CCollisionComponent* componentB = collisionData.collisionComponentB;
	bool mustUpdateA = componentA->SetIsColliding(collisionData, componentB);
    bool mustUpdateB = componentB->SetIsColliding(collisionData, componentA);

    switch (collisionData.collisionResponse)
    {
		case ECollisionResponse::BLOCK:
            BlockOnCollision(collisionData, deltaTime);
            break;

    	case ECollisionResponse::OVERLAP:
            if (p_eventManager)
            {
                if(mustUpdateA)
                {
                    p_eventManager->Publish<FComponentBeginOverlapEvent>(componentA->GetUniqueID(), new FComponentBeginOverlapEvent{ componentB, collisionData.objectB });
                }
			    if(mustUpdateB)
			    {
                    p_eventManager->Publish<FComponentBeginOverlapEvent>(componentB->GetUniqueID(), new FComponentBeginOverlapEvent{ componentA, collisionData.objectA });
                }
            }
            break;

		default:
			break;
    }
}

void CCollisionManager::HandleEndCollision(CCollisionComponent* componentA, CCollisionComponent* componentB)
{
    bool mustUpdateA = componentA->StopColliding(componentB);
    bool mustUpdateB = componentB->StopColliding(componentA);

    switch (FindCollisionResponse(componentA, componentB))
    {
    case ECollisionResponse::OVERLAP:
        if (p_eventManager)
        {
            if(mustUpdateA)
            {
                p_eventManager->Publish<FComponentEndOverlapEvent>(componentA->GetUniqueID(), new FComponentEndOverlapEvent{ componentB,  componentB->GetOwner() });
            }
            if(mustUpdateB)
            {
                p_eventManager->Publish<FComponentEndOverlapEvent>(componentB->GetUniqueID(), new FComponentEndOverlapEvent{ componentA,  componentA->GetOwner() });
            }
        }
        break;

    default:
        break;
    }
}

void CCollisionManager::BlockOnCollision(FCollisionData collisionData, float deltaTime)
{
    if (collisionData.overlapDepth > 0.0f)
    {
        CObject* objectA = collisionData.objectA;
        CObject* objectB = collisionData.objectB;
        FVector2D point1 = collisionData.contactPoints[0];
        FVector2D point2 = collisionData.contactPoints[1];

        float xDifference = std::abs(point1.x - point2.x);
        float yDifference = std::abs(point1.y - point2.y);
        float threshold = 10.f;

        if(yDifference < threshold)
        {
            CMovementComponent* p_movementComponentA = objectA->GetFirstComponentOfClass<CMovementComponent>();
            if (p_movementComponentA != nullptr)
            {
                p_movementComponentA->HandleCollisionMovementOnYAxis(deltaTime, collisionData.normal, yDifference);
            }

            CMovementComponent* p_movementComponentB = objectB->GetFirstComponentOfClass<CMovementComponent>();
            if (p_movementComponentB != nullptr)
            {
                p_movementComponentB->HandleCollisionMovementOnYAxis(deltaTime, collisionData.normal * -1, yDifference);
            }
        }
        else if(xDifference < threshold)
        {
            CMovementComponent* p_movementComponentA = objectA->GetFirstComponentOfClass<CMovementComponent>();
            if (p_movementComponentA != nullptr)
            {
                p_movementComponentA->HandleCollisionMovementOnXAxis(deltaTime);
            }

            CMovementComponent* p_movementComponentB = objectB->GetFirstComponentOfClass<CMovementComponent>();
            if (p_movementComponentB != nullptr)
            {
                p_movementComponentB->HandleCollisionMovementOnXAxis(deltaTime);
            }
        }
    }
}
