#pragma once
enum GAME_STATE{
	MENU,
	APPLICATIONS,
	OPTIONS,
	QUIT
};
typedef struct App{
	char* name;
	int width;
	int height;
	enum GAME_STATE gameState;
} App;
void App_init(App* self);
void App_run(App* self);
void menuState(enum GAME_STATE* gameState);
void applicationSelectionState(enum GAME_STATE* gameState);
void optionsState(enum GAME_STATE* gameState);