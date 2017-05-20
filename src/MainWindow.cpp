//
// Created by Fluxy on 16.06.2016.
//
#include <iostream>
#include "../inc/MainWindow.h"


MainWindow::MainWindow()
{

}

int MainWindow::messageLoop() {
    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0) > 0)
    {
        if (msg.message == WM_KEYUP && msg.wParam == VK_ESCAPE) {
            break;
        }

        // TODO WM_DISPLAYCHANGE (Desktop erweitert)
		if (msg.message == WM_TIMER && msg.wParam == 10)
		{
			update();
		}
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}










