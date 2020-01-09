#include "extern/raylib/raylib.h"


enum GAME_STATE{
	MENU,
	APPLICATIONS,
	OPTIONS,
	GAME,
	PAUSE,
	QUIT
};

typedef struct Button{
	char* name;
	int fontSize;
	Rectangle rec;
} Button;
void Button_draw(Button* self){
	self->rec.width = MeasureText(self->name, self->fontSize) + 10;
	self->rec.height = self->fontSize;

	Rectangle rec = { self->rec.x, self->rec.y,  self->rec.width + 10, self->rec.height };
	DrawTextRec(GetFontDefault(), self->name, rec, self->fontSize, self->fontSize / 10.0f, true, RED);
}
void Button_drawRec(Button* self){
	self->rec.width = MeasureText(self->name, self->fontSize) + 10;
	self->rec.height = self->fontSize;

	Rectangle rec = { self->rec.x, self->rec.y,  self->rec.width + 10, self->rec.height };


	Vector2 mousepoint = GetMousePosition();
	float col;
	float linewidth;
	if(CheckCollisionPointRec(mousepoint, rec)){
		col = 0.5f;
		linewidth = 3;
	} else{
		col = 0.1f;
		linewidth = 1;
	}

	DrawRectangleRec(rec, Fade(GRAY, col));
	DrawRectangleLinesEx(rec, linewidth, GRAY);
	DrawTextRec(GetFontDefault(), self->name, rec, self->fontSize, self->fontSize / 10.0f, true, RED);
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


int main(){
	const int screenWidth = 800;
	const int screenHeight = 800;


	InitWindow(screenWidth, screenHeight, "RayING");
	SetTargetFPS(60);

	enum GAME_STATE gameState = MENU;
	int running = 1;
	while(running && !WindowShouldClose()){

		switch(gameState){
		case MENU: menuState(&gameState); break;
		case APPLICATIONS: applicationSelectionState(&gameState); break;
		case OPTIONS: optionsState(&gameState); break;
		case QUIT: running = 0; break;
		}

	}
	CloseWindow();
	return 0;
}