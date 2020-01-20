#include "App_Snake.h"
#include "raylib/raylib.h"
#include "../Button.h"
#include "Board.h"
#include "Snake.h"
#include "Food.h"

enum GAME_STATE{
	MAINMENU,
	PLAY,
	PAUSE,
	GAMEOVER,
	QUIT
};
typedef struct SnakeGame{
	int running;
	enum GAME_STATE gameState;
	App* appPtr;
	Vector2 windowSize;
	Vector2 ratio;
	int framesCounter;

	Board board;
	Snake snake;
	Food food;
} SnakeGame;

void SnakeGame_init(SnakeGame* self){

	SetTargetFPS(60);
	self->running = true;
	self->gameState = MAINMENU;
	self->framesCounter = 0;


	self->windowSize = self->appPtr->windowSize;
	self->ratio = self->appPtr->r;
	SQUARE_SIZE = self->windowSize.y / 20;
	Board_init(&self->board, 15, 15, 10, 10);
	Snake_init(&self->snake, &self->board);
	Food_init(&self->food, &self->board);
}
void drawCurrentPlayState(SnakeGame* self){
	Vector2 r = self->ratio;
	DrawText("SNAKE", self->windowSize.x / 2 * r.x, 50 * r.y, 75.0 * r.y, RED);
	DrawText(FormatText("Count: % i", self->snake.segmentCount), self->windowSize.x / 2 * r.x + 350 * r.x, 50 * r.y, 75.0 * r.y, RED);

	Board_drawGrid(&self->board, self->windowSize);
	Food_draw(&self->food, &self->board);
	Snake_draw(&self->snake, &self->board);
}

void State_GameOver(SnakeGame* self);
void State_Pause_Option(SnakeGame* self);
void State_Pause(SnakeGame* self);
void State_Play(SnakeGame* self);
void State_MainMenu(SnakeGame* self);

void Button_init(Button* self, char* name, float recX, float recY, float recW, float recH, float fontSize, Vector2 ratio){
	self->name = name;

	self->rec.x = recX;
	self->rec.y = recY;
	self->rec.width = recW;
	self->rec.height = recH;

	self->fontSize = fontSize;
	self->ratio = ratio;
	self->isClicked = false;
}

void State_GameOver(SnakeGame* self){
	Vector2 r = self->ratio;
	float fontSize = 75.0;
	float widthOffset = 50.0;

	enum N{ numButton = 2 };
	Button buttons[numButton];
	{
		char* menuNames[numButton] = { "Repeat", "Quit" };
		float widthOffset2 = 70;
		for(int i = 0; i < numButton; i++){
			Button_init(&buttons[i],
						menuNames[i],
						widthOffset * r.x,
						fontSize * r.y + (fontSize + 10) * r.y * i,
						(float)MeasureText(menuNames[i], fontSize) * r.x,
						fontSize * r.y,
						fontSize * r.y,
						r
			);
		}
	}


	bool gameoverState = true;
	while(gameoverState && !WindowShouldClose()){
		//Update
		{
			for(int i = 0; i < numButton; i++){
				Vector2 mousepoint = GetMousePosition();
				if(CheckCollisionPointRec(mousepoint, buttons[i].rec)){
					if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
						buttons[i].isClicked = true;
					}
				}
				if(buttons[i].isClicked == true){
					buttons[i].isClicked = false;
					switch(i){
					case 0: SnakeGame_init(self); return;  break;
					case 1: self->gameState = QUIT; return; break;
					}
				}
			}
		}
		//Draw
		{
			BeginDrawing();
			ClearBackground(RAYWHITE);

			drawCurrentPlayState(self);

			DrawText("State_GameOver", self->windowSize.x / 2 * r.x, 400 * r.y, 75.0 * r.y, RED);

			for(int i = 0; i < numButton; i++){
				Button_drawRec(&buttons[i]);
			}
			EndDrawing();
		}
	}
}
void State_Pause_Option(SnakeGame* self){
	Vector2 r = self->ratio;
	float fontSize = 75.0;
	float widthOffset = 50.0;

	enum N{ numButton = 4 };
	Button buttons[numButton];
	{
		char* menuNames[numButton] = { "Snake Head Color", "Snake Head Color", "Food Color", "Back" };
		float widthOffset2 = 70;
		for(int i = 0; i < numButton; i++){
			Button_init(&buttons[i],
						menuNames[i],
						widthOffset * r.x,
						fontSize * r.y + (fontSize + 10) * r.y * i,
						(float)MeasureText(menuNames[i], fontSize) * r.x,
						fontSize * r.y,
						fontSize * r.y,
						r
			);
		}
	}

	typedef struct Circle{
		Vector2 position;
		float radius;
		Color color;
	} Circle;
	enum {numCircle = 3};
	Circle headColorcircle[numCircle];
	Color headColors[3] = { MAROON , DARKBLUE, DARKGREEN };
	for(int i = 0; i < numCircle; i++){
		headColorcircle[i].radius = (fontSize * r.y) / 2;
		headColorcircle[i].position.x = headColorcircle[i].radius + widthOffset * r.x + (float)MeasureText(buttons[0].name, fontSize) * r.x;
		headColorcircle[i].position.y = -headColorcircle[i].radius + fontSize * r.y + (fontSize + 10) * r.y * 0 + fontSize * r.y;
		headColorcircle[i].position.x += i * headColorcircle[i].radius * 2 + 40 *r.x;
		headColorcircle[i].color = headColors[i];
	}

	Circle tailColorcircle[numCircle];
	Color tailColors[3] = { RED , BLUE, LIME };
	for(int i = 0; i < numCircle; i++){
		tailColorcircle[i].radius = (fontSize * r.y) / 2;
		tailColorcircle[i].position.x = tailColorcircle[i].radius + widthOffset * r.x + (float)MeasureText(buttons[0].name, fontSize) * r.x;
		tailColorcircle[i].position.y = -tailColorcircle[i].radius + fontSize * r.y + (fontSize + 10) * r.y * 1 + fontSize * r.y;
		tailColorcircle[i].position.x += i * tailColorcircle[i].radius * 2 + 40 * r.x;
		tailColorcircle[i].color = tailColors[i];
	}

	Circle foodColorcircle[numCircle];
	Color foodColors[3] = { PINK , SKYBLUE, GREEN  };
	for(int i = 0; i < numCircle; i++){
		foodColorcircle[i].radius = (fontSize * r.y) / 2;
		foodColorcircle[i].position.x = foodColorcircle[i].radius + widthOffset * r.x + (float)MeasureText(buttons[0].name, fontSize) * r.x;
		foodColorcircle[i].position.y = -foodColorcircle[i].radius + fontSize * r.y + (fontSize + 10) * r.y * 2 + fontSize * r.y;
		foodColorcircle[i].position.x += i * foodColorcircle[i].radius * 2 + 40 * r.x;
		foodColorcircle[i].color = foodColors[i];
	}

	bool optionState = true;
	while(optionState && !WindowShouldClose()){
		//Update
		{
			Vector2 mousepoint = GetMousePosition();
			for(int i = 0; i < numCircle; i++){
				if(CheckCollisionPointCircle(mousepoint, headColorcircle[i].position, headColorcircle[i].radius)){
					if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
						self->snake.headColor = headColorcircle[i].color;
					}
				}
				if(CheckCollisionPointCircle(mousepoint, tailColorcircle[i].position, tailColorcircle[i].radius)){
					if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
						self->snake.tailColor = tailColorcircle[i].color;
					}
				}
				if(CheckCollisionPointCircle(mousepoint, foodColorcircle[i].position, foodColorcircle[i].radius)){
					if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
						self->food.color = foodColorcircle[i].color;
					}
				}
			}

		}
		//Draw
		{
			BeginDrawing();
			ClearBackground(RAYWHITE);

			drawCurrentPlayState(self);
			DrawRectangle(0, 0, self->windowSize.x, self->windowSize.y, Fade(GRAY, 0.5f));
			DrawText("State_Pause_Option", self->windowSize.x / 2 * r.x, 400 * r.y, 75.0 * r.y, RED);

			for(int i = 0; i < numButton; i++){
				Button_draw(&buttons[i]);
			}
			for(int i = 0; i < numCircle; i++){
				DrawCircle(headColorcircle[i].position.x, headColorcircle[i].position.y, headColorcircle[i].radius, headColorcircle[i].color);
				DrawCircle(tailColorcircle[i].position.x, tailColorcircle[i].position.y, tailColorcircle[i].radius, tailColorcircle[i].color);
				DrawCircle(foodColorcircle[i].position.x, foodColorcircle[i].position.y, foodColorcircle[i].radius, foodColorcircle[i].color);
			}
			EndDrawing();
		}
	}
}
void State_Pause(SnakeGame* self){
	Vector2 r = self->ratio;
	float fontSize = 75.0;
	float widthOffset = 50.0;

	enum N{ numButton = 3 };
	Button buttons[numButton];
	{
		char* menuNames[numButton] = { "Continue", "Options", "Quit" };
		float widthOffset2 = 70;
		for(int i = 0; i < numButton; i++){
			Button_init(&buttons[i],
						menuNames[i],
						widthOffset2 * r.x,
						fontSize * r.y + (fontSize + 10) * r.y * i,
						(float)MeasureText(menuNames[i], fontSize) * r.x,
						fontSize * r.y,
						fontSize * r.y,
						r
			);
		}
	}
	bool pauseState = true;
	while(pauseState && !WindowShouldClose()){

		//Update
		{
			for(int i = 0; i < numButton; i++){
				Vector2 mousepoint = GetMousePosition();
				if(CheckCollisionPointRec(mousepoint, buttons[i].rec)){
					if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
						buttons[i].isClicked = true;
					}
				}
				if(buttons[i].isClicked == true){
					buttons[i].isClicked = false;
					switch(i){
					case 0: pauseState = false; self->gameState = PLAY;  break;
					case 1: State_Pause_Option(self); break;
					case 2: pauseState = false; self->gameState = QUIT; break;
					}
				}
			}
		}

		//Draw
		{
			BeginDrawing();
			ClearBackground(RAYWHITE);

			drawCurrentPlayState(self);
			DrawRectangle(0, 0, self->windowSize.x, self->windowSize.y, Fade(GRAY, 0.5f));
			DrawText("State_Pause", self->windowSize.x / 2 * r.x, 400 * r.y, 75.0 * r.y, RED);

			for(int i = 0; i < numButton; i++){
				Button_drawRec(&buttons[i]);
			}

			EndDrawing();
		}
	}

}
void State_Play(SnakeGame* self){
	Vector2 r = self->ratio;
	float fontSize = 75.0;
	float widthOffset = 50.0;

	self->gameState = PLAY;
	bool playState = true;
	while(playState && !WindowShouldClose()){
		//Update
		{
			if(self->gameState != GAMEOVER && self->gameState != QUIT){

				if(IsKeyReleased(KEY_B)){
					self->gameState = MAINMENU;
					playState = false;
				}
				if(IsKeyReleased(KEY_SPACE)){
					self->gameState = PAUSE;
				}
				if(self->gameState != PAUSE){
					Snake_move(&self->snake, self->framesCounter);
					Snake_control(&self->snake);
					if(Snake_isOutsideWall(&self->snake, &self->board)) self->gameState = GAMEOVER;
					if(Snake_detectSelf(&self->snake, &self->board)) self->gameState = GAMEOVER;
					Snake_detectFood(&self->snake, &self->food);

					Food_calculatePosition(&self->food, &self->board, &self->snake);

					self->framesCounter++;
				}
				if(self->gameState == PAUSE){
					State_Pause(self);
				}

			}
			if(self->gameState == GAMEOVER){
				State_GameOver(self);
			}
			if(self->gameState == QUIT){
				return;
			}
		}


		//Draw
		{
			BeginDrawing();
			ClearBackground(RAYWHITE);
			drawCurrentPlayState(self);
			if(self->gameState == GAMEOVER){
				DrawText("GAME OVER", self->windowSize.x / 2 * r.x, 400 * r.y, 75.0 * r.y, RED);
			}

			EndDrawing();
		}
	}

}
void State_MainMenu(SnakeGame* self){
	Vector2 r = self->ratio;
	float fontSize = 75.0;

	Button titleButton;
	{
		float widthOffset = 50.0;
		Button_init(&titleButton,
					"Snake",
					widthOffset * r.x,
					0,
					MeasureText("Snake", (int)(fontSize * r.y)),
					fontSize * r.y,
					fontSize * r.y,
					r
		);
	}

	enum N{ numButton = 5 };
	Button buttons[numButton];
	{
		char* menuNames[numButton] = { "Play", "Options", "Back to Selection", "Back to Main Menu", "Quit the Game" };
		float widthOffset2 = 70;
		for(int i = 0; i < numButton; i++){
			Button_init(&buttons[i],
						menuNames[i],
						widthOffset2 * r.x,
						fontSize * r.y + (fontSize + 10) * r.y * i,
						(float)MeasureText(menuNames[i], fontSize) * r.x,
						fontSize * r.y,
						fontSize * r.y,
						r
			);
		}
	}
	while(self->running && !WindowShouldClose()){

		// Update
		{
			for(int i = 0; i < numButton; i++){
				Vector2 mousepoint = GetMousePosition();
				if(CheckCollisionPointRec(mousepoint, buttons[i].rec)){
					if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
						buttons[i].isClicked = true;
					}
				}
				if(buttons[i].isClicked == true){
					switch(i){
					case 0: State_Play(self); buttons[i].isClicked = false; break;
					case 1: buttons[i].isClicked = false;  break;
					case 2: self->running = 0; break;
					case 3: self->running = 0; self->appPtr->gameState = MAINMENU; break;
					case 4: self->running = 0; self->appPtr->running = 0; self->appPtr->gameState = QUIT; break;
					}
				}
			}
		}

		// Draw
		{
			BeginDrawing();
			ClearBackground(RAYWHITE);

			Button_draw(&titleButton);
			for(int i = 0; i < numButton; i++){
				Button_drawRec(&buttons[i]);
			}
			EndDrawing();
		}
	}
}

void initSnakeGame2(App* self){
	SnakeGame snakeGame;
	snakeGame.appPtr = self;

	SnakeGame_init(&snakeGame);
	State_MainMenu(&snakeGame);
}