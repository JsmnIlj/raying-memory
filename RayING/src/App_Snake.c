#include "App_Snake.h"
#include "raylib/raylib.h"

#define SNAKE_LENGTH   256
#define SQUARE_SIZE     31

typedef struct Snake{
	Vector2 position;
	Vector2 size;
	Vector2 speed;
	Color color;
} Snake;

typedef struct Food{
	Vector2 position;
	Vector2 size;
	bool active;
	Color color;
} Food;

typedef struct SnakeGame{
	int screenWidth;
	int screenHeight;

	int framesCounter;
	bool gameOver;
	bool pause;

	Food fruit;
	Snake snake[SNAKE_LENGTH];
	Vector2 snakePosition[SNAKE_LENGTH];
	bool allowMove;
	Vector2 offset;
	int counterTail;
	int running;
	App* appPtr;
}SnakeGame;





void initGame(SnakeGame* self){

	self->screenWidth = self->appPtr->width;
	self->screenHeight = self->appPtr->height;

	self->fruit = (Food){ 0 };
	self->snake[SNAKE_LENGTH] = (Snake){ 0 };
	self->snakePosition[SNAKE_LENGTH] = (Vector2){ 0 };
	self->allowMove = false;
	self->offset = (Vector2){ 0 };
	self->running = 1;
	self->framesCounter = 0;
	self->gameOver = false;
	self->pause = false;

	self->counterTail = 1;
	self->allowMove = false;

	self->offset.x = self->appPtr->width % SQUARE_SIZE;
	self->offset.y = self->appPtr->height % SQUARE_SIZE;

	for(int i = 0; i < SNAKE_LENGTH; i++){
		self->snake[i].position = (Vector2){ self->offset.x / 2, self->offset.y / 2 };
		self->snake[i].size = (Vector2){ SQUARE_SIZE, SQUARE_SIZE };
		self->snake[i].speed = (Vector2){ SQUARE_SIZE, 0 };

		if(i == 0) self->snake[i].color = DARKBLUE;
		else self->snake[i].color = BLUE;
	}

	for(int i = 0; i < SNAKE_LENGTH; i++){
		self->snakePosition[i] = (Vector2){ 0.0f, 0.0f };
	}

	self->fruit.size = (Vector2){ SQUARE_SIZE, SQUARE_SIZE };
	self->fruit.color = SKYBLUE;
	self->fruit.active = false;
}


void controlSnake(SnakeGame* self){
	if(IsKeyPressed(KEY_RIGHT) && (self->snake[0].speed.x == 0) && self->allowMove){
		self->snake[0].speed = (Vector2){ SQUARE_SIZE, 0 };
		self->allowMove = false;
	}
	if(IsKeyPressed(KEY_LEFT) && (self->snake[0].speed.x == 0) && self->allowMove){
		self->snake[0].speed = (Vector2){ -SQUARE_SIZE, 0 };
		self->allowMove = false;
	}
	if(IsKeyPressed(KEY_UP) && (self->snake[0].speed.y == 0) && self->allowMove){
		self->snake[0].speed = (Vector2){ 0, -SQUARE_SIZE };
		self->allowMove = false;
	}
	if(IsKeyPressed(KEY_DOWN) && (self->snake[0].speed.y == 0) && self->allowMove){
		self->snake[0].speed = (Vector2){ 0, SQUARE_SIZE };
		self->allowMove = false;
	}
}
void moveSnake(SnakeGame* self){
	for(int i = 0; i < self->counterTail; i++){
		self->snakePosition[i] = self->snake[i].position;
	}

	if((self->framesCounter % 5) == 0){
		for(int i = 0; i < self->counterTail; i++){
			if(i == 0){
				self->snake[0].position.x += self->snake[0].speed.x;
				self->snake[0].position.y += self->snake[0].speed.y;
				self->allowMove = true;
			} else self->snake[i].position = self->snakePosition[i - 1];
		}
	}
}
void wallBehavior(SnakeGame* self){
	if(((self->snake[0].position.x) > (self->screenWidth - self->offset.x)) ||
		((self->snake[0].position.y) > (self->screenHeight - self->offset.y)) ||
	   (self->snake[0].position.x < 0) || (self->snake[0].position.y < 0)){
		self->gameOver = true;
	}
}
void detectSelfColision(SnakeGame* self){
	for(int i = 1; i < self->counterTail; i++){
		if((self->snake[0].position.x == self->snake[i].position.x) && (self->snake[0].position.y == self->snake[i].position.y)) self->gameOver = true;
	}
}
void calculateFruitPosition(SnakeGame* self){
	if(!self->fruit.active){
		self->fruit.active = true;
		self->fruit.position = (Vector2){ GetRandomValue(0, (self->screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + self->offset.x / 2, GetRandomValue(0, (self->screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + self->offset.y / 2 };

		for(int i = 0; i < self->counterTail; i++){
			while((self->fruit.position.x == self->snake[i].position.x) && (self->fruit.position.y == self->snake[i].position.y)){
				self->fruit.position = (Vector2){ GetRandomValue(0, (self->screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + self->offset.x / 2, GetRandomValue(0, (self->screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + self->offset.y / 2 };
				i = 0;
			}
		}
	}
}
void detectCollision(SnakeGame* self){
	if((self->snake[0].position.x < (self->fruit.position.x + self->fruit.size.x) && (self->snake[0].position.x + self->snake[0].size.x) > self->fruit.position.x) &&
		(self->snake[0].position.y < (self->fruit.position.y + self->fruit.size.y) && (self->snake[0].position.y + self->snake[0].size.y) > self->fruit.position.y)){
		self->snake[self->counterTail].position = self->snakePosition[self->counterTail - 1];
		self->counterTail += 1;
		self->fruit.active = false;
	}
}
void updateGame(SnakeGame* self){
	if(!self->gameOver){
		if(IsKeyPressed('P')){
			self->pause = !self->pause;
		}

		if(!self->pause){
			controlSnake(self);
			moveSnake(self);
			wallBehavior(self);
			detectSelfColision(self);
			calculateFruitPosition(self);
			detectCollision(self);
			self->framesCounter++;
		}
	} else if(IsKeyPressed(KEY_ENTER)){
		initGame(self);
		self->gameOver = false;

	}
}

void drawGrid(SnakeGame* self){
	// Draw grid lines
	for(int i = 0; i < self->screenWidth / SQUARE_SIZE + 1; i++){
		DrawLineV((Vector2){ SQUARE_SIZE* i + self->offset.x / 2, self->offset.y / 2 }, (Vector2){ SQUARE_SIZE* i + self->offset.x / 2, self->screenHeight - self->offset.y / 2 }, LIGHTGRAY);
	}

	for(int i = 0; i < self->screenHeight / SQUARE_SIZE + 1; i++){
		DrawLineV((Vector2){ self->offset.x / 2, SQUARE_SIZE* i + self->offset.y / 2 }, (Vector2){ self->screenWidth - self->offset.x / 2, SQUARE_SIZE* i + self->offset.y / 2 }, LIGHTGRAY);
	}
}
void drawSnake(SnakeGame* self){
	// Draw snake
	for(int i = 0; i < self->counterTail; i++){
		DrawRectangleV(self->snake[i].position, self->snake[i].size, self->snake[i].color);
	}
}
void drawFruit(SnakeGame* self){
	// Draw fruit to pick
	DrawRectangleV(self->fruit.position, self->fruit.size, self->fruit.color);
}
void drawGame(SnakeGame* self){
	BeginDrawing();

	ClearBackground(RAYWHITE);

	if(!self->gameOver){
		drawGrid(self);
		drawSnake(self);
		drawFruit(self);

		if(self->pause){
			DrawText("GAME PAUSED", self->screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, self->screenHeight / 2 - 40, 40, GRAY);
		}
	} else{
		DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 2 - 50, 20, GRAY);
	}

	EndDrawing();
}
void updateDrawFrame(SnakeGame* self){
	updateGame(self);
	drawGame(self);
}
void initSnakeGame(App* self){

	SnakeGame snakeGame;
	snakeGame.appPtr = self;
	initGame(&snakeGame);
	while(snakeGame.running && !WindowShouldClose()){
		updateDrawFrame(&snakeGame);
	}
}