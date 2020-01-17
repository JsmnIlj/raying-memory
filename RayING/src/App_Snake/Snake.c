#include "Snake.h"
#include "GlobalVariables.h"
#include "Food.h"

void Snake_init(Snake* self, Board* wall){
	for(int i = 0; i < MAX_TAIL_LENGTH; i++){
		self->segments[i] = (Rectangle){ 0, 0, 1, 1 };
	}
	self->segmentCount = 1;
	self->speed = (Vector2){ 1, 0 };
	self->allowMove = true;
}
void Snake_draw(Snake* self, Board* wall){
	DrawRectangle(
		(int)(SQUARE_SIZE * self->segments[0].x + wall->position.x),
		(int)(SQUARE_SIZE * self->segments[0].y + wall->position.y),
		(int)(SQUARE_SIZE * self->segments[0].width),
		(int)(SQUARE_SIZE * self->segments[0].height),
		self->headColor
	);

	for(int i = 1; i < self->segmentCount; i++){
		DrawRectangle(
			(int)(SQUARE_SIZE * self->segments[i].x + wall->position.x),
			(int)(SQUARE_SIZE * self->segments[i].y + wall->position.y),
			(int)(SQUARE_SIZE * self->segments[i].width),
			(int)(SQUARE_SIZE * self->segments[i].height),
			self->tailColor
		);
	}
}
void Snake_move(Snake* self, const int framesCounter){
	if((framesCounter % 10) == 0){
		for(int i = MAX_TAIL_LENGTH - 1; i > 0; i--){
			self->segments[i] = self->segments[i - 1];
		}
		self->segments[0].x += self->speed.x;
		self->segments[0].y += self->speed.y;
		self->allowMove = true;
	}
}
void Snake_control(Snake* self){
	float step = 1;

	if(IsKeyPressed(KEY_RIGHT) && self->speed.x == 0 && self->allowMove){
		self->speed.x = +step;
		self->speed.y = 0;
		self->allowMove = false;
	}
	if(IsKeyPressed(KEY_LEFT) && self->speed.x == 0 && self->allowMove){
		self->speed.x = -step;
		self->speed.y = 0;
		self->allowMove = false;
	}
	if(IsKeyPressed(KEY_UP) && self->speed.y == 0 && self->allowMove){
		self->speed.x = 0;
		self->speed.y = -step;
		self->allowMove = false;
	}
	if(IsKeyPressed(KEY_DOWN) && self->speed.y == 0 && self->allowMove){
		self->speed.x = 0;
		self->speed.y = +step;
		self->allowMove = false;
	}
}
bool Snake_isOutsideWall(Snake* self, Board* wall){

	if((self->segments[0].x < 0) ||
		(self->segments[0].y < 0) ||
	   (self->segments[0].x > wall->numCol - 1) ||
	   (self->segments[0].y > wall->numRow - 1)){
		return true;
	} else{
		return false;
	}
}
bool Snake_detectSelf(Snake* self, Board* wall){
	for(int i = 1; i < self->segmentCount; i++){
		if((self->segments[0].x == self->segments[i].x) &&
			(self->segments[0].y == self->segments[i].y)){
			return true;
		}
	}
	return false;
}
bool Snake_detectFood(Snake* self, Food* food){
	if((self->segments[0].x							   < (food->position.x + food->size.x) &&
		(self->segments[0].x + self->segments[0].width) > food->position.x) &&
		(self->segments[0].y							   < (food->position.y + food->size.y) &&
	   (self->segments[0].y + self->segments[0].height)> food->position.y)){

		food->isEaten = true;
		self->segmentCount += 1;
	}
}


