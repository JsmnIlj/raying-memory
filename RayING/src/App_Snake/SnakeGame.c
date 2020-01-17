#include "SnakeGame.h"
#include "Menu.h"

void SnakeGame_init(SnakeGame* self){
	self->gameState		= PLAY;
	self->framesCounter = 0;


	self->windowSize = self->appPtr->windowSize;
	SQUARE_SIZE = 40;
	SQUARE_SIZE = self->windowSize.y/20;
	self->snake.headColor = DARKBLUE;
	self->snake.tailColor = VIOLET;
	self->food.color = SKYBLUE;
	self->wall.color = LIGHTGRAY;

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
				if(Snake_isOutsideWall(&self->snake, &self->wall)) self->gameState = GAMEOVER;
				if(Snake_detectSelf(&self->snake, &self->wall)) self->gameState = GAMEOVER;
				Snake_detectFood(&self->snake, &self->food);

				Food_calculatePosition(&self->food, &self->wall, &self->snake);

			} else if(self->gameState == PAUSE){

			}
			self->framesCounter++;

		}
	} else{
		self->running = false;
	}
}
void SnakeGame_drawUI(SnakeGame* self){
	int fontSize = self->windowSize.y / 22;
	fontSize = self->windowSize.y / 10;
	char* snakeText = "SNAKE";
	int snakeTextPosX = self->windowSize.x / 2 - MeasureText(snakeText, fontSize) / 2;
	int snakeTextPoxY = self->windowSize.y / 50;
	DrawText("SNAKE", snakeTextPosX, snakeTextPoxY, fontSize, MAROON);

	int countTextPosX = self->windowSize.x / 4 * 3;
	int countTextPosY = self->windowSize.y / 50 * 1 + fontSize / 2;
	DrawText(FormatText("Count: %i", self->snake.segmentCount), countTextPosX, countTextPosY, fontSize / 2, MAROON);
}
void SnakeGame_draw(SnakeGame* self){

	ClearBackground(RAYWHITE);

	SnakeGame_drawUI(self);
	Board_drawGrid(&self->wall, self->windowSize);
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