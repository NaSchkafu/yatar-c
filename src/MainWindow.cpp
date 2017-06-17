//
// Created by Fluxy on 16.06.2016.
//
#include <iostream>
#include "../inc/MainWindow.h"
#include "ShellHook.h"


MainWindow::MainWindow()
{
  registerKeys();

  if (!RegisterShellHookWindow(hwnd())) {
    throw "Could not register shell hook window";
  }
  m_shellHookId = RegisterWindowMessage(TEXT("SHELLHOOK"));
}

void MainWindow::registerKeys()
{
  RegisterHotKey(NULL, S_CHANGE_WIN_HOTKEY, MOD_WIN, 0x4b);
}

MainWindow::~MainWindow()
{
}

int MainWindow::messageLoop()
{
  MSG msg;
  while(GetMessage(&msg, NULL, 0, 0) > 0) {
    if (msg.message == m_shellHookId) {
      switch(msg.wParam) {
      case HSHELL_WINDOWACTIVATED:
        activeWindowChanged(reinterpret_cast<HWND>(msg.lParam));
        break;
      case HSHELL_WINDOWCREATED:
      case HSHELL_WINDOWDESTROYED:
        update();
        break;
      }
    }

    if (msg.message == WM_KEYUP && msg.wParam == VK_ESCAPE) {
      break;
    } else if (msg.message == WM_HOTKEY && msg.wParam == S_CHANGE_WIN_HOTKEY) {
      selectWindow();
    }

    // TODO WM_DISPLAYCHANGE (Desktop erweitert)

    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return msg.wParam;
}