#pragma once
#include <raylib/raylib.h>
typedef struct Button{
	char* name;
	float fontSize;
	Rectangle rec;
	Vector2 ratio;
	bool isClicked;
} Button;
void Button_draw(Button* self);
void Button_drawRec(Button* self);