#include <raylib/raylib.h>
#include "App.h"
#include "Button.h"
#define Artur

int main(){
	App app;
	app.name = "RayING";
	//app.windowSize.x = 800;
	//app.windowSize.y = 800;
#if defined Artur
	app.windowSize.x = 1920-100;
	app.windowSize.y = 1080-100;
#elif Jasmin
	app.windowSize.x = 1920 - 100;
	app.windowSize.y = 1080 - 100;
#elif William
	app.windowSize.x = 1920 - 100;
	app.windowSize.y = 1080 - 100;
#endif

	App_init(&app);
	App_run(&app);
}