#pragma once
#include <raylib/raylib.h>
typedef struct Button{
	char* name;
	int fontSize;
	Rectangle rec;
} Button;
void Button_draw(Button* self);
void Button_drawRec(Button* self);