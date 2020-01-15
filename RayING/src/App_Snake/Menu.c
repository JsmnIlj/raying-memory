#include "Menu.h"
#include "../Button.h"
#include "SnakeGame.h"
void Menu_drawGameoverMenu(SnakeGame* self){
	int fontSize = 40;
	int textPosX = self->windowSize.x / 2;
	int textPosY = self->windowSize.y / 2;

	DrawText("GAME OVER", textPosX, textPosY, fontSize, GRAY);


	Button repeatButton = { "Repeat", fontSize, {textPosX + 100, textPosY + 200, 1, 1} };
	Button_drawRec(&repeatButton);

	Button quitButton = { "Quit", fontSize, {textPosX - 100, textPosY + 200, 1, 1} };
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
void Menu_drawOptionsMenu(SnakeGame* self){
	DrawText("OPTIONS", self->windowSize.x / 10 * 1, self->windowSize.y / 10 * 1, 50, MAROON);
	DrawText("GAME PAUSED", self->appPtr->windowSize.x / 2 - MeasureText("GAME PAUSED", 40) / 2, self->appPtr->windowSize.y / 2 - 40, 40, GRAY);
}
