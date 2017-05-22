//
// Created by Fluxy on 16.06.2016.
//
#include <iostream>
#include "../inc/MainWindow.h"

// Indicates that the WIN Key is pressed currently
static bool winPressed = false;

LRESULT CALLBACK GlobalHoteyProc(int nCode, WPARAM wParam, LPARAM lParam)
{
  bool handled = false;
  if (nCode == HC_ACTION) {
    auto kbdInfo = (LPKBDLLHOOKSTRUCT)lParam;
    if (wParam == WM_KEYDOWN) {
      winPressed = winPressed || VK_LWIN == kbdInfo->vkCode;
      if (winPressed) {
        switch (kbdInfo->vkCode) {
        case 0x31:
          MessageBeep(0xFFFFFFFF);
          handled = true;
          break;
        }
      }
    } else if (wParam == WM_KEYUP) {
      winPressed = winPressed && VK_LWIN != kbdInfo->vkCode;
    }
  }

  if (handled)
    return 1;
  return CallNextHookEx(NULL, nCode, wParam, lParam);
}

MainWindow::MainWindow()
{
  m_hotkeyHook= SetWindowsHookEx(WH_KEYBOARD_LL, GlobalHoteyProc, NULL, 0);
  if (!m_hotkeyHook) {
    throw "Could not register global keyboard hook";
  }
}

MainWindow::~MainWindow()
{
  UnhookWindowsHookEx(m_hotkeyHook);
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










