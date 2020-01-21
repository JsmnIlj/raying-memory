#include "App_Memory.h"
#include "raylib/raylib.h"

enum GAME_STATE {
	PLAY,
	PAUSE,
	QUIT
};
typedef struct Card {
	Vector2 position;
	Color color;
} Card;
typedef struct MemoryGame {
	int running;
	enum GAME_STATE gamestate;
	Vector2 windowSize;

	Card cards[16];

} MemoryGame;


void MemoryGame_init(MemoryGame* self) {
	self->running = 1;
	self->gamestate = PLAY;
	int xpos = 100;
	int ypos = 300;
	for (int i = 0; i < 8; i++) // i = columns
	{
		for (int j = 0; j < 2; j++) // j = rows
		{
			self->cards[i + j].position = (Vector2){ xpos, ypos };
			self->cards[i + j].color = BLACK;
		}
	}
}

void update() {

}
void draw(MemoryGame* self) {
	BeginDrawing();

	ClearBackground(RAYWHITE);

	int xpos = 100;
	int ypos = 300;
	int xrec = 400;
	int yrec = 600;

	Vector2 mousepos = GetMousePosition();
	//Erstelle ein Array mit jeweils 16, 36 und 64 Rechtecken. (Das ist das jeweilige Spielfeld)

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 2; j++)
		{

			if (CheckCollisionPointRec(mousepos, (Rectangle) { self->cards[i + j].position.x + (xrec + 10) * i, self->cards[i + j].position.y + (yrec + 10) * j, xrec, yrec }))
			{
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					self->cards[i + j].color = BLUE;
				}
// TODO: Es soll sich nur das Rechteck verändern, das auch wirklich angeklickt wurde. 
			}
			DrawRectangle(self->cards[i + j].position.x + (xrec + 10) * i, self->cards[i + j].position.y + (yrec + 10) * j, xrec, yrec, self->cards[i + j].color);
			
		}

	}
	EndDrawing();
}

/*
int xpos = 200;
int ypos = 100;
int xrec = 250;
int yrec = 450;
Card card[36];
for (int i = 0; i < 6; i++)
{
	for (int j = 0; j < 6; j++)
	{
		card[i + j].position = (Vector2){ xpos, ypos };
	}
}
for (int i = 0; i < 6; i++)
{
	for (int j = 0; j < 6; j++)
	{
		DrawRectangle(card[i + j].position.x + (xrec+ 10)*i, card[i + j].position.y + (yrec+10)*j, xrec,yrec, BLACK);
	}
}
*/



void initMemoryGame(App* self)
{
	MemoryGame memoryGame;
	MemoryGame_init(&memoryGame);



	while (memoryGame.running && !WindowShouldClose()) {
		update();
		draw(&memoryGame);
	}

	return 0;
}

