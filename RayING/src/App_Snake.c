#include "App_Snake.h"
#include "raylib/raylib.h"
#define SNAKE_LENGTH   256
#define SQUARE_SIZE     31
typedef struct Snake{
	Vector2 position;
	Vector2 speed;
	Vector2 size;
	Color color;

} Snake;
void Snake_init(Snake* self){
	SetTargetFPS(5);
	self->position = (Vector2){ 0, 0 };
	self->size = (Vector2){ SQUARE_SIZE, SQUARE_SIZE };
	self->speed = (Vector2){ SQUARE_SIZE, 0 };
	self->color = DARKBLUE;
}
void Snake_draw(Snake* self){
	DrawRectangleV(self->position, self->size, self->color);
}
void Snake_move(Snake* self){
	self->position.x += self->speed.x;
	self->position.y += self->speed.y;
}
void Snake_control(Snake* self){
	float step = SQUARE_SIZE;

	if(IsKeyPressed(KEY_RIGHT)){
		self->speed.x = +SQUARE_SIZE;
		self->speed.y = 0;
	}
	if(IsKeyPressed(KEY_LEFT)){
		self->speed.x = -SQUARE_SIZE;
		self->speed.y = 0;
	}
	if(IsKeyPressed(KEY_UP)){
		self->speed.x = 0;
		self->speed.y = -SQUARE_SIZE;
	}
	if(IsKeyPressed(KEY_DOWN)){
		self->speed.x = 0;
		self->speed.y = +SQUARE_SIZE;
	}
}

enum GAME_STATE{
	PLAY,
	PAUSE,
	GAMEOVER
};
typedef struct SnakeGame{
	int running;
	enum GAME_STATE gameState;

	Snake snake;
} SnakeGame;
void SnakeGame_init(SnakeGame* self){
	Snake_init(&self->snake);
}

void SnakeGame_update(SnakeGame* self){
	Snake_control(&self->snake);
	Snake_move(&self->snake);
}
void SnakeGame_draw(SnakeGame* self){
	BeginDrawing();
	ClearBackground(RAYWHITE);
	Snake_draw(&self->snake);

	EndDrawing();
}
void SnakeGame_updateDrawFrame(SnakeGame* self){
	SnakeGame_update(self);
	SnakeGame_draw(self);
}



void initSnakeGame(App* self){
	SnakeGame snakeGame;
	SnakeGame_init(&snakeGame);
	while(snakeGame.running && !WindowShouldClose()){
		SnakeGame_updateDrawFrame(&snakeGame);
	}
}

//#define SNAKE_LENGTH   256
//#define SQUARE_SIZE     31
//
//typedef struct Snake{
//	Vector2 position;
//	Vector2 size;
//	Vector2 speed;
//	Color color;
//} Snake;
//
//typedef struct Food{
//	Vector2 position;
//	Vector2 size;
//	bool isEaten;
//	Color color;
//} Food;
//enum GAME_STATE{
//	PLAY,
//	GAMEOVER,
//	PAUSE
//};
//typedef struct SnakeGame{
//	int screenWidth;
//	int screenHeight;
//	int framesCounter;
//
//	enum GAME_STATE gameState;
//	bool running;
//
//	Rectangle wall;
//	Food food;
//	Snake snake[SNAKE_LENGTH];
//	Snake snek;
//	Vector2 snakePosition[SNAKE_LENGTH];
//	bool allowMove;
//	Vector2 offset;
//	int counterTail;
//	App* appPtr;
//}SnakeGame;
//
//
//
//void Snake_init(SnakeGame* self){
//	for(int i = 0; i < SNAKE_LENGTH; i++){
//		self->snake[i].position = (Vector2){ self->offset.x / 2, self->offset.y / 2 };
//		self->snake[i].size = (Vector2){ SQUARE_SIZE, SQUARE_SIZE };
//		self->snake[i].speed = (Vector2){ SQUARE_SIZE, 0 };
//
//		if(i == 0){
//			self->snake[i].color = DARKBLUE;
//		} else{
//			self->snake[i].color = BLUE;
//		}
//	}
//
//	for(int i = 0; i < SNAKE_LENGTH; i++){
//		self->snakePosition[i] = (Vector2){ 0.0f, 0.0f };
//	}
//}
//void Snake_draw(SnakeGame* self){
//	// Draw snake
//	for(int i = 0; i < self->counterTail; i++){
//		DrawRectangleV(self->snake[i].position, self->snake[i].size, self->snake[i].color);
//	}
//}
//void Snake_control(SnakeGame* self){
//	if(IsKeyPressed(KEY_RIGHT) && (self->snake[0].speed.x == 0) && self->allowMove){
//		self->snake[0].speed = (Vector2){ SQUARE_SIZE, 0 };
//		self->allowMove = false;
//	}
//	if(IsKeyPressed(KEY_LEFT) && (self->snake[0].speed.x == 0) && self->allowMove){
//		self->snake[0].speed = (Vector2){ -SQUARE_SIZE, 0 };
//		self->allowMove = false;
//	}
//	if(IsKeyPressed(KEY_UP) && (self->snake[0].speed.y == 0) && self->allowMove){
//		self->snake[0].speed = (Vector2){ 0, -SQUARE_SIZE };
//		self->allowMove = false;
//	}
//	if(IsKeyPressed(KEY_DOWN) && (self->snake[0].speed.y == 0) && self->allowMove){
//		self->snake[0].speed = (Vector2){ 0, SQUARE_SIZE };
//		self->allowMove = false;
//	}
//}
//void Snake_move(SnakeGame* self){
//	for(int i = 0; i < self->counterTail; i++){
//		self->snakePosition[i] = self->snake[i].position;
//	}
//
//	if((self->framesCounter % 5) == 0){
//		for(int i = 0; i < self->counterTail; i++){
//			if(i == 0){
//				self->snake[0].position.x += self->snake[0].speed.x;
//				self->snake[0].position.y += self->snake[0].speed.y;
//				self->allowMove = true;
//			} else{
//				self->snake[i].position = self->snakePosition[i - 1];
//			}
//		}
//	}
//}
//void Snake_detectFood(SnakeGame* self){
//	if((self->snake[0].position.x < (self->food.position.x + self->food.size.x) &&
//		(self->snake[0].position.x + self->snake[0].size.x) > self->food.position.x) &&
//		(self->snake[0].position.y < (self->food.position.y + self->food.size.y) &&
//	   (self->snake[0].position.y + self->snake[0].size.y) > self->food.position.y)){
//		self->snake[self->counterTail].position = self->snakePosition[self->counterTail - 1];
//		self->counterTail += 1;
//		self->food.isEaten = true;
//	}
//}
//void Snake_detectSelfColision(SnakeGame* self){
//	for(int i = 1; i < self->counterTail; i++){
//		if((self->snake[0].position.x == self->snake[i].position.x) &&
//			(self->snake[0].position.y == self->snake[i].position.y)){
//			printf("%s", __FUNCTION__);
//			self->gameState = GAMEOVER;
//		}
//	}
//}
//void Snake_detectWall(SnakeGame* self){
//	if(!CheckCollisionPointRec(self->snake[0].position, self->wall)){
//		printf("%s\n", __FUNCTION__);
//		printf("SnakePos: %f %f, WalPos: %f %f %f %f\n",
//			   self->snake[0].position.x, self->snake[0].position.y,
//			   self->wall.x, self->wall.y, self->wall.width, self->wall.height
//		);
//		self->gameState = GAMEOVER;
//	}
//}
//
//void Food_init(Food* self){
//	self->size = (Vector2){ SQUARE_SIZE, SQUARE_SIZE };
//	self->color = SKYBLUE;
//	self->isEaten = true;
//}
//void Food_draw(Food* self){
//	DrawRectangleV(self->position, self->size, self->color);
//}
//void Food_calculatePosition(SnakeGame* self){
//	if(self->food.isEaten){
//		self->food.isEaten = false;
//		self->food.position = (Vector2){ 
//			GetRandomValue(0, (self->screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + self->offset.x / 2, 
//			GetRandomValue(0, (self->screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + self->offset.y / 2 
//		};
//
//		for(int i = 0; i < self->counterTail; i++){
//			while((self->food.position.x == self->snake[i].position.x) && (self->food.position.y == self->snake[i].position.y)){
//				self->food.position = (Vector2){ GetRandomValue(0, (self->screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + self->offset.x / 2, GetRandomValue(0, (self->screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + self->offset.y / 2 };
//				i = 0;
//			}
//		}
//	}
//}
//
//void SnakeGame_init(SnakeGame* self){
//
//	self->screenWidth = self->appPtr->width;
//	self->screenHeight = self->appPtr->height;
//	self->offset = (Vector2){ 0 };
//	self->offset.x = self->screenWidth % SQUARE_SIZE;
//	self->offset.y = self->screenWidth % SQUARE_SIZE;
//	self->wall = (Rectangle){ 0, 0, (self->screenWidth - self->offset.x), (self->screenHeight - self->offset.y) };
//
//
//
//	self->gameState = PLAY;
//	self->running = true;
//	self->allowMove = false;
//	self->framesCounter = 0;
//	self->counterTail = 1;
//
//	Snake_init(self);
//	Food_init(self);
//}
//void SnakeGame_update(SnakeGame* self){
//	if(!(self->gameState == GAMEOVER)){
//		if(IsKeyPressed('P') || IsKeyPressed(KEY_SPACE)){
//			if(self->gameState == PLAY){
//				self->gameState = PAUSE;
//			} else{
//				self->gameState = PLAY;
//			}
//		}
//
//		if(!(self->gameState == PAUSE)){
//			Snake_control(self);
//			Snake_move(self);
//			Snake_detectWall(self);
//			Snake_detectSelfColision(self);
//			Food_calculatePosition(self);
//			Snake_detectFood(self);
//			self->framesCounter++;
//		}
//	} else if(IsKeyPressed(KEY_ENTER)){
//		SnakeGame_init(self);
//		self->gameState = PLAY;
//
//	}
//}
//void SnakeGame_drawGrid(SnakeGame* self){
//	// Draw grid lines
//	for(int i = 0; i < self->screenWidth / SQUARE_SIZE + 1; i++){
//		DrawLineV((Vector2){ SQUARE_SIZE* i + self->offset.x / 2, self->offset.y / 2 }, (Vector2){ SQUARE_SIZE* i + self->offset.x / 2, self->screenHeight - self->offset.y / 2 }, LIGHTGRAY);
//	}
//
//	for(int i = 0; i < self->screenHeight / SQUARE_SIZE + 1; i++){
//		DrawLineV((Vector2){ self->offset.x / 2, SQUARE_SIZE* i + self->offset.y / 2 }, (Vector2){ self->screenWidth - self->offset.x / 2, SQUARE_SIZE* i + self->offset.y / 2 }, LIGHTGRAY);
//	}
//}
//void SnakeGame_draw(SnakeGame* self){
//	BeginDrawing();
//
//	ClearBackground(RAYWHITE);
//
//	if(!(self->gameState == GAMEOVER)){
//		SnakeGame_drawGrid(self);
//		Snake_draw(self);
//		Food_draw(&self->food);
//
//		if(self->gameState == PAUSE){
//			DrawText("GAME PAUSED", self->screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, self->screenHeight / 2 - 40, 40, GRAY);
//		}
//	} else{
//		DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 2 - 50, 20, GRAY);
//	}
//
//	EndDrawing();
//}
//void SnakeGame_updateDrawFrame(SnakeGame* self){
//	SnakeGame_update(self);
//	SnakeGame_draw(self);
//}
//
//void initSnakeGame(App* self){
//	SnakeGame snakeGame;
//	snakeGame.appPtr = self;
//	SnakeGame_init(&snakeGame);
//	while(snakeGame.running && !WindowShouldClose()){
//		SnakeGame_updateDrawFrame(&snakeGame);
//	}
//}