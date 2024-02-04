#include <string>

#include "raylib.h"
#include "Drone.h"
#include "LandScape.h"

//#define DEBUG

//size is frontSize
void DrawHUD(Drone other, int size);

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

    //Add drone
    Drone main;

    //Randomly generate buildings
    LandScape city;
    city.AutoGenBuildings(BUILDINGS_COUNT);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    //For camera movement
    HideCursor();

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera, CAMERA_THIRD_PERSON);
        //Set drone target
        camera.target = main.getForvard().position;
        camera.position = Vector3Add(camera.position, Vector3Scale(main.getVel(), GetFrameTime()));

        SetMousePosition(GetScreenWidth() / 2, GetScreenHeight() / 2);

        //Update drone
        main.UpdatePos();
        //Update power
        if ((int)(GetTime() - main.getPowerTimer()) > POWERTIME)    main.Kill();
        //Update collision
        city.CheckCollision(main);

        //Debug regenerate
#ifdef DEBUG
        if (IsKeyPressed(KEY_R))    city.ReAutoGenBuildings(BUILDINGS_COUNT);
        if (IsKeyPressed(KEY_T))    city.ChangeTarget();
#endif // DEBUG

        
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

#ifdef DEBUG
                DrawGrid(20, 1.0f);
                DrawLine3D({ -10,0,0 }, { 10,0,0 }, PURPLE);
                DrawLine3D({ 0,-10,0 }, { 0,10,0 }, GREEN);
#endif // DEBUG

            EndMode3D();

            DrawHUD(main, 20);

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

void DrawHUD(Drone other, int size)
{
    //Draw FPS
    DrawFPS(10, 30);
    
    int width = MeasureText("Power remaining: 100", size) + size;
    DrawRectangle(GetScreenWidth() - width, 0, width, 4.5 * size, { 130, 130, 130, 220 });

    //Drawing text
    DrawText(("Score: " + std::to_string(other.getScore())).c_str(), GetScreenWidth() - width + size / 2, 1.5 * size, size, GREEN);
    DrawText(("Power remaining: " + std::to_string(POWERTIME - (int)(GetTime() - other.getPowerTimer()))).c_str(), GetScreenWidth() - width + size / 2, 3 * size, size, GREEN);
}
