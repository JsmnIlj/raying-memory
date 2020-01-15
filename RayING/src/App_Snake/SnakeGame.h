#pragma once
#include "../App.h"
#include "../Button.h"
#include "Board.h"
#include "Snake.h"
#include "Food.h"
#include <time.h>

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
	Vector2 windowSize;

	Snake snake;
	Food food;
	Board wall;
	int framesCounter;
	clock_t startTime;
} SnakeGame;
void SnakeGame_init(SnakeGame* self);
void SnakeGame_update(SnakeGame* self);
void SnakeGame_draw(SnakeGame* self);
void SnakeGame_updateDrawFrame(SnakeGame* self);

