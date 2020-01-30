#include "App_Memory.h"
#include "raylib/raylib.h"
#include "../Button.h"
#include "../App.h"
#include <time.h>
#include <stdlib.h>

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
	Color initColor;
	char initalized;
	bool isTurned;
} Cards;

typedef struct MemoryGame
{
	int running;
	enum GAME_STATE gameState;
	Vector2 windowSize;
	App* appPtr;
	Cards cards[8][2];
	Vector2 ratio;
	int framesCounter;
} MemoryGame;

//randomizer zur platzierung von farben im array
void randClrPlacement(MemoryGame* g)
{
	srand(time(NULL));
	Color colors[8] = { YELLOW, ORANGE, GREEN, PURPLE, LIME, BLUE, RED, PINK };

	for (int i = 0; i < 8; i++)
	{
		int a, b;
		do {
			a = rand() % 8;
			b = rand() % 2;
		} while (g->cards[a][b].initalized);

		g->cards[a][b].initalized = 1;
		g->cards[a][b].hiddenColor = g->cards[a][b].initColor;
		g->cards[a][b].initColor = colors[i];

		do {
			a = rand() % 8;
			b = rand() % 2;
		} while (g->cards[a][b].initalized);

		g->cards[a][b].initalized = 1;
		g->cards[a][b].hiddenColor = g->cards[a][b].initColor;
		g->cards[a][b].initColor = colors[i];
	}
	//wird 2x durchgeführt weil wir jede farbe doppelt auf dem spielfeld brauchen
}

//es muss überprüft werden, ob zwei ausgewählte karten die gleiche farbe haben.
int compareColor(const Color a, const Color b)
{
	return (a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a);
}

void MemoryGame_init(MemoryGame* self)
{
	SetTargetFPS(60);
	self->running = true;
	self->gameState = MAINMENU;
	self->framesCounter = 0;
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
			self->cards[i][j].isTurned = false;
		}
	}

	randClrPlacement(self); //generiert zufällig 8 farben und platziert diese auf dem spielfeld
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
void updateGame_Medium(MemoryGame* self)
{
	int xpos = 100;
	int ypos = 300;
	for (int i = 0; i < 6; i++) // i = columns
	{
		for (int j = 0; j < 4; j++) // j = rows
		{
			self->cards[i][j].position = (Vector2){ xpos, ypos };
			self->cards[i][j].color = BLACK;
			self->cards[i][j].initalized = 0;
		}
	}
	randClrPlacement(self); //generiert zufällig 8 farben und platziert diese auf dem spielfeld
}

// auflistung der game-states, die wir verwenden werden
void memory_State_GameOver(MemoryGame* self);
void memory_State_Config(MemoryGame* self);
void memory_State_Pause(MemoryGame* self);
void memory_State_Play(MemoryGame* self);
void memory_State_MainMenu(MemoryGame* self);

// initialisiere variablen für Buttongröße etc., damit wir sie in unserem Menü verwenden könenn
void memory_Button_init(Button* self, char* name, float recX, float recY, float recW, float recH, float fontSize, Vector2 ratio)
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
void memory_State_MainMenu(MemoryGame* self)
{
	Vector2 r = self->ratio;
	float fontSize = 100.0;

	//es wird der titel des spiel angezeigt.
	Button titleButton;
	{
		float widthOffset = 400.0;
		memory_Button_init(&titleButton,
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
	float menuFontSize = 75.0;
	{
		char* menuNames[numButton] = { "Play", "Options", "Back to Selection", "Back to Main Menu", "Quit the Game" };
		float widthOffset2 = 100;
		for (int i = 0; i < numButton; i++) {
			memory_Button_init(&buttons[i],
				menuNames[i],
				widthOffset2 * r.x,
				menuFontSize * r.y + (menuFontSize + 10) * r.y * i,
				(float)MeasureText(menuNames[i], menuFontSize) * r.x,
				menuFontSize * r.y,
				menuFontSize * r.y,
				r
			);


		}
	}

	while (self->running && !WindowShouldClose())
	{
		// Update
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
					switch (i)
					{
					case 0: memory_State_Play(self); buttons[i].isClicked = false; break;
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
			Button_draw(&titleButton);

			for (int i = 0; i < numButton; i++)
			{
				Button_drawRec(&buttons[i]);
			}

			EndDrawing();
		}
	}
}

// vorgang, wenn das spiel gestartet wird:
void memory_State_Play(MemoryGame* self)
{
	int recWidth = 400;
	int recHeight = 600;
	self->gameState = PLAY;
	bool playState = true;
	updateGame_Easy(self);
	Cards* prevCard = NULL;
	Cards* nextCard = NULL;
	char x = 0;
	char isGameOver = 16;


	while (playState && !WindowShouldClose())
	{
		Vector2 mousepos = GetMousePosition();

		if (self->gameState == QUIT) {
			return;
		}

		if (self->gameState == GAMEOVER) {
			memory_State_GameOver(self);
		}
		else
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
				memory_State_Pause(self);
			}

			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 2; j++)
				{
					Rectangle rec;
					rec.x = self->cards[i][j].position.x + (recWidth + 10) * i;
					rec.y = self->cards[i][j].position.y + (recHeight + 10) * j;
					rec.width = recWidth;
					rec.height = recHeight;

					if (CheckCollisionPointRec(mousepos, rec))
					{
						if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
						{

							if (nextCard && prevCard)
							{
								if (compareColor(nextCard->color, prevCard->color))
								{
									prevCard->color = RAYWHITE;
									nextCard->color = RAYWHITE;
									isGameOver -= 2;
								}
								else
								{
									prevCard->color = BLACK;
									nextCard->color = BLACK;
								}
								nextCard = NULL;
								prevCard = NULL;
							}

							if (!x)
							{
								prevCard = &self->cards[i][j];
								x++;
							}
							else
							{
								nextCard = &self->cards[i][j];
								x--;
							}

							if (!isGameOver)
								self->gameState = GAMEOVER;

							if (!compareColor(self->cards[i][j].color, RAYWHITE))
								self->cards[i][j].color = self->cards[i][j].hiddenColor;
						}
					}
				}
			}
		}

		/* Drawing. */
		BeginDrawing();
		ClearBackground(RAYWHITE);

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				int x = self->cards[i][j].position.x + (recWidth + 10) * i;
				int y = self->cards[i][j].position.y + (recHeight + 10) * j;
		
				if (self->cards[i][j].isTurned)
				{
					DrawRectangle(x, y, recWidth, recHeight, self->cards[i][j].hiddenColor);
				}
				else
				{
					DrawRectangle(x, y, recWidth, recHeight, self->cards[i][j].color);
				}
			}
		}

		EndDrawing();
	}
}

void memory_State_Pause(MemoryGame* self)
{
	Vector2 r = self->ratio;
	bool pauseState = true;
	float fontSize = 100;
	float widthOffset2 = 100;
	Button buttons[3] = { 0 };
	char* menuNames[3] = { "Continue", "Options", "Quit" };

	for (int i = 0; i < 3; i++)
	{
		memory_Button_init(&buttons[i],
			menuNames[i],
			widthOffset2 * r.x,
			fontSize * r.y + (fontSize + 10) * r.y * i,
			(float)MeasureText(menuNames[i], fontSize) * r.x,
			fontSize * r.y,
			fontSize * r.y,
			r
		);
	}

	while (pauseState && !WindowShouldClose())
	{
		//Update
		for (int i = 0; i < 3; i++)
		{
			Vector2 mousepoint = GetMousePosition();
			if (CheckCollisionPointRec(mousepoint, buttons[i].rec))
			{
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
				{
					buttons[i].isClicked = true;
				}
			}
			if (buttons[i].isClicked)
			{
				buttons[i].isClicked = false;
				switch (i)
				{
				case 0: pauseState = false; self->gameState = PLAY;  break;
				case 1: memory_State_Config(self); break;
				case 2: pauseState = false; self->gameState = QUIT; break;
				}
			}

		}

		//Draw
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawRectangle(0, 0, self->windowSize.x, self->windowSize.y, Fade(GRAY, 0.5f));
		DrawText("Game Paused", self->windowSize.x / 2 * r.x, 400 * r.y, 75.0 * r.y, RED);

		for (int i = 0; i < 3; i++)
		{
			Button_drawRec(&buttons[i]);
		}

		EndDrawing();
	}
}

void memory_State_Config(MemoryGame* self)
{
	Vector2 r = self->ratio;
	float fontSize = 75.0;
	float widthOffset = 50.0;
	Button buttons[4];
	char* menuNames[4] = { "SFX", "Difficulty", "Theme", "Go Back" };

	for (int i = 0; i < 4; i++)
	{
		memory_Button_init(&buttons[i],
			menuNames[i],
			widthOffset * r.x,
			fontSize * r.y + (fontSize + 10) * r.y * i,
			(float)MeasureText(menuNames[i], fontSize) * r.x,
			fontSize * r.y,
			fontSize * r.y,
			r
		);

	}

	typedef struct Circle
	{
		Vector2 position;
		float radius;
		Color color;
	} Circle;

	Circle sfxCircle[3];
	Circle themeCircle[3];
	Circle difficultyCircle[3];
	Color sfxColors[2] = { RED, GREEN };
	Color difficultyColors[3] = { GREEN, YELLOW, RED };
	Color themeColors[3] = { BLUE, PINK, GOLD };

	for (int i = 0; i < 3; i++)
	{
		sfxCircle[i].radius = (fontSize * r.y) / 2;
		sfxCircle[i].position.x = sfxCircle[i].radius + widthOffset * r.x + (float)MeasureText(buttons[0].name, fontSize) * r.x;
		sfxCircle[i].position.y = -sfxCircle[i].radius + fontSize * r.y + (fontSize + 10) * r.y * 0 + fontSize * r.y;
		sfxCircle[i].position.x += i * sfxCircle[i].radius * 2 + 40 * r.x;
		sfxCircle[i].color = sfxColors[i % 2];

		difficultyCircle[i].radius = (fontSize * r.y) / 2;
		difficultyCircle[i].position.x = difficultyCircle[i].radius + widthOffset * r.x + (float)MeasureText(buttons[0].name, fontSize) * r.x;
		difficultyCircle[i].position.y = -difficultyCircle[i].radius + fontSize * r.y + (fontSize + 10) * r.y * 0 + fontSize * r.y;
		difficultyCircle[i].position.x += i * difficultyCircle[i].radius * 2 + 40 * r.x;
		difficultyCircle[i].color = difficultyColors[i];

		themeCircle[i].radius = (fontSize * r.y) / 2;
		themeCircle[i].position.x = themeCircle[i].radius + widthOffset * r.x + (float)MeasureText(buttons[0].name, fontSize) * r.x;
		themeCircle[i].position.y = -themeCircle[i].radius + fontSize * r.y + (fontSize + 10) * r.y * 0 + fontSize * r.y;
		themeCircle[i].position.x += i * themeCircle[i].radius * 2 + 40 * r.x;
		themeCircle[i].color = themeColors[i];
	}


	bool optionState = true;

	while (optionState && !WindowShouldClose())
	{
		//Update
		{
			Vector2 mousepoint = GetMousePosition();

			for (int i = 0; i < 4; i++)
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
					case 3: return;
					}
				}
			}
			for (int i = 0; i < 3; i++)
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

			for (int i = 0; i < 4; i++)
			{
				Button_draw(&buttons[i]);
			}
			for (int i = 0; i < 3; i++)
			{
				DrawCircle(sfxCircle[i].position.x, sfxCircle[i].position.y, sfxCircle[i].radius, sfxCircle[i].color);
				DrawCircle(difficultyCircle[i].position.x, difficultyCircle[i].position.y, difficultyCircle[i].radius, difficultyCircle[i].color);
				DrawCircle(themeCircle[i].position.x, themeCircle[i].position.y, themeCircle[i].radius, themeCircle[i].color);
			}
			EndDrawing();
		}
	}
}

void memory_State_GameOver(MemoryGame* self)
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
			memory_Button_init(&buttons[i],
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

void init_memoryGame_main(App* self)
{
	MemoryGame memoryGame;
	memoryGame.appPtr = self;
	MemoryGame_init(&memoryGame);
	memory_State_MainMenu(&memoryGame);
}