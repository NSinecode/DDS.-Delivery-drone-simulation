#include <string>

#include "raylib.h"
#include "Drone.h"
#include "LandScape.h"

#define DEBUG

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = GetMonitorWidth(GetCurrentMonitor());
    const int screenHeight = GetMonitorHeight(GetCurrentMonitor());

    InitWindow(screenWidth, screenHeight, "DDS");

    SetWindowPosition(0, 30);

    // Define our custom camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = { 18.0f, 21.0f, 18.0f };     // Camera position
    camera.target = { 0.0f, 0.0f, 0.0f };          // Camera looking at point
    camera.up = { 0.0f, 1.0f, 0.0f };              // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                    // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;                 // Camera projection type

    Drone main;

    LandScape city;

    city.AutoGenBuildings(BUILDINGS_COUNT);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    HideCursor();

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera, CAMERA_THIRD_PERSON);
        camera.target = main.getForvard().position;
        SetMousePosition(GetScreenWidth() / 2, GetScreenHeight() / 2);

        main.UpdatePos();

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                //DrawModel(model, mapPosition, 1.0f, RED);
                
                //Draw drone
                main.Draw();
                //Draw landscape
                city.Draw();

                DrawGrid(20, 1.0f);
                DrawLine3D({ -10,0,0 }, { 10,0,0 }, PURPLE);
                DrawLine3D({ 0,-10,0 }, { 0,10,0 }, GREEN);

            EndMode3D();

            DrawText(std::to_string((int)(GetTime() - main.getPowerTimer())).c_str(), GetScreenWidth()/2 - MeasureText(std::to_string((int)(GetTime() - main.getPowerTimer())).c_str(), 40)/2, 90, 40, DARKGRAY);

            DrawFPS(10, 30);

#ifdef DEBUG
                DrawText(std::to_string(main.getForvard().position.y).c_str(), 10, 60, 20, GREEN);
                DrawText(std::to_string(main.getAx().y).c_str(), 10, 90, 20, GREEN);
                DrawText(std::to_string(main.getVel().y).c_str(), 10, 120, 20, GREEN);

                DrawText(std::to_string(main.getRotation().fi).c_str(), 10, 160, 20, GREEN);
                //DrawText(std::to_string(main.getForvard().direction.z).c_str(), 10, 190, 20, GREEN);
#endif // DEBUG

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    CloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}