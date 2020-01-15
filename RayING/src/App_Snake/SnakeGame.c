#include "SnakeGame.h"
#include "Menu.h"

void SnakeGame_init(SnakeGame* self){
	self->gameState = PLAY;
	self->framesCounter = 0;

	self->windowSize = self->appPtr->windowSize;
	SQUARE_SIZE = 40;
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

	ClearBackground(RAYWHITE);

	int fontSize = self->windowSize.x /22;

	DrawText("SNAKE", self->windowSize.x / 2 - MeasureText("SNAKE", fontSize)/2, self->windowSize.y/ 2 / 2 / 2, fontSize, MAROON);

	DrawText(FormatText("Count: %i", self->snake.segmentCount), 280, 130, fontSize/2, MAROON);
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