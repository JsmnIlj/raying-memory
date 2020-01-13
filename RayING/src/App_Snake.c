#include "App_Snake.h"
#include "Button.h"
#include "raylib/raylib.h"
#define MAX_TAIL_LEGNTH 256
#define SQUARE_SIZE       31

typedef struct Box{
	Rectangle edges;
} Box;
void Box_init(Box* self, int x, int y, int width, int height);
void Box_drawGrid(Box* self);

typedef struct Food{
	Vector2 position;
	Vector2 size;
	bool isEaten;
	Color color;
} Food;
void Food_init(Food* self);
void Food_draw(Food* self);
void Food_calculatePosition(Food* self, Vector2* winSize);

typedef struct Snake{
	Rectangle segments[MAX_TAIL_LEGNTH];
	int segmentCount;
	Vector2 speed;
	Color color;
} Snake;
void Snake_init(Snake* self, Box* wall);
void Snake_draw(Snake* self);
void Snake_move(Snake* self);
void Snake_control(Snake* self);
bool Snake_detectWall(Snake* self, Box* wall);
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
	Box wall;
	Vector2 offset;
} SnakeGame;
void SnakeGame_init(SnakeGame* self);
void SnakeGame_update(SnakeGame* self);
void SnakeGame_draw(SnakeGame* self);
void SnakeGame_updateDrawFrame(SnakeGame* self);


//
// IMPLEMENTATION
//


void Box_init(Box* self, int x, int y, int width, int height){
	self->edges.x = 0;
	self->edges.y = 0;
	self->edges.width = width;
	self->edges.height = height;

}
void Box_drawGrid(Box* self){
	//left-right
	for(int i = 0; i < self->edges.width / SQUARE_SIZE + 1; i++){
		DrawLineV((Vector2){ SQUARE_SIZE* i + SQUARE_SIZE, 0}, (Vector2){ SQUARE_SIZE* i + SQUARE_SIZE, self->edges.height - 0 }, LIGHTGRAY);
	}
	//up-down
	for(int i = 0; i < self->edges.height / SQUARE_SIZE + 1; i++){
		DrawLineV((Vector2){ SQUARE_SIZE, SQUARE_SIZE* i + SQUARE_SIZE }, (Vector2){ self->edges.width - SQUARE_SIZE, SQUARE_SIZE* i + SQUARE_SIZE}, LIGHTGRAY);
	}
}

void Food_init(Food* self){
	self->position = (Vector2){ 0,0 };
	self->size = (Vector2){ SQUARE_SIZE, SQUARE_SIZE };
	self->isEaten = true;
	self->color = SKYBLUE;

}
void Food_draw(Food* self){
	DrawRectangleV(self->position, self->size, self->color);
}
void Food_calculatePosition(Food* self, Vector2* winSize){
	if(self->isEaten){
		self->isEaten = false;
		self->position = (Vector2){
			GetRandomValue(0, (winSize->x / SQUARE_SIZE) - 1) * SQUARE_SIZE + 0,
			GetRandomValue(0, (winSize->y / SQUARE_SIZE) - 1) * SQUARE_SIZE + 0
		};
	}
}

void Snake_init(Snake* self, Box* wall){
	SetTargetFPS(10);
	for(int i = 0; i < MAX_TAIL_LEGNTH; i++){
		self->segments[0] = (Rectangle){ wall->edges.x, wall->edges.y, SQUARE_SIZE, SQUARE_SIZE };
	}
	self->segmentCount = 1;
	self->speed = (Vector2){ SQUARE_SIZE, 0 };
	self->color = DARKBLUE;
}
void Snake_draw(Snake* self){
	DrawRectangleV((Vector2){ self->segments[0].x, self->segments[0].y }, (Vector2){ self->segments[0].width, self->segments[0].height }, self->color);

	for(int i = 1; i < self->segmentCount; i++){
		int resize = 0;
		DrawRectangleV((Vector2){ self->segments[i].x + resize, self->segments[i].y + resize }, (Vector2){ self->segments[i].width - resize * 2, self->segments[i].height - resize * 2 }, VIOLET);
		//CLITERAL{ i * 30, 0, 0, 255 }
	}
}
void Snake_move(Snake* self){
	for(int i = MAX_TAIL_LEGNTH - 1; i > 0; i--){
		self->segments[i] = self->segments[i - 1];
	}
	self->segments[0].x += self->speed.x;
	self->segments[0].y += self->speed.y;
}
void Snake_control(Snake* self){
	float step = SQUARE_SIZE;

	if(IsKeyPressed(KEY_RIGHT) && self->speed.x == 0){
		self->speed.x = +step;
		self->speed.y = 0;
	}
	if(IsKeyPressed(KEY_LEFT) && self->speed.x == 0){
		self->speed.x = -step;
		self->speed.y = 0;
	}
	if(IsKeyPressed(KEY_UP) && self->speed.y == 0){
		self->speed.x = 0;
		self->speed.y = -step;
	}
	if(IsKeyPressed(KEY_DOWN) && self->speed.y == 0){
		self->speed.x = 0;
		self->speed.y = +step;
	}
}
bool Snake_detectWall(Snake* self, Box* wall){
	if(!CheckCollisionRecs((Rectangle){ self->segments[0].x, self->segments[0].y, self->segments[0].width, self->segments[0].height }, (Rectangle){ wall->edges.x, wall->edges.y, wall->edges.width - SQUARE_SIZE, wall->edges.height - SQUARE_SIZE })){
		return true;
	} else{
		return false;
	}
}
bool Snake_detectSelf(Snake* self, Box* wall){
	for(int i = 1; i < self->segmentCount; i++){
		if((self->segments[0].x == self->segments[i].x) &&
			(self->segments[0].y == self->segments[i].y)){
			return true;
		}
	}
	return false;
}
void Snake_growTail(Snake* self){
	self->segmentCount += 1;
}
bool Snake_detectFood(Snake* self, Food* food){
	if((self->segments[0].x < (food->position.x + food->size.x) &&
		(self->segments[0].x + self->segments[0].width) > food->position.x) &&
		(self->segments[0].y < (food->position.y + food->size.y) &&
	   (self->segments[0].y + self->segments[0].height) > food->position.y)){
		food->isEaten = true;
		Snake_growTail(self);
	}
}


void SnakeGame_init(SnakeGame* self){
	self->gameState = PLAY;
	self->offset.x = (int)self->appPtr->windowSize.x % SQUARE_SIZE;
	self->offset.y = (int)self->appPtr->windowSize.y % SQUARE_SIZE;

	printf("selfOffsetX: %d\n", (int)self->appPtr->windowSize.x % SQUARE_SIZE);
	printf("selfOffsetY: %d\n", (int)self->appPtr->windowSize.y % SQUARE_SIZE);



	Box_init(&self->wall, 0 + self->offset.x, 0 + self->offset.y, self->appPtr->windowSize.x - self->offset.x, self->appPtr->windowSize.y - self->offset.y);
	Snake_init(&self->snake, &self->wall);
	Food_init(&self->food);
	self->wall.edges = (Rectangle){ 0, 0, self->appPtr->windowSize.x, self->appPtr->windowSize.y };
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
				Snake_move(&self->snake);
				if(Snake_detectWall(&self->snake, &self->wall)){
					self->gameState = GAMEOVER;
				}
				if(Snake_detectSelf(&self->snake, &self->wall)){
					self->gameState = GAMEOVER;
				}
				Food_calculatePosition(&self->food, &self->appPtr->windowSize);
				Snake_detectFood(&self->snake, &self->food);

			} else if(self->gameState == PAUSE){

			}

		}
	} else{
		self->running = false;
	}
}
void SnakeGame_draw(SnakeGame* self){
	BeginDrawing();
	ClearBackground(RAYWHITE);
	Box_drawGrid(&self->wall);
	Food_draw(&self->food);
	Snake_draw(&self->snake);
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



//_______
//
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
//	 Draw snake
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
//			while((self->food.position.x == self->snake[i].position.x) && 
//				  (self->food.position.y == self->snake[i].position.y)){
//				self->food.position = (Vector2){ 
//					GetRandomValue(0, (self->screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + self->offset.x / 2, 
//					GetRandomValue(0, (self->screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + self->offset.y / 2 
//				};
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
//	 Draw grid lines
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






////______----____--__-__-__-__-
//#if defined(PLATFORM_WEB)
//#include <emscripten/emscripten.h>
//#endif
//
////----------------------------------------------------------------------------------
//// Some Defines
////----------------------------------------------------------------------------------
//#define SNAKE_LENGTH   256
//#define SQUARE_SIZE     31
//
////----------------------------------------------------------------------------------
//// Types and Structures Definition
////----------------------------------------------------------------------------------
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
//	bool active;
//	Color color;
//} Food;
//
////------------------------------------------------------------------------------------
//// Global Variables Declaration
////------------------------------------------------------------------------------------
//static const int screenWidth = 800;
//static const int screenHeight = 450;
//
//static int framesCounter = 0;
//static bool gameOver = false;
//static bool pause = false;
//
//static Food fruit = { 0 };
//static Snake snake[SNAKE_LENGTH] = { 0 };
//static Vector2 snakePosition[SNAKE_LENGTH] = { 0 };
//static bool allowMove = false;
//static Vector2 offset = { 0 };
//static int counterTail = 0;
//
////------------------------------------------------------------------------------------
//// Module Functions Declaration (local)
////------------------------------------------------------------------------------------
//static void InitGame(void);         // Initialize game
//static void UpdateGame(void);       // Update game (one frame)
//static void DrawGame(void);         // Draw game (one frame)
//static void UnloadGame(void);       // Unload game
//static void UpdateDrawFrame(void);  // Update and Draw (one frame)
//
//////------------------------------------------------------------------------------------
////// Program main entry point
//////------------------------------------------------------------------------------------
////int main(void){
////    // Initialization (Note windowTitle is unused on Android)
////    //---------------------------------------------------------
////    InitWindow(screenWidth, screenHeight, "sample game: snake");
////
////    InitGame();
////
////#if defined(PLATFORM_WEB)
////    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
////#else
////    SetTargetFPS(60);
////    //--------------------------------------------------------------------------------------
////
////    // Main game loop
////    while(!WindowShouldClose())    // Detect window close button or ESC key
////    {
////        // Update and Draw
////        //----------------------------------------------------------------------------------
////        UpdateDrawFrame();
////        //----------------------------------------------------------------------------------
////    }
////#endif
////    // De-Initialization
////    //--------------------------------------------------------------------------------------
////    UnloadGame();         // Unload loaded data (textures, sounds, models...)
////
////    CloseWindow();        // Close window and OpenGL context
////    //--------------------------------------------------------------------------------------
////
////    return 0;
////}
//
//
//
//
////---------------------------------------------------------------------------------------------------------------------------------------
////------------------------------------------------------------------------------------
//// Module Functions Definitions (local)
////------------------------------------------------------------------------------------
//
//// Initialize game variables
//void InitGame(void){
//	framesCounter = 0;
//	gameOver = false;
//	pause = false;
//
//	counterTail = 1;
//	allowMove = false;
//
//	offset.x = screenWidth % SQUARE_SIZE;
//	offset.y = screenHeight % SQUARE_SIZE;
//
//	for(int i = 0; i < SNAKE_LENGTH; i++){
//		snake[i].position = (Vector2){ offset.x / 2, offset.y / 2 };
//		snake[i].size = (Vector2){ SQUARE_SIZE, SQUARE_SIZE };
//		snake[i].speed = (Vector2){ SQUARE_SIZE, 0 };
//
//		if(i == 0) snake[i].color = DARKBLUE;
//		else snake[i].color = BLUE;
//	}
//
//	for(int i = 0; i < SNAKE_LENGTH; i++){
//		snakePosition[i] = (Vector2){ 0.0f, 0.0f };
//	}
//
//	fruit.size = (Vector2){ SQUARE_SIZE, SQUARE_SIZE };
//	fruit.color = SKYBLUE;
//	fruit.active = false;
//}
//
//// Update game (one frame)
//void UpdateGame(void){
//	if(!gameOver){
//		if(IsKeyPressed('P')) pause = !pause;
//
//		if(!pause){
//			// Player control
//			if(IsKeyPressed(KEY_RIGHT) && (snake[0].speed.x == 0) && allowMove){
//				snake[0].speed = (Vector2){ SQUARE_SIZE, 0 };
//				allowMove = false;
//			}
//			if(IsKeyPressed(KEY_LEFT) && (snake[0].speed.x == 0) && allowMove){
//				snake[0].speed = (Vector2){ -SQUARE_SIZE, 0 };
//				allowMove = false;
//			}
//			if(IsKeyPressed(KEY_UP) && (snake[0].speed.y == 0) && allowMove){
//				snake[0].speed = (Vector2){ 0, -SQUARE_SIZE };
//				allowMove = false;
//			}
//			if(IsKeyPressed(KEY_DOWN) && (snake[0].speed.y == 0) && allowMove){
//				snake[0].speed = (Vector2){ 0, SQUARE_SIZE };
//				allowMove = false;
//			}
//
//			// Snake movement
//			for(int i = 0; i < counterTail; i++) snakePosition[i] = snake[i].position;
//
//			if((framesCounter % 60) == 0){
//				for(int i = 0; i < counterTail; i++){
//					if(i == 0){
//						snake[0].position.x += snake[0].speed.x;
//						snake[0].position.y += snake[0].speed.y;
//						allowMove = true;
//					} else snake[i].position = snakePosition[i - 1];
//				}
//			}
//
//			// Wall behaviour
//			if(((snake[0].position.x) > (screenWidth - offset.x)) ||
//				((snake[0].position.y) > (screenHeight - offset.y)) ||
//			   (snake[0].position.x < 0) || (snake[0].position.y < 0)){
//				gameOver = true;
//			}
//
//			// Collision with yourself
//			for(int i = 1; i < counterTail; i++){
//				if((snake[0].position.x == snake[i].position.x) && (snake[0].position.y == snake[i].position.y)) gameOver = true;
//			}
//
//			// Fruit position calculation
//			if(!fruit.active){
//				fruit.active = true;
//				fruit.position = (Vector2){ GetRandomValue(0, (screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.x / 2, GetRandomValue(0, (screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.y / 2 };
//
//				for(int i = 0; i < counterTail; i++){
//					while((fruit.position.x == snake[i].position.x) && (fruit.position.y == snake[i].position.y)){
//						fruit.position = (Vector2){ GetRandomValue(0, (screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.x / 2, GetRandomValue(0, (screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.y / 2 };
//						i = 0;
//					}
//				}
//			}
//
//			// Collision
//			if((snake[0].position.x < (fruit.position.x + fruit.size.x) && (snake[0].position.x + snake[0].size.x) > fruit.position.x) &&
//				(snake[0].position.y < (fruit.position.y + fruit.size.y) && (snake[0].position.y + snake[0].size.y) > fruit.position.y)){
//				snake[counterTail].position = snakePosition[counterTail - 1];
//				counterTail += 1;
//				fruit.active = false;
//			}
//
//			framesCounter++;
//		}
//	} else{
//		if(IsKeyPressed(KEY_ENTER)){
//			InitGame();
//			gameOver = false;
//		}
//	}
//}
//
//// Draw game (one frame)
//void DrawGame(void){
//	BeginDrawing();
//
//	ClearBackground(RAYWHITE);
//
//	if(!gameOver){
//		// Draw grid lines
//		for(int i = 0; i < screenWidth / SQUARE_SIZE + 1; i++){
//			DrawLineV((Vector2){ SQUARE_SIZE* i + offset.x / 2, offset.y / 2 }, (Vector2){ SQUARE_SIZE* i + offset.x / 2, screenHeight - offset.y / 2 }, LIGHTGRAY);
//		}
//
//		for(int i = 0; i < screenHeight / SQUARE_SIZE + 1; i++){
//			DrawLineV((Vector2){ offset.x / 2, SQUARE_SIZE* i + offset.y / 2 }, (Vector2){ screenWidth - offset.x / 2, SQUARE_SIZE* i + offset.y / 2 }, LIGHTGRAY);
//		}
//
//		// Draw snake
//		for(int i = 0; i < counterTail; i++) DrawRectangleV(snake[i].position, snake[i].size, CLITERAL{ i * 60,i * 60,i * 60, 255 });
//
//		// Draw fruit to pick
//		DrawRectangleV(fruit.position, fruit.size, fruit.color);
//
//		if(pause) DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, screenHeight / 2 - 40, 40, GRAY);
//	} else DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 2 - 50, 20, GRAY);
//
//	EndDrawing();
//}
//
//// Unload game variables
//void UnloadGame(void){
//	// TODO: Unload all dynamic loaded data (textures, sounds, models...)
//}
//
//// Update and Draw (one frame)
//void UpdateDrawFrame(void){
//	UpdateGame();
//	DrawGame();
//}
//
//void initSnakeGame(App* self){
//	InitGame();
//	while(!WindowShouldClose()){
//		UpdateDrawFrame();
//	}
//}