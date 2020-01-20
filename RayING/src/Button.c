#include "Button.h"

void Button_draw(Button* self){
	Vector2 r = self->ratio;

	Vector2 pos = { self->rec.x, self->rec.y };
	DrawText(self->name, pos.x, pos.y, self->fontSize, RED);
}
void Button_drawRec(Button* self){
	Vector2 r = self->ratio;

	Rectangle rec = { self->rec.x , self->rec.y,  self->rec.width + 30.0f * r.x, self->rec.height };

	Vector2 mousepoint = GetMousePosition();
	float col;
	float linewidth;
	if(CheckCollisionPointRec(mousepoint, rec)){
		col = 0.5f;
		linewidth = 3.0;
	} else{
		col = 0.1f;
		linewidth = 1.0;
	}

	DrawRectangleRec(rec, Fade(GRAY, col));
	DrawRectangleLinesEx(rec, linewidth, GRAY);
	DrawText(self->name, rec.x + 15.0f * r.x, rec.y, self->fontSize, RED);
}