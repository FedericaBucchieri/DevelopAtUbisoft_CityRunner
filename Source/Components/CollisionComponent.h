#pragma once
#include "stdafx.h"
#include "Component.h"

//
// CCollisionComponent grants the possibility to game objects to have a bounding box and collide with other objects.
// It provides an object a collision profile and a box estension.
//

class CTransformComponent;

class CCollisionComponent : public CComponent
{
public:
	CCollisionComponent(CObject* inOwner, ECollisionProfile profile);

	void RegisterComponent();
	void UnRegisterComponent();

	void Initialize() override;
	void DeInitialize() override;
	void Activate() override;
	void Deactivate() override;

	void Update(float deltaTime) override;
	void Render() override;

	bool GetIsDirty() { return b_dirty; }

	void SetCollisionProfile(ECollisionProfile Profile) { m_collisionProfile = Profile; }
	ECollisionProfile GetCollisionProfile() const { return m_collisionProfile; }

	void SetBoundingBox(FVector2D location, float width, float height);
	void SetBoundingBox(FMeshBoundBox MeshBoundBox) { m_boundingBox = MeshBoundBox;  }
	FMeshBoundBox GetBoundingBox() const { return m_boundingBox; }

	void SetBoxExtension(float width, float height);
	bool SetIsColliding(FCollisionData collisionData, CCollisionComponent* otherCollider);
	bool StopColliding(CCollisionComponent* otherCollider);
	bool CheckCollisionWith(ECollisionProfile profile);
	void RemoveCollisionWith(ECollisionProfile profile);

	void SetColliderOffset(FVector2D inOffset) { m_offset = inOffset; }
	FVector2D GetColliderOffeset() { return m_offset; }
	std::vector<CObject*> GetAllCollidingObjectsOfProfile(ECollisionProfile profile);

protected:
	FMeshBoundBox m_boundingBox;
	ECollisionProfile m_collisionProfile;
	CTransformComponent* p_transformComponent;
	std::map<CCollisionComponent*, ECollisionProfile> v_p_collidingWith;
	FVector2D m_lastUpdatePosition;
	FVector2D m_offset;
	bool b_dirty;
	bool bDebug = false;
};

