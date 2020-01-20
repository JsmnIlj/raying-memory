#include <raylib/raylib.h>
#include "App.h"
#include "Button.h"
#include "WinHelper.h"

int main(){
	int monitorWidth = getMonitorWidth();
	int monitorHeight = getMonitorHeight();
	App app;
	app.name = "RayING";
	app.windowSize.x = monitorWidth  - (monitorWidth  / 10);
	app.windowSize.y = monitorHeight - (monitorHeight / 10);

	App_init(&app);
	App_run(&app);
}