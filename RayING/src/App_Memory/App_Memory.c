#include "App_Memory.h"
#include "raylib/raylib.h"
#include "../Button.h"
#include "../App.h"
#include <time.h>
#include <stdlib.h>
#include <string.h.>

//definieren von Game States
enum GAME_STATE
{
	MAINMENU,
	PLAY,
	CONFIG,
	PAUSE,
	GAMEOVER,
	QUIT
};

//definiere variablen für die erstellung von karten
typedef struct Cards
{
	Vector2 position;
	Color color;
	Color hiddenColor;
	char initalized;
} Cards;

typedef struct MemoryGame
{
	int running;
	enum GAME_STATE gameState;
	Vector2 windowSize;
	App* appPtr;
	Cards cards[8][2];
	Vector2 ratio;
} MemoryGame;


void randClrPlacement(MemoryGame *g)
{
	srand(time(NULL));
	Color colors[8] = { YELLOW, ORANGE, GREEN, PURPLE, LIME,
		BLUE, RED, PINK };

	for (int i = 0; i < 8; i++)
	{
		int a, b;
		do {
			a = rand() % 8;
			b = rand() % 2;
		} while (g->cards[a][b].initalized);

		g->cards[a][b].initalized = 1;
		g->cards[a][b].hiddenColor = colors[i];

		
		do {
			a = rand() % 8;
			b = rand() % 2;
		} while (g->cards[a][b].initalized);

		g->cards[a][b].initalized = 1;
		g->cards[a][b].hiddenColor = colors[i];
	}


}

void MemoryGame_init(MemoryGame* self) {

	SetTargetFPS(60);
	self->running = true;
	self->gameState = PLAY;


	self->windowSize = self->appPtr->windowSize;
	self->ratio = self->appPtr->r;

	int xpos = 100;
	int ypos = 300;
	for (int i = 0; i < 8; i++) // i = columns
	{
		for (int j = 0; j < 2; j++) // j = rows
		{
			self->cards[i][j].position = (Vector2){ xpos, ypos };
			self->cards[i][j].color = BLACK;
			self->cards[i][j].initalized = 0;
		}
	}
	randClrPlacement(self);
}


// Main game loop

void initMemoryGame(App* self)
{
	MemoryGame memoryGame;
	memoryGame.appPtr = self;
	MemoryGame_init(&memoryGame);

	while (memoryGame.running && !WindowShouldClose())
	{
		//update:

		Vector2 mousepos = GetMousePosition();
		int xpos = 100;
		int ypos = 300;
		int recWidth = 400;
		int recHeight = 600;
		int num = 8;

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 2; j++)
			{

				Rectangle rec;
				rec.x = memoryGame.cards[i][j].position.x + (recWidth + 10) * i;
				rec.y = memoryGame.cards[i][j].position.y + (recHeight + 10) * j;
				rec.width = recWidth;
				rec.height = recHeight;

				if (CheckCollisionPointRec(mousepos, rec))
				{
					if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
					{
				//		memoryGame.cards[i][j].color;
						memoryGame.cards[i][j].color = memoryGame.cards[i][j].hiddenColor;
					}
				}
			}
		}

		//draw:

		BeginDrawing();

			ClearBackground(RAYWHITE);

		//Erstelle ein Array mit jeweils 16, 36 und 64 Rechtecken. (Das ist das jeweilige Spielfeld)
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 2; j++) 
			{
				Rectangle rec;
				rec.x = memoryGame.cards[i][j].position.x + (recWidth + 10) * i;
				rec.y = memoryGame.cards[i][j].position.y + (recHeight + 10) * j;
				rec.width = recWidth;
				rec.height = recHeight;

				DrawRectangle(rec.x, rec.y, recWidth, recHeight, memoryGame.cards[i][j].color);
			}
		}

		EndDrawing();

	}

	return 0;
}
/*int main()
{
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		update();
		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		draw();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;


}*/