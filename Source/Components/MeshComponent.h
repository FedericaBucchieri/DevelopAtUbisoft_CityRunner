#pragma once
#include "Component.h"
#include "AnimationData.h"
#include <string>

//
// CMeshComponent manages the rendering of sprite meshes and animations for a game object.
// It utilizes the App's sprite creation and animation features for rendering.
//

class CObject;
class CSimpleSprite;
class CTransformComponent;

class CMeshComponent : public CComponent
{
public:
    CMeshComponent(CObject* inOwner);
    CMeshComponent(CObject* inOwner, std::vector<char*> fileNames, int columns, int rows);

    virtual void Initialize() override;
    virtual void Update(float deltaTime) override;
    virtual void Render() override;

    void CreateSpriteMesh(char* fileName, int columns, int rows);
    void CreateAnimation(EAnimationID animationID, float speed, const std::vector<int>& frames);
	void PlayAnimation(EAnimationID animationID);
    EAnimationID GetCurrentAnimation() { return m_currentAnimationID; }

	void SetSpriteTransform(FTransform transform);
    FMeshBoundBox GetMeshBoundBox();
    float GetAnimationDuration(EAnimationID animationID);

private:
    CSimpleSprite* p_sprite;
    FTransform m_transform;
    std::map<EAnimationID, float> v_animations;
    EAnimationID m_currentAnimationID;
    CTransformComponent* p_transformComponent;
    bool bDebug = false;
};
