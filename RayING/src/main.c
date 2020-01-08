#include "extern/raylib/raylib.h"

int main(){
	const int screenWidth = 800;
	const int screenHeight = 800;

	InitWindow(screenWidth, screenHeight, "RayING");

	SetTargetFPS(60);

	// Main game loop
	while(!WindowShouldClose()){

		BeginDrawing();

		ClearBackground(RAYWHITE);

		DrawText("RayING", 50, 10, 50, RED);

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}