#include "App_Snake.h"
#include "raylib/raylib.h"
#include "../Button.h"
#include "Board.h"
#include "Snake.h"
#include "Food.h"

enum GAME_STATE{
	PLAY,
	PAUSE,
	GAMEOVER,
	GAMEOVER_BY_WALL,
	GAMEOVER_BY_SUICIDE,
	QUIT
};
typedef struct SnakeGame{
	int running;
	enum GAME_STATE gameState;
	App* appPtr;

	Snake snake;
	Food food;
	Board wall;
	int framesCounter;
} SnakeGame;
void SnakeGame_init(SnakeGame* self);
void SnakeGame_update(SnakeGame* self);
void SnakeGame_draw(SnakeGame* self);
void SnakeGame_updateDrawFrame(SnakeGame* self);

void Menu_drawGameoverMenu(SnakeGame* self){
	int textPosX = self->appPtr->windowSize.x / 2 - MeasureText("GAME OVER", 40) / 2;
	int textPosY = self->appPtr->windowSize.y / 2 - 40;

	DrawText("GAME OVER", textPosX, textPosY, 40, GRAY);
	int fontSize = 40;

	Button repeatButton = { "Repeat", fontSize, {textPosX - 150, textPosX + 200, 1, 1} };
	Button_drawRec(&repeatButton);

	Button quitButton = { "Quit", fontSize, {textPosX + 150, textPosX + 200, 1, 1} };
	Button_drawRec(&quitButton);

	Vector2 mousepoint = GetMousePosition();
	if(CheckCollisionPointRec(mousepoint, repeatButton.rec)){
		if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
			SnakeGame_init(self);
		}
	}
	if(CheckCollisionPointRec(mousepoint, quitButton.rec)){
		if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
			self->gameState = QUIT;
		}
	}
}
void Menu_drawOptionsMenu(SnakeGame* self){
	DrawText("GAME PAUSED", self->appPtr->windowSize.x / 2 - MeasureText("GAME PAUSED", 40) / 2, self->appPtr->windowSize.y / 2 - 40, 40, GRAY);

}
void SnakeGame_init(SnakeGame* self){
	self->gameState = PLAY;
	self->framesCounter = 0;

	SNAKE_HEAD_COLOR = DARKBLUE;
	SNAKE_TAIL_COLOR = VIOLET;
	FOOD_COLOR = SKYBLUE;
	GRID_COLOR = LIGHTGRAY;

	int bX = SQUARE_SIZE;
	int bY = SQUARE_SIZE;

	Board_init(&self->wall, 15, 15, bX * 2, bY * 4);
	Snake_init(&self->snake, &self->wall);
	Food_init(&self->food, &self->wall);
}
void SnakeGame_update(SnakeGame* self){
	if(!(self->gameState == QUIT)){
		if(!(self->gameState == GAMEOVER)){

			if(IsKeyPressed(KEY_SPACE)){
				if(self->gameState == PLAY){
					self->gameState = PAUSE;
				} else{
					self->gameState = PLAY;
				}
			}

			if(!(self->gameState == PAUSE)){

				Snake_control(&self->snake);
				Snake_move(&self->snake, self->framesCounter++);
				if(Snake_isOutsideWall(&self->snake, &self->wall)){
					self->gameState = GAMEOVER;
				}
				if(Snake_detectSelf(&self->snake, &self->wall)){
					self->gameState = GAMEOVER;
				}
				Food_calculatePosition(&self->food, &self->wall, &self->snake);
				Snake_detectFood(&self->snake, &self->food);

			} else if(self->gameState == PAUSE){

			}
			self->framesCounter++;

		}
	} else{
		self->running = false;
	}
}
void SnakeGame_draw(SnakeGame* self){
	BeginDrawing();
	ClearBackground(RAYWHITE);
	DrawText(FormatText("Count: %i", self->snake.segmentCount), 280, 130, 40, MAROON);
	Board_drawGrid(&self->wall);
	Food_draw(&self->food, &self->wall);
	Snake_draw(&self->snake, &self->wall);
	if(self->gameState == PAUSE){
		Menu_drawOptionsMenu(self);
	}
	if(self->gameState == GAMEOVER){
		Menu_drawGameoverMenu(self);
	}
	EndDrawing();
}
void SnakeGame_updateDrawFrame(SnakeGame* self){
	SnakeGame_update(self);
	SnakeGame_draw(self);
}



void initSnakeGame(App* self){
	SnakeGame snakeGame;
	snakeGame.appPtr = self;
	SnakeGame_init(&snakeGame);
	while(snakeGame.running && !WindowShouldClose()){
		SnakeGame_updateDrawFrame(&snakeGame);
	}
}