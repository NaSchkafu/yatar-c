//
// Created by Fluxy on 16.06.2016.
//

#ifndef YATAR_C_MAINWINDOW_H
#define YATAR_C_MAINWINDOW_H

#include <windows.h>
#include "AppBar.h"

class MainWindow : public AppBar {
public:
    MainWindow();
    int messageLoop();

};


#endif //YATAR_C_MAINWINDOW_H
