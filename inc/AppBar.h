//
// Created by Fluxy on 18.06.2016.
//

#ifndef YATAR_C_APPBAR_H
#define YATAR_C_APPBAR_H


#include <windows.h>
#include <memory>
#include "WindowList.h"
#include "common.h"

class AppBar {
public:
  virtual ~AppBar();

  AppBar(const AppBar&) = delete;
  AppBar(const AppBar&&) = delete;
  AppBar& operator=(const AppBar&) = delete;
  AppBar();


  void update();

  void switchToIdx(int idx);

  HWND hwnd() const;
  void selectWindow();
  void activeWindowChanged(HWND hwnd);

private:
  void createWindow();
  bool setAsAppBar();
  void setPosition();
  void drawWindowList();
  static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
  void clicked(SHORT mouseX, SHORT mouseY);
  void activateWindow(HWND hwnd) const;
  void activateWindow(const Window &w) const;
  void drawRects(SHORT mouseX, SHORT mouseY);

  static void registerClass();

  void updateList();

private:
  APPBARDATA m_appbarData;
  HWND m_hwnd;
  HFONT m_font;
  int m_height;
  std::wstring m_dots;

  WindowList m_windows;
  std::vector<Window> m_drawnWindows;

  bool m_isTracked;
  bool m_selectMode;
private:
  static bool S_ALREADY_REGISTERED;
  static const wchar_t* S_CLASS_NAME;
};


#endif //YATAR_C_APPBAR_H
