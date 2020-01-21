#include "WinHelper.h"
#include "Windows.h"

int getMonitorWidth(){
	return GetSystemMetrics(SM_CXSCREEN);
	
}
int getMonitorHeight(){
	return GetSystemMetrics(SM_CYSCREEN);
}