#include <raylib/raylib.h>
#include "App.h"
#include "Button.h"


int main(){
	App app;
	app.name = "RayING";
	app.width = 800;
	app.height = 800;
	App_init(&app);
	App_run(&app);
}