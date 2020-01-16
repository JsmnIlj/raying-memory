/*  Memory Matching Game "RayING Memory"
    A game made with the raylib library by Ray: https://raysan5.itch.io/
    Author: Jasmin Iljuschin
*/

#include "extern/raylib/raylib.h"

int main(void)
{
    // Initialisation
    //---------------------------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow  (screenWidth, screenHeight, "RayING Memory"); // Displays the window with a given title.

    SetTargetFPS(60);              // Now our game runs at 60 fps (frames-per-second).
    //---------------------------------------------------------------------------------------------------------

    //Main game loop starts here
    while (!WindowShouldClose())   // Detects if the window close button Escape key have been pressed.
    {
        // Update
        //-----------------------------------------------------------------------------------------------------
        // TODO: Update variables here
        //-----------------------------------------------------------------------------------------------------

        // Draw
        //-----------------------------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("Main Menu", 190, 200, 20, BLUE);  // Displays a text in the given position and color
    
        EndDrawing();
        //-----------------------------------------------------------------------------------------------------
    }

    // De-Initialization
    // --------------------------------------------------------------------------------------------------------
    CloseWindow();                 // Closes the window
    //---------------------------------------------------------------------------------------------------------

    return 0;
}

