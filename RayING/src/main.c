#include <raylib/raylib.h>
#include "App.h"
#include "Button.h"
#include "WinHelper.h"

int main(){
	int monitorWidth = getMonitorWidth();
	int monitorHeight = getMonitorHeight();
	App app;
	app.name = "RayING";
	app.windowSize.x = monitorWidth - 100;
	app.windowSize.y = monitorHeight - 100;

	app.windowSize.x = 800;
	app.windowSize.y = 800;

	App_init(&app);
	App_run(&app);
}