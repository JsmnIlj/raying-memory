#pragma once
#include "raylib/raylib.h"
#include "Board.h"
typedef struct Food Food;

typedef struct Snake{
	Rectangle segments[MAX_TAIL_LENGTH];
	int segmentCount;
	Vector2 speed;
	Color color;
	bool allowMove;
} Snake;

void Snake_init(Snake* self, Board* wall);
void Snake_draw(Snake* self, Board* wall);
void Snake_move(Snake* self, const int framesCounter);
void Snake_control(Snake* self);
bool Snake_isOutsideWall(Snake* self, Board* wall);
bool Snake_detectFood(Snake* self, Food* food);
bool Snake_detectSelf(Snake* self, Board* wall);
