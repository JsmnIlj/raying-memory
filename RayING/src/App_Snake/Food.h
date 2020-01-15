#pragma once
#include "raylib/raylib.h"
#include "Board.h"

typedef struct Snake Snake;
typedef struct Food{
	Vector2 position;
	Vector2 size;
	bool isEaten;
	Color color;
} Food;

void Food_init(Food* self, Board* wall);
void Food_draw(Food* self, Board* wall);
void Food_calculatePosition(Food* self, Board* wall, Snake* snake);