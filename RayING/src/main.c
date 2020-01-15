#include <raylib/raylib.h>
#include "App.h"
#include "Button.h"


int main(){
	App app;
	app.name = "RayING";
	app.windowSize.x = 800;
	app.windowSize.y = 800;

	//app.windowSize.x = 1920-100;
	//app.windowSize.y = 1080-100;

	App_init(&app);
	App_run(&app);
}