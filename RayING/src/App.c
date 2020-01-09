#include "App.h"
#include "Button.h"
#include "raylib/raylib.h"
void App_init(App* self){
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(self->width, self->height, self->name);
	SetTargetFPS(60);
}
void App_run(App* self){
	enum GAME_STATE gameState = MENU;
	int running = 1;
	while(running && !WindowShouldClose()){

		switch(gameState){
		case MENU:			menuState(&gameState); break;
		case APPLICATIONS:	applicationSelectionState(&gameState); break;
		case OPTIONS:		optionsState(&gameState); break;
		case QUIT:			running = 0; break;
		}

	}
	CloseWindow();
	return 0;
}
void menuState(enum GAME_STATE* gameState){
	BeginDrawing();
	ClearBackground(RAYWHITE);
	int fontSize = 60;

	Button titleButton = { "RayING", fontSize, {50, 10, 1, 1} };
	Button_draw(&titleButton);

	Button applicationsButton = { "Applications", fontSize, {50, 70, 1, 1} };
	Button_drawRec(&applicationsButton);

	Button optionsButton = { "Options", fontSize, {50, 130, 1, 1} };
	Button_drawRec(&optionsButton);

	Button quitButton = { "Quit", fontSize, {50, 190, 1, 1} };
	Button_drawRec(&quitButton);


	EndDrawing();

	Vector2 mousepoint = GetMousePosition();
	if(CheckCollisionPointRec(mousepoint, applicationsButton.rec)){
		if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
			*gameState = APPLICATIONS;
		}
	}
	if(CheckCollisionPointRec(mousepoint, optionsButton.rec)){
		if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
			*gameState = OPTIONS;
		}
	}
	if(CheckCollisionPointRec(mousepoint, quitButton.rec)){
		if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
			*gameState = QUIT;
		}
	}
}
void applicationSelectionState(enum GAME_STATE* gameState){
	BeginDrawing();
	ClearBackground(RAYWHITE);

	int fontSize = 60;


	Button titleButton = { "Applications", fontSize, {50, 10, 1, 1} };
	Button_draw(&titleButton);

	Button memoryButton = { "Memory", fontSize, {50, 70, 1, 1} };
	Button_drawRec(&memoryButton);

	Button snakeButton = { "Snake", fontSize, {50, 130, 1, 1} };
	Button_drawRec(&snakeButton);

	Button towerButton = { "Tower Builder", fontSize, {50, 190, 1, 1} };
	Button_drawRec(&towerButton);

	Button backButton = { "Back", fontSize, {50, 250, 1, 1} };
	Button_drawRec(&backButton);

	EndDrawing();

	Vector2 mousepoint = GetMousePosition();
	if(CheckCollisionPointRec(mousepoint, backButton.rec)){
		if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
			*gameState = MENU;
		}
	}
}
void optionsState(enum GAME_STATE* gameState){
	BeginDrawing();
	ClearBackground(RAYWHITE);

	int fontSize = 60;
	Button titleButton = { "Options", fontSize, {50, 10, 1, 1} };
	Button_draw(&titleButton);

	Button backButton = { "Back", fontSize, {50, 60, 1, 1} };
	Button_drawRec(&backButton);

	EndDrawing();

	Vector2 mousepoint = GetMousePosition();
	if(CheckCollisionPointRec(mousepoint, backButton.rec)){
		if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
			*gameState = MENU;
		}
	}
}