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
	Cards selCard; //selected Card
	Cards comCard; //comparison card
	Vector2 ratio;
	int framesCounter;
} MemoryGame;

//randomizer zur platzierung von farben im array
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
	//wird 2x durchgeführt weil wir jede farbe doppelt auf dem spielfeld brauchen
}

//konvertiere den namen der Farbe in ein RGB-Struct. evtl später notewendig zum überprüfen der farben.
void convertToRGB(MemoryGame *self)
{
	int ColorToInt(Color color);
	Color GetColor(int hexValue);
}

//es muss überprüft werden, zwei ausgewählte karten die gleiche farbe haben.
int compareColor(Color a, Color b)
{
	if (a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a)
		return 1;
	else
		return 0;
}

/*
void checkColor(MemoryGame *g)
{
	if (compareColor(a,b))

}*/

void MemoryGame_init(MemoryGame* self) 
{

	SetTargetFPS(60);
	self->running = true;
	self->gameState = MAINMENU;
	self->framesCounter = 0;


	self->windowSize = self->appPtr->windowSize;
	self->ratio = self->appPtr->r;
}

void updateGame_Easy(MemoryGame* self)
{
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
	randClrPlacement(self); //generiert zufällig 8 farben und platziert diese auf dem spielfeld
}

// auflistung der game-states, die wir verwenden werden
void State_GameOver(MemoryGame* self);
void State_Config(MemoryGame* self);
void State_Pause(MemoryGame* self);
void State_Play(MemoryGame* self);
void State_MainMenu(MemoryGame* self);

// initialisiere variablen für Buttongröße etc., damit wir sie in unserem Menü verwenden könenn
void Button_init(Button* self, char* name, float recX, float recY, float recW, float recH, float fontSize, Vector2 ratio) 
{
	self->name = name;

	self->rec.x = recX;
	self->rec.y = recY;
	self->rec.width = recW;
	self->rec.height = recH;

	self->fontSize = fontSize;
	self->ratio = ratio;
	self->isClicked = false;
}

// vorgang wenn man die anwendung "memory" startet:
void State_MainMenu(MemoryGame* self)
{
	Vector2 r = self->ratio;
	float fontSize = 75.0;

	//es wird der titel des spiel angezeigt.
	Button titleButton;
	{
		float widthOffset = 50.0;
		Button_init(&titleButton,
			"MEMORY",
			widthOffset * r.x,
			0,
			MeasureText("MEMORY", (int)(fontSize * r.y)),
			fontSize * r.y,
			fontSize * r.y,
			r
		);
	}

	//wir erzeugen buttons mit verschiedenen optionen:
	enum N { numButton = 5 };
	Button buttons[numButton];
	{
		char* menuNames[numButton] = { "Play", "Options", "Back to Selection", "Back to Main Menu", "Quit the Game" };
		float widthOffset2 = 70;
		for (int i = 0; i < numButton; i++) 
		{
			Button_init(&buttons[i],
				menuNames[i],
				widthOffset2 * r.x,
				fontSize * r.y + (fontSize + 10) * r.y * i,
				(float)MeasureText(menuNames[i], fontSize) * r.x,
				fontSize * r.y,
				fontSize * r.y,
				r
			);
		}
	}

	while (self->running && !WindowShouldClose()) 
	{
		// Update
		{
			for (int i = 0; i < numButton; i++) {
				Vector2 mousepoint = GetMousePosition();
				if (CheckCollisionPointRec(mousepoint, buttons[i].rec)) {
					if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
						buttons[i].isClicked = true;
					}
				}
				if (buttons[i].isClicked == true) {
					switch (i) {
					case 0: State_Play(self); buttons[i].isClicked = false; break;
					case 1: buttons[i].isClicked = false;  break;
					case 2: self->running = 0; break;
					case 3: self->running = 0; self->appPtr->gameState = MAINMENU; break;
					case 4: self->running = 0; self->appPtr->running = 0; self->appPtr->gameState = QUIT; break;
					}
				}
			}
		}
		// Draw
		{
			BeginDrawing();
			ClearBackground(RAYWHITE);
			int x = 400;
			int y = 400;
			int w = 30;
			int h = 30;
			typedef struct Rec {
				int x;
				int y;
				int w;
				int h;
			} Rec;
			Rec rec = { 400, 400, 30 ,30 };
			DrawRectangle(rec.x, rec.y, rec.w, rec.h, RED);
			DrawRectangle(x, y, w, h, RED);

			Button_draw(&titleButton);
			for (int i = 0; i < numButton; i++) {
				Button_drawRec(&buttons[i]);
			}
			EndDrawing();
		}
	}

}

// vorgang, wenn das spiel gestartet wird:
void State_Play(MemoryGame* self)
{
	MemoryGame memoryGame;
	memoryGame.appPtr = self;

	Vector2 r = self->ratio;
	float fontSize = 75.0;
	float widthOffset = 50.0;

	Vector2 mousepos = GetMousePosition();
	int xpos = 100;
	int ypos = 300;
	int recWidth = 400;
	int recHeight = 600;
	int num = 8;


	self->gameState = PLAY;
	bool playState = true;

	while (playState && !WindowShouldClose()) 
	{
		//Update
		{
			if (self->gameState != GAMEOVER && self->gameState != QUIT)
			{
				if (IsKeyReleased(KEY_B)) 
				{
					self->gameState = MAINMENU; //wird der B-Knopf gedrückt kommen wir zurück ins hauptmenü
					playState = false;
				}
				if (IsKeyReleased(KEY_SPACE))
				{
					self->gameState = PAUSE; //wenn wir die leertaste drücken wird das spiel pausiert.
				}
				if (self->gameState == PAUSE) {
					State_Pause(self);
				}

				
				//zeichne ein array aus karten:
				void updateGame_Easy(MemoryGame * self);

				for (int i = 0; i < 8; i++)
				{
					for (int j = 0; j < 2; j++)
					{

						Rectangle rec;
						rec.x = memoryGame.cards[i][j].position.x + (recWidth + 10) * i;
						rec.y = memoryGame.cards[i][j].position.y + (recHeight + 10) * j;
						rec.width = recWidth;
						rec.height = recHeight;
						Color w = RAYWHITE;

						if (CheckCollisionPointRec(mousepos, rec))
						{
							if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
							{
								if (compareColor(memoryGame.cards[i][j].color, w))
								{
									memoryGame.cards[i][j].color = w;
								}
								else
								{
									if (compareColor(memoryGame.cards[i][j].color, memoryGame.cards[i][j].initalized.color))
									{
										memoryGame.cards[i][j].color = memoryGame.selCard[i][j].color;
										memoryGame.selCard[i][j].color = memoryGame.selCard[i][j].hiddenColor;
									}
									else
									{
										if (compareColor(memoryGame.cards[i][j].hiddenColor, memoryGame.selCard[i][j].hiddenColor))
										{
											//begone, cards.
											memoryGame.cards[i][j]->color = RAYWHITE;
											memoryGame.selCard[i][j].color = RAYWHITE;
										}
										else
										{
											//cards turn back to normal.
										}
									}
								}
							}
						}
					}
				}
			}
			if (self->gameState == GAMEOVER) {
				State_GameOver(self);
			}
			if (self->gameState == QUIT) {
				return;
			}
		}

		//draw:

		BeginDrawing();

			ClearBackground(RAYWHITE);

		// erstelle ein array mit (in unserem Fall 16) rechtecken
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

void State_Pause(MemoryGame* self)
{
	Vector2 r = self->ratio;
	float fontSize = 75.0;
	float widthOffset = 50.0;

	enum N { numButton = 3 };
	Button buttons[numButton];
	{
		char* menuNames[numButton] = { "Continue", "Options", "Quit" };
		float widthOffset2 = 70;
		for (int i = 0; i < numButton; i++) {
			Button_init(&buttons[i],
				menuNames[i],
				widthOffset2 * r.x,
				fontSize * r.y + (fontSize + 10) * r.y * i,
				(float)MeasureText(menuNames[i], fontSize) * r.x,
				fontSize * r.y,
				fontSize * r.y,
				r
			);
		}
	}
	bool pauseState = true;


	while (pauseState && !WindowShouldClose())
	{
		//Update
		{
			for (int i = 0; i < numButton; i++)
			{
				Vector2 mousepoint = GetMousePosition();
				if (CheckCollisionPointRec(mousepoint, buttons[i].rec))
				{
					if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
					{
						buttons[i].isClicked = true;
					}
				}
				if (buttons[i].isClicked == true)
				{
					buttons[i].isClicked = false;
					switch (i)
					{
					case 0: pauseState = false; self->gameState = PLAY;  break;
					case 1: State_Config(self); break;
					case 2: pauseState = false; self->gameState = QUIT; break;
					}
				}
			}
		}

		//Draw
		{
			BeginDrawing();
			ClearBackground(RAYWHITE);

			DrawRectangle(0, 0, self->windowSize.x, self->windowSize.y, Fade(GRAY, 0.5f));
			DrawText("Game Paused", self->windowSize.x / 2 * r.x, 400 * r.y, 75.0 * r.y, RED);
			for (int i = 0; i < numButton; i++)
			{
				Button_drawRec(&buttons[i]);
			}

			EndDrawing();
		}
	}
}

void State_Config(MemoryGame* self)
{
	Vector2 r = self->ratio;
	float fontSize = 75.0;
	float widthOffset = 50.0;

	enum N { numButton = 4 };
	Button buttons[numButton];
	{
		char* menuNames[numButton] = { "SFX", "Difficulty", "Theme", "Go Back" };
		float widthOffset2 = 70;
		for (int i = 0; i < numButton; i++) 
		{
			Button_init(&buttons[i],
				menuNames[i],
				widthOffset * r.x,
				fontSize * r.y + (fontSize + 10) * r.y * i,
				(float)MeasureText(menuNames[i], fontSize) * r.x,
				fontSize * r.y,
				fontSize * r.y,
				r
			);
		}
	}

	typedef struct Circle 
	{
		Vector2 position;
		float radius;
		Color color;
	} Circle;
	enum { numCircle = 3 };
	Circle sfxCircle[numCircle];
	{
		Color sfxColors[2] = { RED, GREEN };
		for (int i = 0; i < numCircle; i++)
		{
			sfxCircle[i].radius = (fontSize * r.y) / 2;
			sfxCircle[i].position.x = sfxCircle[i].radius + widthOffset * r.x + (float)MeasureText(buttons[0].name, fontSize) * r.x;
			sfxCircle[i].position.y = -sfxCircle[i].radius + fontSize * r.y + (fontSize + 10) * r.y * 0 + fontSize * r.y;
			sfxCircle[i].position.x += i * sfxCircle[i].radius * 2 + 40 * r.x;
			sfxCircle[i].color = sfxColors[i];
		}
	}

	Circle difficultyCircle[numCircle];
	{
		Color difficultyColors[3] = { GREEN, YELLOW, RED };
		for (int i = 0; i < numCircle; i++)
		{
			difficultyCircle[i].radius = (fontSize * r.y) / 2;
			difficultyCircle[i].position.x = difficultyCircle[i].radius + widthOffset * r.x + (float)MeasureText(buttons[0].name, fontSize) * r.x;
			difficultyCircle[i].position.y = -difficultyCircle[i].radius + fontSize * r.y + (fontSize + 10) * r.y * 0 + fontSize * r.y;
			difficultyCircle[i].position.x += i * difficultyCircle[i].radius * 2 + 40 * r.x;
			difficultyCircle[i].color = difficultyColors[i];
		}
	}

	Circle themeCircle[numCircle];
	{
		Color themeColors[3] = { BLUE, PINK, GOLD };
		for (int i = 0; i < numCircle; i++)
		{
			themeCircle[i].radius = (fontSize * r.y) / 2;
			themeCircle[i].position.x = themeCircle[i].radius + widthOffset * r.x + (float)MeasureText(buttons[0].name, fontSize) * r.x;
			themeCircle[i].position.y = -themeCircle[i].radius + fontSize * r.y + (fontSize + 10) * r.y * 0 + fontSize * r.y;
			themeCircle[i].position.x += i * themeCircle[i].radius * 2 + 40 * r.x;
			themeCircle[i].color = themeColors[i];
		}
	}

	bool optionState = true;
	while (optionState && !WindowShouldClose()) 
	{
		//Update
		{
			Vector2 mousepoint = GetMousePosition();

			for (int i = 0; i < numButton; i++)
			{
				Vector2 mousepoint = GetMousePosition();
				if (CheckCollisionPointRec(mousepoint, buttons[i].rec))
				{
					if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
					{
						buttons[i].isClicked = true;
					}
				}
				if (buttons[i].isClicked == true)
				{
					buttons[i].isClicked = false;
					switch (i)
					{
					case 0: break;
					case 1: break;
					case 2: break;
					case 3: return; break;
					}
				}
			}
			for (int i = 0; i < numCircle; i++) 
			{
				if (CheckCollisionPointCircle(mousepoint, sfxCircle[i].position, sfxCircle[i].radius)) 
				{
					if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
					{
						// sfx_on or sfx_off
					}
				}
				if (CheckCollisionPointCircle(mousepoint, difficultyCircle[i].position, difficultyCircle[i].radius)) 
				{
					if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
					{
						// switch to gameState: State_Easy, State_Medium, State_Hard
					}
				}
				if (CheckCollisionPointCircle(mousepoint, themeCircle[i].position, themeCircle[i].radius)) 
				{
					if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
					{
						// change theme to: Theme_Standard, Theme_Animal, Theme_Surprise. Theme_Animal and Theme_Surprise will contain textures, which will be added sometime in the future.
					}
				}
			}
		}
		//Draw
		{
			BeginDrawing();
			ClearBackground(RAYWHITE);

			DrawRectangle(0, 0, self->windowSize.x, self->windowSize.y, Fade(GRAY, 0.5f));
			DrawText("Game Options", self->windowSize.x / 2 * r.x, 400 * r.y, 75.0 * r.y, RED);

			for (int i = 0; i < numButton; i++) 
			{
				Button_draw(&buttons[i]);
			}
			for (int i = 0; i < numCircle; i++) 
			{
				DrawCircle(sfxCircle[i].position.x, sfxCircle[i].position.y, sfxCircle[i].radius, sfxCircle[i].color);
				DrawCircle(difficultyCircle[i].position.x, difficultyCircle[i].position.y, difficultyCircle[i].radius, difficultyCircle[i].color);
				DrawCircle(themeCircle[i].position.x, themeCircle[i].position.y, themeCircle[i].radius, themeCircle[i].color);
			}
			EndDrawing();
		}
	}
}

void State_GameOver(MemoryGame* self)
{
	Vector2 r = self->ratio;
	float fontSize = 75.0;
	float widthOffset = 50.0;

	enum N { numButton = 2 };
	Button buttons[numButton];
	{
		char* menuNames[numButton] = { "Try Again", "Quit" };
		float widthOffset2 = 70;
		for (int i = 0; i < numButton; i++) {
			Button_init(&buttons[i],
				menuNames[i],
				widthOffset * r.x,
				fontSize * r.y + (fontSize + 10) * r.y * i,
				(float)MeasureText(menuNames[i], fontSize) * r.x,
				fontSize * r.y,
				fontSize * r.y,
				r
			);
		}
	}

	bool gameoverState = true;
	while (gameoverState && !WindowShouldClose()) {
		//Update
		{
			for (int i = 0; i < numButton; i++) {
				Vector2 mousepoint = GetMousePosition();
				if (CheckCollisionPointRec(mousepoint, buttons[i].rec)) {
					if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
						buttons[i].isClicked = true;
					}
				}
				if (buttons[i].isClicked == true) {
					buttons[i].isClicked = false;
					switch (i) {
					case 0: MemoryGame_init(self); return;  break;
					case 1: self->gameState = QUIT; return; break;
					}
				}
			}
		}
		//Draw
		{
			BeginDrawing();
			ClearBackground(RAYWHITE);

			DrawText("Game Over", self->windowSize.x / 2 * r.x, 400 * r.y, 75.0 * r.y, RED);

			for (int i = 0; i < numButton; i++) {
				Button_drawRec(&buttons[i]);
			}
			EndDrawing();
		}
	}
}

void main(App* self)
{
	MemoryGame memoryGame;
	memoryGame.appPtr = self;

	MemoryGame_init(&memoryGame);
	State_MainMenu(&memoryGame);
}