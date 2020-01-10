#pragma once
#include "raylib/raylib.h"
enum GAME_STATE;
typedef struct App{
	char* name;
	Vector2 windowSize;
	//int width;
	//int height;

	enum GAME_STATE gameState;

} App;
void App_init(App* self);
void App_run(App* self);
void menuState(enum GAME_STATE* gameState);
void applicationSelectionState(enum GAME_STATE* gameState);
void optionsState(enum GAME_STATE* gameState);