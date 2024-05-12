#include "stdafx.h"
#include "BackgroundPool.h"
#include <string>
#include "GameMode.h"
#include "SimpleSprite.h"
#include "CameraComponent.h"
#include "GameInstance.h"

CBackgroundPool::CBackgroundPool() :
    p_cameraComponent(nullptr),
    m_position(FVector2D{0.f, 0.f})
{
}

void CBackgroundPool::OnBeginDestroy()
{
    for (auto sprite : v_p_availableSprites)
    {
        delete sprite;
    }
    v_p_availableSprites.clear();

    for (auto sprite : v_p_inUseSprites)
    {
        delete sprite;
    }
    v_p_inUseSprites.clear();
    CObject::OnBeginDestroy();
}

void CBackgroundPool::OnBeginPlay()
{

    CGameMode* p_gameMode = CGameInstance::GetInstance()->GetCurrentGameMode();
    if (p_gameMode == nullptr) return;
    p_cameraComponent = p_gameMode->GetCameraComponent();

    for (int i = 1; i < CGameCostants::poolSize; i++)
    {
        std::string fileName = ".\\TestData\\Background\\Frame" + std::to_string(i) + ".png";
        CSimpleSprite* sprite = new CSimpleSprite(fileName.c_str());
        v_p_availableSprites.push_back(sprite);
    }

    FCameraData data = p_cameraComponent->GetCameraData();
    m_position = FVector2D{ data.left - APP_VIRTUAL_WIDTH, CGameCostants::backgroundLevel };

    for (int i = 0; i < 3; i++)
    {
        GetSprite();
    }

    CObject::OnBeginPlay();
}

void CBackgroundPool::Update(float deltaTime)
{
    for (int i = static_cast<int>(v_p_inUseSprites.size()) - 1; i >= 0; i--)
    {
        CSimpleSprite* sprite = v_p_inUseSprites[i];
        sprite->Update(deltaTime);

        FVector2D spriteLocation;
        sprite->GetPosition(spriteLocation.x, spriteLocation.y);
        spriteLocation = spriteLocation - FVector2D{ CGameCostants::backgroundWidth / 2, CGameCostants::backgroundHeight / 2 };


        if (!p_cameraComponent->IsInCameraViewFromAppSide(spriteLocation, CGameCostants::backgroundWidth, CGameCostants::backgroundHeight))
        {
            GetSprite();
            ReturnSprite(sprite);
        }
    }
}

void CBackgroundPool::Render()
{
    for (int i = 0; i < v_p_inUseSprites.size(); i++)
    {
        v_p_inUseSprites[i]->Draw();
    }
}

CSimpleSprite* CBackgroundPool::GetSprite()
{
    if (v_p_availableSprites.empty()) {
        return nullptr;
    }

    CSimpleSprite* sprite = v_p_availableSprites.back();
    v_p_availableSprites.pop_back();
    v_p_inUseSprites.push_front(sprite);

    sprite->SetPosition(m_position.x, m_position.y);
    m_position = m_position + FVector2D{ sprite->GetWidth(), 0 };

    return sprite;
}

void CBackgroundPool::ReturnSprite(CSimpleSprite* sprite)
{
    v_p_inUseSprites.pop_back();
    v_p_availableSprites.push_back(sprite);
}
