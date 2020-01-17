#include "App.h"
#include "Button.h"
#include "App_Snake/App_Snake.h"
#include "App_Memory/App_Memory.h"

enum GAME_STATE{
	MENU,
	APPLICATIONS,
	OPTIONS,
	QUIT
};
void App_init(App* self){
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(self->windowSize.x, self->windowSize.y, self->name);
	SetTargetFPS(60);
}
void App_run(App* self){
	self->gameState = MENU;
	int running = 1;
	while(running && !WindowShouldClose()){

		switch(self->gameState){
		case MENU:			menuState(self); break;
		case APPLICATIONS:	applicationSelectionState(self); break;
		case OPTIONS:		optionsState(self); break;
		case QUIT:			running = 0; break;
		}

	}
	CloseWindow();
	return 0;
}
void menuState(App* self){
	BeginDrawing();
	ClearBackground(RAYWHITE);


	float windowRatio = 7.0f / 100.0f;
	int fontSize = self->windowSize.y * windowRatio;
	Button titleButton = { "RayING", fontSize, {50, 00, 1, 1} };
	Button_draw(&titleButton);


	Vector2 mousepoint = GetMousePosition();
	Button buttons[3];
	char* menuNames[] = { "Applications", "Options", "Quit" };
	for(int i = 0; i < 3; i++){
		buttons[i] = (Button){ menuNames[i], fontSize, {50, 70 + 60 * i, 1, 1} };
		Button_drawRec(&buttons[i]);
		if(CheckCollisionPointRec(mousepoint, buttons[i].rec)){
			switch(i){
			case 0: if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) self->gameState = APPLICATIONS; break;
			case 1: if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) self->gameState = OPTIONS; break;
			case 2: if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) self->gameState = QUIT; break;
			}
		}
	}
	EndDrawing();
}
void applicationSelectionState(App* self){
	BeginDrawing();
	ClearBackground(RAYWHITE);

	float windowRatio = 7.0f / 100.0f;
	int fontSize = self->windowSize.y * windowRatio;

	Button titleButton = { "Applications", fontSize, {50, 10, 1, 1} };
	Button_draw(&titleButton);


	Vector2 mousepoint = GetMousePosition();
	Button buttons[4];
	char* menuNames[] = { "Memory", "Snake", "Tower Builder", "Back" };
	for(int i = 0; i < 4; i++){
		buttons[i] = (Button){ menuNames[i], fontSize, {50, 70 + 60 * i, 1, 1} };
		Button_drawRec(&buttons[i]);
		if(CheckCollisionPointRec(mousepoint, buttons[i].rec)){
			switch(i){
			case 0: if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) initMemoryGame(self); break;
			case 1: if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) initSnakeGame(self); break;
			case 2: if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) break;
			case 3: if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) self->gameState = MENU; break;
			}
		}
	}
	EndDrawing();
}
void optionsState(App* self){
	BeginDrawing();
	ClearBackground(RAYWHITE);

	float windowRatio = 7.0f / 100.0f;
	int fontSize = self->windowSize.y * windowRatio;

	Button titleButton = { "Options", fontSize, {50, 10, 1, 1} };
	Button_draw(&titleButton);

	Vector2 mousepoint = GetMousePosition();
	Button backButton = { "Back", fontSize, {50, 60, 1, 1} };
	Button_drawRec(&backButton);
	if(CheckCollisionPointRec(mousepoint, backButton.rec)){
		if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
			self->gameState = MENU;
		}
	}
	EndDrawing();
}