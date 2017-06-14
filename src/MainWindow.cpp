//
// Created by Fluxy on 16.06.2016.
//
#include <iostream>
#include "../inc/MainWindow.h"
#include "ShellHook.h"


MainWindow::MainWindow()
{
  registerKeys();
}

void MainWindow::registerKeys()
{
  for (DWORD i = 0x31; i <= 0x39; i++) {
    Hotkey key;
    auto idx = i - 0x31;
    key.callback = [this, idx]() { this->switchToIdx(idx); };
    key.modifier = VK_LWIN;
    key.key = i;
    key.supressChain = true;
    registerHotkey(key);
  }
}

MainWindow::~MainWindow()
{
}

int MainWindow::messageLoop()
{
  MSG msg;
  while(GetMessage(&msg, NULL, 0, 0) > 0) {
    if (msg.message == WM_KEYUP && msg.wParam == VK_ESCAPE) {
      break;
    }

    // TODO WM_DISPLAYCHANGE (Desktop erweitert)
    if (msg.message == WM_TIMER && msg.wParam == 10) {
      update();
    }
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return msg.wParam;
}










