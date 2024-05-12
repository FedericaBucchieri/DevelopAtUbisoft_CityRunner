#pragma once
#include "Object.h"
#include <queue>

//
// CBackgroundPool manages a pool of background sprites for efficient rendering in the game.
// The pool dynamically creates and recycles background sprites based on camera visibility.
//


class CCameraComponent;

class CBackgroundPool : public CObject
{
public:
	CBackgroundPool();

	void OnBeginPlay() override;
	void OnBeginDestroy() override;
	void Update(float deltaTime) override;
	void Render() override;

	CSimpleSprite* GetSprite();
	void ReturnSprite(CSimpleSprite* sprite);

private:
	std::vector<CSimpleSprite*> v_p_availableSprites;
	std::deque<CSimpleSprite*> v_p_inUseSprites;
	CCameraComponent* p_cameraComponent;
	FVector2D m_position;
};

