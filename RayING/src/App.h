#pragma once
#include "raylib/raylib.h"
enum GAME_STATE;
typedef struct App{
	char* name;
	Vector2 windowSize;
	Vector2 r;
	enum GAME_STATE gameState;
	bool running;
} App;
void App_init(App* self);
void App_run(App* self);
void menuState(enum GAME_STATE* gameState);
void applicationSelectionState(enum GAME_STATE* gameState);
void optionsState(enum GAME_STATE* gameState);