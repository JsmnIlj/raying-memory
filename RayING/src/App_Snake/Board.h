#pragma once
#include "raylib/raylib.h"
#include "GlobalVariables.h"

typedef struct Board{
	Vector2 position;
	int numCol;
	int numRow;
	RenderTexture2D target;
	Color color;
} Board;

void Board_init(Board* self, int numCol, int numRow, int x, int y);
void Board_drawGrid(Board* self, Vector2 windowSize);
