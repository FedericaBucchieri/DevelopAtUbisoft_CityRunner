#pragma once
#include "stdafx.h"

struct FCameraData
{
    float screenWidth = 0.0f;
    float screenHeight = 0.0f;
    float left = 0.0f;
    float right = 0.0f;
    float top = 0.0f;
    float bottom = 0.0f;

    static void UpdateCameraData(FVector2D newPosition, FCameraData& data, float zoom)
    {
        data.screenWidth = APP_VIRTUAL_WIDTH / zoom;
        data.screenHeight = APP_VIRTUAL_HEIGHT / zoom;
        data.left = newPosition.x - (data.screenWidth / 2);
        data.right = newPosition.x + (data.screenWidth / 2);
        data.top = newPosition.y + (data.screenHeight / 2);
        data.bottom = newPosition.y - (data.screenHeight / 2);
    }
};
