#include "App.h"
#include "Button.h"
#include "App_Snake/App_Snake.h"
#include "App_Memory/App_Memory.h"
#include "App_TowerBuilder/TowerBuilder.h"



enum GAME_STATE{
	MENU,
	APP_SELECTION,
	OPTIONS,
	QUIT
};


void App_init(App* self){
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(self->windowSize.x, self->windowSize.y, self->name);
	SetTargetFPS(60);


	self->r.x = self->windowSize.x / 1920.0;
	self->r.y = self->windowSize.y / 1080.0;
}
void App_run(App* self){
	self->gameState = MENU;
	self->running = 1;
	while(self->running && !WindowShouldClose()){
		BeginDrawing();
		ClearBackground(RAYWHITE);
		switch(self->gameState){
		case MENU:			menuState(self); break;
		case APP_SELECTION:	applicationSelectionState(self); break;
		case OPTIONS:		optionsState(self); break;
		case QUIT:			self->running = 0; break;
		}
		EndDrawing();

	}
	CloseWindow();
	return 0;
}
void menuState(App* self){
	Vector2 r = self->r;
	int fontSize = 75;
	int widthOffset = 50;

	Button titleButton = { "RayING", fontSize * r.y, {widthOffset * r.x, 00, MeasureText("RayING", fontSize), fontSize} };
	Button_draw(&titleButton);

	int widthOffset2 = 70;
	Vector2 mousepoint = GetMousePosition();
	Button buttons[3];
	char* menuNames[] = { "App Selection", "Options", "Quit" };
	for(int i = 0; i < 3; i++){
		buttons[i] = (Button){ menuNames[i], fontSize * r.y, {widthOffset2 * r.x, fontSize * r.y + (fontSize + 10) * r.y * i, MeasureText(menuNames[i], fontSize), fontSize}, r };
		Button_drawRec(&buttons[i]);
		if(CheckCollisionPointRec(mousepoint, buttons[i].rec)){
			if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
				switch(i){
				case 0: self->gameState = APP_SELECTION; break;
				case 1: self->gameState = OPTIONS; break;
				case 2: self->gameState = QUIT; break;
				}
			}
		}
	}
}
void applicationSelectionState(App* self){
	Vector2 r = self->r;
	int fontSize = 75;
	int widthOffset = 50;

	Button titleButton = { "Applications", fontSize * r.y, {widthOffset * r.x, 0, -1, -1} };
	Button_draw(&titleButton);

	int widthOffset2 = 70;
	Vector2 mousepoint = GetMousePosition();
	enum{ numApp = 4 };
	Button buttons[numApp];
	char* menuNames[] = { "Memory", "Snake", "Tower Builder", "Back", "Snake2" };


	for(int i = 0; i < numApp; i++){
		buttons[i] = (Button){ menuNames[i], fontSize * r.y, {widthOffset2 * r.x, fontSize * r.y + (fontSize + 10) * r.y * i, MeasureText(menuNames[i], fontSize), fontSize}, r };
		Button_drawRec(&buttons[i]);
		if(CheckCollisionPointRec(mousepoint, buttons[i].rec)){
			if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
				buttons[i].isClicked = true;
			}
			if(buttons[i].isClicked == true){
				switch(i){
				case 0: buttons[i].isClicked = false;  init_memoryGame_main(self); break;
				case 1: buttons[i].isClicked = false;  initSnakeGame2(self); break;
				case 2:  buttons[i].isClicked = false;  initTowerBuilder(self);  break;
				case 3: self->gameState = MENU; break;
				}
			}
		}
	}
}
void optionsState(App* self){
	Vector2 r = self->r;
	int fontSize = 75;
	int widthOffset = 50;

	Button titleButton = { "Options", fontSize * r.y, {widthOffset * r.x, 0, -1, -1} };
	Button_draw(&titleButton);

	int widthOffset2 = 70;
	Vector2 mousepoint = GetMousePosition();
	Button backButton = { "Back", fontSize * r.y, {widthOffset2 * r.x, (fontSize + 10) * r.x, MeasureText("Back", fontSize), fontSize}, r };
	Button_drawRec(&backButton);
	if(CheckCollisionPointRec(mousepoint, backButton.rec)){
		if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
			self->gameState = MENU;
		}
	}
}