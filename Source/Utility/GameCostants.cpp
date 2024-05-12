#include "stdafx.h"
#include "GameCostants.h"

const float CGameCostants::platform_distance = 100.0f;
const float CGameCostants::feet_safety_threshold = 10.f;
const int CGameCostants::min_platform_width = 20;
const int CGameCostants::max_platform_width = 30;
const int CGameCostants::max_platform_height = 5;
const int CGameCostants::max_platform_height_difference = 3;
const int CGameCostants::max_upper_platform_number = 3;
const int CGameCostants::max_upper_platform_width = 5;
const int CGameCostants::min_upper_platform_width = 3;
const int CGameCostants::max_space_between_upper_platforms = 4;
const int CGameCostants::min_space_between_upper_platforms = 2;
const float CGameCostants::upper_platform_height_gap = 150.f;
const int CGameCostants::max_object_per_floor = 4;
const int CGameCostants::min_object_per_floor = 2;
const int CGameCostants::max_object_per_upper = 2;
const int CGameCostants::max_enemies_per_platform = 2;
char* CGameCostants::gameOverBackgroudMesh = ".\\TestData\\UI\\BgGameOver.png";
char* CGameCostants::mainMenuBackgroundMesh = ".\\TestData\\UI\\BgMainMenu.png";
char* CGameCostants::pauseBackgroundMesh = ".\\TestData\\UI\\BgPause.png";
char* CGameCostants::characterMesh = ".\\TestData\\Characters\\punk_character.png";
char* CGameCostants::coinMesh = ".\\TestData\\Objects\\Money.png";
char* CGameCostants::jumpBoostMesh = ".\\TestData\\Objects\\BoostJump.png";
char* CGameCostants::speedBoostMesh = ".\\TestData\\Objects\\BoostSpeed.png";
char* CGameCostants::healthUpMesh = ".\\TestData\\Objects\\HealthUp.png";
char* CGameCostants::barrel1Mesh = ".\\TestData\\Objects\\Barrel1.png";
char* CGameCostants::barrel2Mesh = ".\\TestData\\Objects\\Barrel2.png";
char* CGameCostants::barrel3Mesh = ".\\TestData\\Objects\\Barrel3.png";
char* CGameCostants::floorTileLeftBorder = ".\\TestData\\Tiles\\IndustrialTile_13.png";
char* CGameCostants::floorTileLeftCorner = ".\\TestData\\Tiles\\IndustrialTile_04.png";
char* CGameCostants::floorTileCenterTop = ".\\TestData\\Tiles\\IndustrialTile_05.png";
char* CGameCostants::floorTileCenterBottom = ".\\TestData\\Tiles\\IndustrialTile_14.png";
char* CGameCostants::floorTileRightCorner = ".\\TestData\\Tiles\\IndustrialTile_06.png";
char* CGameCostants::floorTileRightBorder = ".\\TestData\\Tiles\\IndustrialTile_15.png";
char* CGameCostants::upperTileLeftV1 = ".\\TestData\\Tiles\\IndustrialTile_80.png";
char* CGameCostants::upperTileCenterV1 = ".\\TestData\\Tiles\\IndustrialTile_81.png";
char* CGameCostants::upperTileRightV1 = ".\\TestData\\Tiles\\IndustrialTile_72.png";
char* CGameCostants::upperTileLeftV2 = ".\\TestData\\Tiles\\IndustrialTile_77.png";
char* CGameCostants::upperTileCenterV2 = ".\\TestData\\Tiles\\IndustrialTile_78.png";
char* CGameCostants::upperTileRightV2 = ".\\TestData\\Tiles\\IndustrialTile_79.png";
float CGameCostants::deathLevel = -50.f;
float CGameCostants::groundLevel = 200.f;
float CGameCostants::tileSize = 32.f;
float CGameCostants::backgroundLevel = 400.f;
const float CGameCostants::poolSize = 8;
const float CGameCostants::backgroundWidth = 576;
const float CGameCostants::backgroundHeight = 600;
char* CGameCostants::enemyWithBathMesh = ".\\TestData\\Characters\\enemy_bat.png";
int CGameCostants::objectMeshColums = 6;
int CGameCostants::boostMeshColums = 8;
char* CGameCostants::gameplayGameMode = "gameplay";
char* CGameCostants::mainMenuGameMode = "mainMenu";
char* CGameCostants::gameoverGameMode = "gameOver";
char* CGameCostants::goBackToMainMenuText = "Press ENTER to go back to Main Menu";
char* CGameCostants::startText = "Press ENTER to start";
char* CGameCostants::HUDbackground = ".\\TestData\\UI\\HUDBackground.png";
char* CGameCostants::UIMoneySprite = ".\\TestData\\UI\\UIMoney.png";
char* CGameCostants::UIInvulnerabilitySprite = ".\\TestData\\UI\\UIInvulnerability.png";
char* CGameCostants::UISpeedBoostSprite = ".\\TestData\\UI\\UISpeedBoost.png";
char* CGameCostants::UIJumpBoostSprite = ".\\TestData\\UI\\UIJumpBoost.png";
char* CGameCostants::UIHealthSprite = ".\\TestData\\UI\\UIHealth.png";
const float CGameCostants::characterFeetHeight = 10;