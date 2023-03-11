#include <iostream>
#include "raylib.h"
#include "verlet.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1920;
    const int screenHeight = 1080;
    float pRadius = 20.0f;
    int numParticles = 0;
    bool menuOpen = false;
    bool inMenu = false;
    bool inInfo = false;
    bool rMouse = false;

    InitWindow(screenWidth, screenHeight, "2D Verlet Sim");

    SetTargetFPS(75);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    ParticleSystem particleSystem(pRadius);
    Rectangle menuBox = { 10,10,300,150 };
    Rectangle infoBox = { GetScreenWidth() - 380,10,370,150 };

    while (!WindowShouldClose())
    {
        Color rColor = getRainbow();

        BeginDrawing();

        ClearBackground(BLACK);
        
        if (!rMouse)
        {
            //Add Gravity to all particles
            particleSystem.addVelocity({ 0.0f, 9.8f }, GetFrameTime());
        }

        particleSystem.update(GetFrameTime());
        particleSystem.draw();

        DrawRectangleRec(infoBox, { 70,70,70,80 });
        inInfo = CheckCollisionPointRec(GetMousePosition(), infoBox);

        DrawText("Press", GetScreenWidth() - 370, 20, 20, WHITE);
        DrawText("F1", GetScreenWidth() - 300, 20, 20, RED);
        DrawText("to open the debug menu.", GetScreenWidth() - 270, 20, 20, WHITE);

        DrawText("Left click", GetScreenWidth() - 370, 50, 20, RED);
        DrawText("to spawn objects.", GetScreenWidth() - 265, 50, 20, WHITE);

        DrawText("Hold", GetScreenWidth() - 370, 80, 20, WHITE);
        DrawText("Right click", GetScreenWidth() - 317, 80, 20, RED);
        DrawText("to disable gravity.", GetScreenWidth() - 205, 80, 20, WHITE);

        DrawText("Press", GetScreenWidth() - 370, 110, 20, WHITE);
        DrawText("Middle Mouse Button", GetScreenWidth() - 300, 110, 20, RED);
        DrawText("to delete an object.", GetScreenWidth() - 370, 130, 20, WHITE);

        if (numParticles < 100)
        {
            numParticles++;
            particleSystem.addParticle({ pRadius + 50, (float)GetScreenHeight() / 5 }, pRadius, rColor);
            particleSystem.addVelocity({ 10.0f, 1.0f }, GetFrameTime());
        }

        if (IsKeyPressed(KEY_F1))
        {
            menuOpen = !menuOpen;
            inMenu = false;
        }

        if (menuOpen)
        {
            char fTime[7];
            snprintf(fTime, sizeof(fTime), "%f", GetFrameTime());
            char numObj[5];
            snprintf(numObj, sizeof(numObj), "%i", numParticles);
            char pRad[6];
            snprintf(pRad, sizeof(pRad), "%f", pRadius);

            DrawRectangleRec(menuBox, { 70,70,70,80 });
            inMenu = CheckCollisionPointRec(GetMousePosition(), menuBox);
            DrawText("Made by FleeTime", GetScreenWidth() / 2 - 150, 20, 20, rColor);

            DrawText("FPS:", 20, 20, 20, WHITE);
            DrawFPS(70, 20);
            DrawText("Frametime:", 20, 50, 20, WHITE);
            DrawText(fTime, 130, 50, 20, GREEN);
            DrawText("Number of objects:", 20, 80, 20, WHITE);
            DrawText(numObj, 215, 80, 20, YELLOW);
            DrawText("Object Radius:", 20, 110, 20, WHITE);
            pRadius = GuiSliderBar({ 20, 130, 150, 20 }, NULL, NULL, pRadius, 0.1f, 100.0f);
            DrawText(pRad, 180, 130, 20, WHITE);
        }

        if (!inMenu && !inInfo)
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                numParticles++;
                particleSystem.addParticle(GetMousePosition(), pRadius, rColor);
            }

            if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
            {
                rMouse = true;
            }

            if (IsMouseButtonUp(MOUSE_BUTTON_RIGHT))
            {
                rMouse = false;
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE))
            {
                particleSystem.deleteParticle();
            }
        }
        else
        {
            rMouse = false;
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}