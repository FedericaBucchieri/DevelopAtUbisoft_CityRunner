#include "stdafx.h"
#include "TilesComponent.h"
#include "Random.h"
#include "Object.h"
#include "TransformComponent.h"

CTilesComponent::CTilesComponent(CObject* owner, int inPlatformHeight, int inPlatformWidth, std::vector<std::vector<char*>> variations, bool isUpperPlatform) :
	CComponent(owner),
	m_platformHeight(inPlatformHeight),
	m_platformWidth(inPlatformWidth),
	b_isUpperPlatform(isUpperPlatform)
{
	for (int i = 0; i < static_cast<int>(variations.size()); i++)
	{
		v_platformMeshVariations.push_back(variations[i]);
	}
}


void CTilesComponent::OnBeginPlay()
{
	CComponent::OnBeginPlay();

	CTransformComponent* p_transformComponent = p_owner->GetFirstComponentOfClass<CTransformComponent>();
	if (p_transformComponent == nullptr) return;
	m_platformLocation = p_transformComponent->GetLocation();

	int index = CRandom::GetRandomIntInRange(0, static_cast<int>(v_platformMeshVariations.size()) - 1);
	std::vector<char*> variation = v_platformMeshVariations[index];
	char* leftBorderMesh = variation[0];
	char* rightCornerMesh = variation[1];
	char* centerTopMesh = variation[2];

	if (b_isUpperPlatform)
	{
		SetupUpperTiles(leftBorderMesh, centerTopMesh, rightCornerMesh);
	}
	else 
	{
		char* leftCornerMesh = variation[3];
		char* rightBorderMesh = variation[4];
		char* centerBottomMesh = variation[5];
		SetupTiles(leftBorderMesh, leftCornerMesh, centerTopMesh, centerBottomMesh, rightCornerMesh, rightBorderMesh);
	}
}

void CTilesComponent::OnBeginDestroy()
{
	for (int i = static_cast<int>(v_p_platformTiles.size()) - 1; i >= 0; i--)
	{
		delete v_p_platformTiles[i];
	}
	v_p_platformTiles.clear();

	CComponent::OnBeginDestroy();
}

void CTilesComponent::Render()
{
	CComponent::Render();

	for (CSimpleSprite* tile : v_p_platformTiles)
	{
		tile->Draw();
	}
}

void CTilesComponent::SetupTiles(const char* leftBorderMesh, const char* leftCornerMesh, const char* centerTopMesh, const char* centerBottomMesh, const char* rightCornerMesh, const char* rightBorderMesh)
{
	for (int j = 0; j < static_cast<int>(m_platformHeight); j++)
	{
		for (int i = 0; i < static_cast<int>(m_platformWidth); i++)
		{
			CSimpleSprite* tile = nullptr;

			if (i == 0 && j != m_platformHeight - 1)
			{
				tile = new CSimpleSprite(leftBorderMesh);
			}
			else if (i > 0 && i < m_platformWidth - 1 && j != m_platformHeight - 1)
			{
				tile = new CSimpleSprite(centerBottomMesh);
			}
			else if (i == m_platformWidth - 1 && j != m_platformHeight - 1)
			{
				tile = new CSimpleSprite(rightBorderMesh);
			}
			else if (i == 0 && j == m_platformHeight - 1)
			{
				tile = new CSimpleSprite(leftCornerMesh);
			}
			else if (i == m_platformWidth - 1 && j == m_platformHeight - 1)
			{
				tile = new CSimpleSprite(rightCornerMesh);
			}
			else if (i > 0 && j == m_platformHeight - 1)
			{
				tile = new CSimpleSprite(centerTopMesh);
			}

			if (tile)
			{
				tile->SetPosition(m_platformLocation.x + i * CGameCostants::tileSize + CGameCostants::tileSize / 2, m_platformLocation.y + j * CGameCostants::tileSize + CGameCostants::tileSize / 2);
				v_p_platformTiles.push_back(tile);
			}
		}
	}
}


void CTilesComponent::SetupUpperTiles(const char* leftMesh, const char* centerMesh, const char* rightMesh)
{
	for (int i = 0; i < m_platformWidth; i++)
	{
		CSimpleSprite* tile = nullptr;

		if (i == 0)
		{
			tile = new CSimpleSprite(leftMesh);
		}
		else if (i > 0 && i < m_platformWidth - 1)
		{
			tile = new CSimpleSprite(centerMesh);
		}
		else if (i == m_platformWidth - 1)
		{
			tile = new CSimpleSprite(rightMesh);
		}

		if (tile)
		{
			tile->SetPosition(m_platformLocation.x + i * CGameCostants::tileSize + CGameCostants::tileSize / 2, m_platformLocation.y + CGameCostants::tileSize / 4);
			v_p_platformTiles.push_back(tile);
		}
	}
}