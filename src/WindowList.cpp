//
// Created by Fluxy on 18.06.2016.
//

#include "WindowList.h"

void WindowList::update()
{
  m_windows.clear();
  EnumWindows(enumWindowsProc, reinterpret_cast<LPARAM>(this));
}

BOOL WindowList::enumWindowsProc(HWND hwnd, LPARAM lParam)
{
  if(lParam != 0) {
    auto instance = reinterpret_cast<WindowList*>(lParam);
    instance->insertWindow(hwnd);
    return TRUE;
  }
  return FALSE;
}

void WindowList::insertWindow(HWND hwnd)
{
  if(isTabbarWindow(hwnd)) {
    Window info;
    wchar_t text[255];
    GetWindowText(hwnd, text, 255);
    info.title = text;
    info.hwnd = hwnd;
    m_windows.push_back(info);
  }
}

bool WindowList::isTabbarWindow(HWND hwnd)
{
  // Only windows in ALT+TAB
  // see https://blogs.msdn.microsoft.com/oldnewthing/20071008-00/?p=24863
  if (IsWindowVisible(hwnd) && !(GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW)) {
    auto hwndWalk = GetAncestor(hwnd, GA_ROOTOWNER);
    auto hwndTry = GetLastActivePopup(hwndWalk);
    while (hwndWalk != hwndTry) {
      auto style = GetWindowLong(hwndTry, GWL_EXSTYLE);
      if ((IsWindowVisible(hwndTry) && !(style & WS_EX_TOOLWINDOW)) || (style & WS_EX_APPWINDOW))
        break;
      hwndWalk = hwndTry;
      hwndTry = GetLastActivePopup(hwndWalk);
    }

    return hwndWalk == hwndTry;
  }

  return false;
}

const std::vector<Window> &WindowList::windows() const
{
  return m_windows;
}









