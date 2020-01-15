#include "Board.h"


void Board_init(Board* self, int numCol, int numRow, int x, int y){

	self->numCol = 15;
	self->numRow = 15;



	self->position.x = (float)x;
	self->position.y = (float)y;

	self->position.x = GetMonitorWidth(0) / 2 - self->numCol * SQUARE_SIZE / 2;
	self->position.y = GetMonitorHeight(0) / 2 - self->numRow * SQUARE_SIZE / 2;

	self->target = LoadRenderTexture(GetMonitorWidth(0), GetMonitorHeight(0));


}
void Board_drawGrid(Board* self, Vector2 windowSize){



	self->position.x = windowSize.x/ 2 - self->numCol * SQUARE_SIZE / 2;
	self->position.y = windowSize.y/ 2 - self->numRow * SQUARE_SIZE / 2;
	//up-down
	for(int i = 0; i <= self->numCol; i++){
		DrawLine(
			(int)self->position.x + i * SQUARE_SIZE,
			(int)self->position.y,
			(int)self->position.x + i * SQUARE_SIZE,
			(int)self->position.y + self->numCol * SQUARE_SIZE,
			GRID_COLOR
		);
	}
	//left-right
	for(int i = 0; i <= self->numRow; i++){
		DrawLine(
			(int)self->position.x,
			(int)self->position.y + i * SQUARE_SIZE,
			(int)self->position.x + self->numRow * SQUARE_SIZE,
			(int)self->position.y + i * SQUARE_SIZE,
			GRID_COLOR
		);
	}
}
