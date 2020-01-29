#include "TowerBuilder.h"
#include "..//App.h"
#include "raylib/raylib.h"

enum GAME_STATE {
	PLAY,
	PAUSE,
	GAMEOVER
};
typedef struct Box {
	Rectangle rec;
	Vector2 speed;
	Color color;
	bool isSet;
	bool isFalling;
} Box;

#define MAX_BOXES 10
typedef struct TBGame {
	App* appPtr;

	int score;

	Vector2 startPosition;
	Box mainBox;
	Box bottomBoxes[MAX_BOXES];
}TBGame;

TBGame_init(TBGame* self) {
	self->score = 0;
	self->startPosition = (Vector2){ 200, 400 };
	self->mainBox.rec = (Rectangle){ self->startPosition.x, self->startPosition.y, 100, 100 };
	self->mainBox.speed.x = 4;
	self->mainBox.speed.y = 0;
	self->mainBox.color = RED;
	self->mainBox.isSet = false;
	self->mainBox.isFalling = false;

	for(int i = 0; i < 10; i++) {
		self->bottomBoxes[i].rec = (Rectangle){ self->appPtr->windowSize.x / 2, self->appPtr->windowSize.y - 100, 100, 100 };
		self->bottomBoxes[i].speed.x = 0;
		self->bottomBoxes[i].speed.y = 0;
		self->bottomBoxes[i].color = BLUE;
		self->bottomBoxes[i].isSet = false;
		self->bottomBoxes[i].isFalling = false;
	}
}

void Box_move(Box* self) {
	if(self->isFalling == true) {
		self->speed.x = 0;
		self->speed.y = 6;
	}
	self->rec.x += self->speed.x;
	self->rec.y += self->speed.y;


}
void Box_collisionWithBorder(Box* self, float windowSizeX) {
	if(self->rec.x < 0 || self->rec.x + self->rec.width > windowSizeX) {
		self->speed.x *= -1.0f;
	}
}
Box_addToBottom(Box* self, Box* bottombox) {

}

int initTowerBuilder(App* self) {
	enum GAME_STATE gameState = PLAY;
	TBGame tB;
	tB.appPtr = self;
	TBGame_init(&tB);

	while(!WindowShouldClose()) {
		//Update
		if(gameState != GAMEOVER) {

			if(IsKeyPressed(KEY_SPACE)) {
				if(gameState == PAUSE) {
					gameState = PLAY;
				} else {
					gameState = PAUSE;
				}
			}

			if(gameState != PAUSE) {
				if(IsKeyPressed(KEY_F)) {
					tB.mainBox.isFalling = true;
				}
				Box_move(&tB.mainBox);
				Box_collisionWithBorder(&tB.mainBox, tB.appPtr->windowSize.x);


				//BoxPlacement after Collision w/ startBox
				for(int i = 0; i < MAX_BOXES; i++) {
					if(CheckCollisionRecs(tB.mainBox.rec, tB.bottomBoxes[i].rec)) {
						if(tB.bottomBoxes[i].isSet == false) {
							Box_addToBottom(&tB.mainBox, &tB.bottomBoxes[i]);

							tB.bottomBoxes[i + 1] = tB.mainBox;
							tB.bottomBoxes[i].isSet = true;

							tB.mainBox.isFalling = false;

							//Speed Increase
							tB.mainBox.speed.x *= 1.2;

							//Randomise MovingBox locationes
							int upper = GetScreenWidth() - tB.mainBox.rec.width;
							int lower = tB.mainBox.rec.width;
							tB.mainBox.rec.x = rand() % (upper - lower + 1) + lower;

							//Increase startposition
							tB.startPosition.y -= tB.mainBox.rec.height;

							//Reset of falling Box
							tB.mainBox.rec.y = tB.startPosition.y;
							tB.mainBox.isFalling = false;
							tB.mainBox.speed.x = 4;
							tB.mainBox.speed.y = 0;

							//ScoreCounter
							tB.score += 1;

							//if(score >= 4){
							//	for(int i = 0;)


							//}

						} else {
							gameState = GAMEOVER;
						}
					}
				}
				//Collision with floor
				if(tB.mainBox.rec.y + tB.mainBox.rec.height > self->windowSize.y) {
					gameState = GAMEOVER;
				}
			}

		}


		// Draw
		BeginDrawing();
		ClearBackground(RAYWHITE);

		//DrawTower Boxes 
		DrawRectangleRec(tB.mainBox.rec, tB.mainBox.color);
		for(int i = 0; i < MAX_BOXES; i++) {
			DrawRectangleRec(tB.bottomBoxes[i].rec, tB.bottomBoxes[i].color);
		}

		if(gameState == GAMEOVER) {
			DrawText("GAMEOVER", 400, 400, 60, BLUE);
		}

		//ScoreDraw
		DrawText(FormatText("SCORE: %i", tB.score), 50, 50, 40, MAROON);

		DrawText("F to drop the Box", GetScreenWidth() - 200, 50, 40, MAROON);



		DrawFPS(10, 10);

		EndDrawing();
	}

	return 0;
}