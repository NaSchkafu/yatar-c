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
  void registerKeys();

private:
  UINT m_shellHookId;
  static const auto S_CHANGE_WIN_HOTKEY = 1;
};


#endif //YATAR_C_MAINWINDOW_H
