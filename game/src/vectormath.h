#pragma once
#include "raymath.h"

static Color getRainbow()
{
    float time = GetTime();
    float hue = time * 220.0f;  // full cycle speed
    Color color = ColorFromHSV(hue, 0.8f, 0.9f);
    return color;
}

RMAPI Vector2 Vector2MultiplyValue(Vector2 v1, float v2)
{
    Vector2 result = { v1.x * v2, v1.y * v2 };

    return result;
}

RMAPI Vector2 Vector2DivideValue(Vector2 v1, float v2)
{
    Vector2 result = { v1.x / v2, v1.y / v2 };

    return result;
}