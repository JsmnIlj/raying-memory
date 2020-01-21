#include "TowerBuilder.h"
#include "..//App.h"
#include "raylib/raylib.h"

enum GAME_STATE{
	PLAY,
	PAUSE,
	GAMEOVER
};
typedef struct Box{
	Rectangle rec;
	Vector2 speed;
	Color color;
	bool isSet;
	bool isFalling;
} Box;

int initTowerBuilder(App* self){
	enum GAME_STATE gameState = PLAY;

	Vector2 startPosition = { 200, 400 };
	Box box;
	box.rec = (Rectangle){ startPosition.x, startPosition.y, 100, 100 };
	box.speed.x = 4;
	box.speed.y = 0;
	box.color = RED;
	box.isSet = false;
	box.isFalling = false;

	enum {numBottomBoxes = 10};
	Box bottomBoxes[numBottomBoxes];
	for(int i = 0; i < 10; i++){
		bottomBoxes[i].rec = (Rectangle){ 400, self->windowSize.y - 100, 100, 100 };
		bottomBoxes[i].speed.x = 0;
		bottomBoxes[i].speed.y = 0;
		bottomBoxes[i].color = BLUE;
		bottomBoxes[i].isSet = false;
		bottomBoxes[i].isFalling = false;
	}

	int speed = 4;

	while(!WindowShouldClose()){
		//Update
		
		if(gameState != GAMEOVER){
			if(IsKeyPressed(KEY_SPACE)){
				if(gameState == PAUSE){
					gameState = PLAY;
				} else{
					gameState = PAUSE;
				}
			}
			if(gameState != PAUSE){
				if(IsKeyPressed(KEY_F)){
					box.isFalling = true;
					
				}

				if(box.isFalling == true){
					box.speed.x = 0;
					box.speed.y = 4;
				} else{
					box.speed.x = speed;
					box.speed.y = 0;
				}
				box.rec.x += box.speed.x;
				box.rec.y += box.speed.y;

				if(box.rec.x < 0 || box.rec.x + box.rec.width > self->windowSize.x){

					speed *= -1;
				}

				//BoxPlacement after Collision w/ startBox
				for(int i = 0; i < numBottomBoxes; i++){
					if(CheckCollisionRecs(box.rec, bottomBoxes[i].rec)){
						if(bottomBoxes[i].isSet == false) {
						bottomBoxes[i + 1] = box;
						bottomBoxes[i].isSet = true;

						box.isFalling = false;

						box.rec.x = startPosition.x;
						startPosition.y -= box.rec.height;

						box.rec.y = startPosition.y;
						
						} else{
							gameState = GAMEOVER;
						}
					}
				}
				//Collision with floor
				if(box.rec.y + box.rec.height > self->windowSize.y){
					gameState = GAMEOVER;
				}
			}

		}


		// Draw
		BeginDrawing();
		ClearBackground(RAYWHITE);
		
		//DrawTower Boxes 
		DrawRectangleRec(box.rec, box.color);
		for(int i = 0; i < numBottomBoxes; i++){
			DrawRectangleRec(bottomBoxes[i].rec, bottomBoxes[i].color);
		}
		
		if(gameState == GAMEOVER){
			DrawText("GAMEOVER", 400, 400, 60, BLUE);
		}


		//DrawText(FormatText("Score: %08i", score), self->windowSize.x+200, 0, 20, RED);
		

		
		DrawFPS(10, 10);

		EndDrawing();
	}

	return 0;
}
/*
static float x = 5.0f;
static float recPos = 0;

	if (IsKeyPressed(KEY_A)) {
		  x = 0;
		  collision = CheckCollisionRecs(boxA, boxB);
	  }
	  recPos += x;
	  if (recPos >= 500) {
		  x *= -1;
	  }

	   if (recPos <= 0) {
		  x *= -1;
	  }
	  */