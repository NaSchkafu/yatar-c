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
  virtual ~MainWindow();
  int messageLoop();

private:
  HHOOK m_hotkeyHook;
};


#endif //YATAR_C_MAINWINDOW_H
