#include "raylib.h"

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 600;
    const int circleRadius = 50;

    // Remove to run as fast as you can.
    SetConfigFlags(FLAG_VSYNC_HINT);
    SetTargetFPS(60);

    InitWindow(screenWidth, screenHeight, "Sample Raylib test");

    const auto floatRadius = static_cast<float>(circleRadius);
    const auto diameter = floatRadius * 2.f;

    const Rectangle widthBoundingBox{ diameter,
                                      0.f,
                                      static_cast<float>(GetScreenWidth()) - diameter * 2.f,
                                      static_cast<float>(GetScreenHeight()) };
    const Rectangle heightBoundingBox{ 0.,
                                       diameter,
                                       static_cast<float>(GetScreenWidth()),
                                       static_cast<float>(GetScreenHeight()) - diameter * 2.f };

    Vector2 ballPosition = { (float) screenWidth / 2, (float) screenHeight / 2 };
    Vector2 ballSpeed = { 1000., 500. };

    while (!WindowShouldClose())
    {
        int fps = GetFPS();
        float frameTime = GetFrameTime();

        Vector2 futurePosition{ ballPosition.x + ballSpeed.x * frameTime,
                                ballPosition.y + ballSpeed.y * frameTime };

        Rectangle boundingBox{ futurePosition.x - floatRadius, futurePosition.y - floatRadius,
                               diameter, diameter };

        auto oldSpeed = ballSpeed;

        if (!CheckCollisionRecs(boundingBox, widthBoundingBox))
        {
            ballSpeed.x = -ballSpeed.x;
        }

        if (!CheckCollisionRecs(boundingBox, heightBoundingBox))
        {
            ballSpeed.y = -ballSpeed.y;
        }

        if (oldSpeed.x == ballSpeed.x && oldSpeed.y == ballSpeed.y)
        {
            ballPosition = futurePosition;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        const auto* fpsStr = TextFormat("%i - %f", fps, frameTime * 1000.f);
        DrawText(fpsStr, 10, 10, 20, DARKGRAY);

        DrawCircleV(ballPosition, circleRadius, DARKGREEN);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}



