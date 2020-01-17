#include "Board.h"


void Board_init(Board* self, int numCol, int numRow, int x, int y){
	self->numCol = numCol;
	self->numRow = numRow;

	self->position.x = (float)x;
	self->position.y = (float)y;

	self->position.x = GetMonitorWidth(0) / 2 - self->numCol * SQUARE_SIZE / 2;
	self->position.y = GetMonitorHeight(0) / 2 - self->numRow * SQUARE_SIZE / 2;
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
			self->color
		);
	}
	//left-right
	for(int i = 0; i <= self->numRow; i++){
		DrawLine(
			(int)self->position.x,
			(int)self->position.y + i * SQUARE_SIZE,
			(int)self->position.x + self->numRow * SQUARE_SIZE,
			(int)self->position.y + i * SQUARE_SIZE,
			self->color
		);
	}
}
