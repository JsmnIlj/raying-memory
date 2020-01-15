#include "App_Snake.h"
#include "raylib/raylib.h"
#include "SnakeGame.h"


void initSnakeGame(App* self){
	SnakeGame snakeGame;
	snakeGame.appPtr = self;
	SnakeGame_init(&snakeGame);
	while(snakeGame.running && !WindowShouldClose()){
		SnakeGame_updateDrawFrame(&snakeGame);
	}
}