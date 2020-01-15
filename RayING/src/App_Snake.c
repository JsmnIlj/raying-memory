#include "App_Snake.h"
#include "Button.h"
#include "raylib/raylib.h"
#define MAX_TAIL_LENGTH 256

const int SQUARE_SIZE = 40;

typedef struct Board{
	Vector2 position;
	int numCol;
	int numRow;
} Board;
typedef struct Food{
	Vector2 position;
	Vector2 size;
	bool isEaten;
	Color color;
} Food;
typedef struct Snake{
	Rectangle segments[MAX_TAIL_LENGTH];
	int segmentCount;
	Vector2 speed;
	Color color;
	bool allowMove;
} Snake;

void Board_init(Board* self, int x, int y, int width, int height);
void Board_drawGrid(Board* self);

void Food_init(Food* self, Board* wall);
void Food_draw(Food* self, Board* wall);
void Food_calculatePosition(Food* self, Board* wall, Snake* snake);


void Snake_init(Snake* self, Board* wall);
void Snake_draw(Snake* self, Board* wall);
void Snake_move(Snake* self, const int framesCounter);
void Snake_control(Snake* self);
bool Snake_isOutsideWall(Snake* self, Board* wall);
bool Snake_detectFood(Snake* self, Food* food);

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

	Snake snake;
	Food food;
	Board wall;
	int framesCounter;
} SnakeGame;
void SnakeGame_init(SnakeGame* self);
void SnakeGame_update(SnakeGame* self);
void SnakeGame_draw(SnakeGame* self);
void SnakeGame_updateDrawFrame(SnakeGame* self);


//
// IMPLEMENTATION
//


void Board_init(Board* self, int numCol, int numRow, int x, int y){

	self->numCol = 15;
	self->numRow = 15;

	self->position.x = x;
	self->position.y = y;
}
void Board_drawGrid(Board* self){
	//up-down
	for(int i = 0; i <= self->numCol; i++){
		DrawLine(
			self->position.x + i * SQUARE_SIZE,
			self->position.y,
			self->position.x + i * SQUARE_SIZE,
			self->position.y + self->numCol * SQUARE_SIZE,
			LIGHTGRAY
		);
	}
	//left-right
	for(int i = 0; i <= self->numRow; i++){
		DrawLine(
			self->position.x,
			self->position.y + i * SQUARE_SIZE,
			self->position.x + self->numRow * SQUARE_SIZE,
			self->position.y + i * SQUARE_SIZE,
			LIGHTGRAY
		);
	}
}

void Food_init(Food* self, Board* wall){
	self->position = (Vector2){ 0, 0 };
	self->size = (Vector2){ 1, 1 };
	self->isEaten = true;
	self->color = SKYBLUE;

}
void Food_draw(Food* self, Board* wall){

	DrawRectangle(
		SQUARE_SIZE * self->position.x + wall->position.x,
		SQUARE_SIZE * self->position.y + wall->position.y,
		SQUARE_SIZE * self->size.x,
		SQUARE_SIZE * self->size.y,
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

void Snake_init(Snake* self, Board* wall){
	for(int i = 0; i < MAX_TAIL_LENGTH; i++){
		self->segments[i] = (Rectangle){ 0, 0, 1, 1 };
	}
	self->segmentCount = 1;
	self->speed = (Vector2){ 1, 0 };
	self->color = DARKBLUE;
	self->allowMove = true;
}
void Snake_draw(Snake* self, Board* wall){
	DrawRectangle(
		SQUARE_SIZE * self->segments[0].x + wall->position.x,
		SQUARE_SIZE * self->segments[0].y + wall->position.y,
		SQUARE_SIZE * self->segments[0].width,
		SQUARE_SIZE * self->segments[0].height,
		self->color
	);

	for(int i = 1; i < self->segmentCount; i++){
		DrawRectangle(
			SQUARE_SIZE * self->segments[i].x + wall->position.x,
			SQUARE_SIZE * self->segments[i].y + wall->position.y,
			SQUARE_SIZE * self->segments[i].width,
			SQUARE_SIZE * self->segments[i].height,
			VIOLET
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


void SnakeGame_init(SnakeGame* self){
	self->gameState = PLAY;
	self->framesCounter = 0;


	int bX = SQUARE_SIZE;
	int bY = SQUARE_SIZE;

	Board_init(&self->wall, 15, 15, bX *2 , bY * 4);
	Snake_init(&self->snake, &self->wall);
	Food_init(&self->food, &self->wall);
}
void SnakeGame_update(SnakeGame* self){
	if(!(self->gameState == QUIT)){
		if(!(self->gameState == GAMEOVER)){

			if(IsKeyPressed(KEY_SPACE)){
				if(self->gameState == PLAY){
					self->gameState = PAUSE;
				} else{
					self->gameState = PLAY;
				}
			}

			if(!(self->gameState == PAUSE)){

				Snake_control(&self->snake);
				Snake_move(&self->snake, self->framesCounter++);
				if(Snake_isOutsideWall(&self->snake, &self->wall)){
					self->gameState = GAMEOVER;
				}
				if(Snake_detectSelf(&self->snake, &self->wall)){
					self->gameState = GAMEOVER;
				}
				Food_calculatePosition(&self->food, &self->wall, &self->snake);
				Snake_detectFood(&self->snake, &self->food);

			} else if(self->gameState == PAUSE){

			}
			self->framesCounter++;

		}
	} else{
		self->running = false;
	}
}
void SnakeGame_draw(SnakeGame* self){
	BeginDrawing();
	ClearBackground(RAYWHITE);
	DrawText(FormatText("Count: %i", self->snake.segmentCount), 280, 130, 40, MAROON);
	Board_drawGrid(&self->wall);
	Food_draw(&self->food, &self->wall);
	Snake_draw(&self->snake, &self->wall);
	if(self->gameState == PAUSE){
		DrawText("GAME PAUSED", self->appPtr->windowSize.x / 2 - MeasureText("GAME PAUSED", 40) / 2, self->appPtr->windowSize.y / 2 - 40, 40, GRAY);
	}
	if(self->gameState == GAMEOVER){
		int textPosX = self->appPtr->windowSize.x / 2 - MeasureText("GAME OVER", 40) / 2;
		int textPosY = self->appPtr->windowSize.y / 2 - 40;

		DrawText("GAME OVER", textPosX, textPosY, 40, GRAY);
		int fontSize = 40;

		Button repeatButton = { "Repeat", fontSize, {textPosX - 150, textPosX + 200, 1, 1} };
		Button_drawRec(&repeatButton);

		Button quitButton = { "Quit", fontSize, {textPosX + 150, textPosX + 200, 1, 1} };
		Button_drawRec(&quitButton);

		Vector2 mousepoint = GetMousePosition();
		if(CheckCollisionPointRec(mousepoint, repeatButton.rec)){
			if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
				SnakeGame_init(self);
			}
		}
		if(CheckCollisionPointRec(mousepoint, quitButton.rec)){
			if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
				self->gameState = QUIT;
			}
		}
	}
	EndDrawing();
}
void SnakeGame_updateDrawFrame(SnakeGame* self){
	SnakeGame_update(self);
	SnakeGame_draw(self);
}



void initSnakeGame(App* self){
	SnakeGame snakeGame;
	snakeGame.appPtr = self;
	SnakeGame_init(&snakeGame);
	while(snakeGame.running && !WindowShouldClose()){
		SnakeGame_updateDrawFrame(&snakeGame);
	}
}