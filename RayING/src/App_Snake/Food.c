#include "Food.h"
#include "Snake.h"

void Food_init(Food* self, Board* wall){
	self->position = (Vector2){ 0, 0 };
	self->size = (Vector2){ 1, 1 };
	self->isEaten = true;
	self->color = FOOD_COLOR;

}
void Food_draw(Food* self, Board* wall){

	DrawRectangle(
		(int)(SQUARE_SIZE * self->position.x + wall->position.x),
		(int)(SQUARE_SIZE * self->position.y + wall->position.y),
		(int)(SQUARE_SIZE * self->size.x),
		(int)(SQUARE_SIZE * self->size.y),
		self->color
	);


}
void Food_calculatePosition(Food* self, Board* wall, Snake* snake){
	if(self->isEaten){
		self->isEaten = false;

		self->position = (Vector2){
			GetRandomValue(0, wall->numCol - 1),
			GetRandomValue(0, wall->numRow - 1)
		};

		for(int i = 0; i < snake->segmentCount; i++){
			while((self->position.x == snake->segments[i].x) && (self->position.y == snake->segments[i].y)){

				self->position = (Vector2){
					GetRandomValue(0, wall->numCol - 1),
					GetRandomValue(0, wall->numRow - 1)
				};
				i = 0;
			}
		}
	}

}
