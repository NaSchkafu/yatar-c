#include "MainWindow.h"
#include <windows.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int cmdShow)
{
    MainWindow wnd;
    wnd.messageLoop();
}

