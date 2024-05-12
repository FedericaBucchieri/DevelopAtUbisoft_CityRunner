#pragma once
#include "stdafx.h"
#include "Component.h"

//
// CTilesComponent handles the rendering and setup of platform tiles based on variations and configurations.
// It creates a set of sprites to represent the platform and manages their positions.
//

class CSimpleSprite;
class CObject;

class CTilesComponent : public CComponent
{
public:
	CTilesComponent(CObject* owner, int inPlatformHeight, int inPlatformWidth, std::vector<std::vector<char*>> variations, bool isUpperPlatform);
	void OnBeginPlay() override;
	void OnBeginDestroy() override;
	void Render() override;

	int GetPlatformHeight() { return m_platformHeight; }
	int GetPlatformWidth() { return m_platformWidth; }
	float GetPlatformTilesTotalWidth() { return CGameCostants::tileSize * m_platformWidth; }
	float GetPlatformTilesTotalHeight() { return CGameCostants::tileSize * m_platformHeight; }

protected:
	void SetupTiles(const char* leftBorderMesh, const char* leftCornerMesh, const char* centerTopMesh, const char* centerBottomMesh, const char* rightCornerMesh, const char* rightBorderMesh);
	void SetupUpperTiles(const char* leftMesh, const char* centerMesh, const char* rightMesh);
	
	bool b_isUpperPlatform;
	std::vector<CSimpleSprite*> v_p_platformTiles;
	std::vector<std::vector<char*>> v_platformMeshVariations;
	int m_platformWidth;
	int m_platformHeight;
	FVector2D m_platformLocation;
};

