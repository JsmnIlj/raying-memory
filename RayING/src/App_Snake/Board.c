#include "Board.h"


void Board_init(Board* self, int numCol, int numRow, int x, int y){

	self->numCol = 15;
	self->numRow = 15;

	self->position.x = (float)x;
	self->position.y = (float)y;
}
void Board_drawGrid(Board* self){
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
