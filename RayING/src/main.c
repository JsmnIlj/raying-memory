#include <raylib/raylib.h>
#include "App.h"
#include "Button.h"

int main(){
	App app;
	app.name = "RayING";
	//app.windowSize.x = 800;
	//app.windowSize.y = 800;
	app.windowSize.x = 3840 - 200;
	app.windowSize.y = 2160 - 200;

	App_init(&app);
	App_run(&app);
}