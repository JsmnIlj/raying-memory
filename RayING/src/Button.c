#include "Button.h"

void Button_draw(Button* self){
	self->rec.width = MeasureText(self->name, self->fontSize) + 10;
	self->rec.height = self->fontSize;

	Rectangle rec = { self->rec.x, self->rec.y,  self->rec.width + 10, self->rec.height };
	DrawTextRec(GetFontDefault(), self->name, rec, self->fontSize, self->fontSize / 10.0f, true, RED);
}
void Button_drawRec(Button* self){
	self->rec.width = MeasureText(self->name, self->fontSize) + 10;
	self->rec.height = self->fontSize;

	Rectangle rec = { self->rec.x, self->rec.y,  self->rec.width + 10, self->rec.height };


	Vector2 mousepoint = GetMousePosition();
	float col;
	float linewidth;
	if(CheckCollisionPointRec(mousepoint, rec)){
		col = 0.5f;
		linewidth = 3;
	} else{
		col = 0.1f;
		linewidth = 1;
	}

	DrawRectangleRec(rec, Fade(GRAY, col));
	DrawRectangleLinesEx(rec, linewidth, GRAY);
	DrawTextRec(GetFontDefault(), self->name, rec, self->fontSize, self->fontSize / 10.0f, true, RED);
}