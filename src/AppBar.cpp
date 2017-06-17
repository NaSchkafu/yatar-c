//
// Created by Fluxy on 18.06.2016.
//

#include <iostream>
#include <map>
#include <algorithm>
#include "MainWindow.h"
#include "AppBar.h"
#include "ShellHook.h"
#include <future>

std::map<HWND, AppBar*> registry;

void trackMouseLeave(HWND hwnd, bool &alreadyTracked)
{
  if (!alreadyTracked) {
    TRACKMOUSEEVENT track;
    track.cbSize = sizeof(TRACKMOUSEEVENT);
    track.dwFlags = TME_LEAVE;
    track.hwndTrack = hwnd;
    TrackMouseEvent(&track);
    alreadyTracked = true;
  }
}

LRESULT CALLBACK AppBar::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  auto appBar = registry.find(hWnd) == registry.end() ? nullptr : registry[hWnd];
  switch (msg) {
  case WM_CLOSE:
    DestroyWindow(hWnd);
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  }

  if (appBar) {
    switch (msg) {
    case WM_MOUSEMOVE: {
      trackMouseLeave(hWnd, appBar->m_isTracked);
      auto mousePos = MAKEPOINTS(lParam);
      appBar->drawRects(mousePos.x, mousePos.y);
    }
    break;
    case WM_MOUSELEAVE: {
      appBar->m_isTracked = false;
      auto mousePos = MAKEPOINTS(lParam);
      appBar->drawRects(mousePos.x, mousePos.y);
    }
    break;
    case WM_LBUTTONUP: {
      auto mousePos = MAKEPOINTS(lParam);
      appBar->clicked(mousePos.x, mousePos.y);
    }
    default:
      break;
    }
  }

  return DefWindowProcW(hWnd, msg, wParam, lParam);
}

void AppBar::clicked(SHORT mouseX, SHORT mouseY)
{
  POINT p = { mouseX, mouseY };
  ScreenToClient(m_hwnd, &p);
  for (auto &w : m_drawnWindows) {
    if (PtInRect(&w.appBarRect, p)) {
      activateWindow(w);
    }
  }
}

void AppBar::activateWindow(HWND hwnd) const
{
  auto foregroundThread = GetWindowThreadProcessId(GetForegroundWindow(), nullptr);
  auto myThread = GetCurrentThreadId();
  if (foregroundThread != myThread) {
    AttachThreadInput(foregroundThread, myThread, TRUE);
    SetForegroundWindow(m_hwnd);
    SetFocus(m_hwnd);
  }

  SetForegroundWindow(hwnd);
  auto flag = IsIconic(hwnd) ? SW_RESTORE : SW_SHOW;
  ShowWindow(hwnd, flag);


  if (foregroundThread != myThread) {
    AttachThreadInput(foregroundThread, myThread, FALSE);
  }
}

void AppBar::activateWindow(const Window& w) const
{
  activateWindow(w.hwnd);
}


void AppBar::activeWindowChanged(HWND hwnd )
{
  auto found = false;
  for (auto &w : m_drawnWindows) {
    if (w.hwnd == hwnd) {
      found = true;
      break;
    }
  }

  found = found || (m_selectMode && GetForegroundWindow() != m_hwnd);
  m_selectMode = m_selectMode && GetForegroundWindow() == m_hwnd;
  if (found) {
    drawWindowList();
  }
}

void AppBar::drawRects(SHORT mouseX, SHORT mouseY)
{
  POINT p = { mouseX, mouseY };
  ScreenToClient(m_hwnd, &p);
  auto hdc = GetDC(m_hwnd);
  SelectObject(hdc, GetStockObject(DC_PEN));
  SelectObject(hdc, GetStockObject(DC_BRUSH));
  SelectObject(hdc, m_font);
  SetTextColor(hdc, RGB(0x00, 0xff, 0x00));
  SetBkMode(hdc, TRANSPARENT);
  auto activeHwnd = GetForegroundWindow();

  for (auto &w : m_drawnWindows) {
    if (PtInRect(&w.appBarRect, p)) {
      SetDCBrushColor(hdc, RGB(0xa0, 0xa0, 0xa0));
    } else {
      SetDCBrushColor(hdc, RGB(0x00, 0x00, 0x00));
    }
    if (w.hwnd == activeHwnd) {
      SetDCPenColor(hdc, RGB(0x00, 0x00, 0xff));
    } else {
      SetDCPenColor(hdc, RGB(0x00, 0xff, 0x00));
    }
    Rectangle(hdc, w.appBarRect.left, w.appBarRect.top, w.appBarRect.right, w.appBarRect.bottom);
    DrawText(hdc, w.title.c_str(), w.title.size(), &w.appBarRect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

  }
}

void AppBar::update()
{
  m_windows.update();
  drawWindowList();
}

AppBar::AppBar()
  :m_isTracked(false)
{
  registerClass();
  createWindow();
  registry[m_hwnd] = this;

  // TODO Refactor (sollte von außen kommen)
  m_height = MulDiv(12, GetDeviceCaps(GetWindowDC(NULL), LOGPIXELSY), 72);

  NONCLIENTMETRICS metrics;
  metrics.cbSize = sizeof(NONCLIENTMETRICS);
  SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &metrics, 0);
  m_font = CreateFontIndirect(&metrics.lfMenuFont);

  setAsAppBar();
  setPosition();

  update();
}


AppBar::~AppBar()
{
  DeleteObject(m_font);
  registry.erase(m_hwnd);
}

void AppBar::createWindow()
{
  m_hwnd = CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST, S_CLASS_NAME, 0, WS_BORDER,
                          CW_USEDEFAULT, CW_USEDEFAULT, 1, 1, NULL, NULL, GetModuleHandle(NULL), NULL);
  SetWindowLong(m_hwnd, GWL_STYLE, 0);
  ShowWindow(m_hwnd, SW_SHOW);
  UpdateWindow(m_hwnd);
}

bool AppBar::setAsAppBar()
{
  m_appbarData.cbSize = sizeof(m_appbarData);
  m_appbarData.hWnd = m_hwnd;
  m_appbarData.uCallbackMessage = WM_USER;
  // TODO Fehlerhandling
  return SHAppBarMessage(ABM_NEW, &m_appbarData) != 0;
}

void AppBar::setPosition()
{
  auto width = GetSystemMetrics(SM_CXFULLSCREEN);
  m_appbarData.rc.left = 0;
  m_appbarData.rc.right = width;
  m_appbarData.rc.top = 0;
  m_appbarData.rc.bottom = m_height + 10 ;
  m_appbarData.uEdge = ABE_TOP;
  SHAppBarMessage(ABM_QUERYPOS, &m_appbarData);
  SHAppBarMessage(ABM_SETPOS, &m_appbarData);
  MoveWindow(m_appbarData.hWnd, m_appbarData.rc.left, m_appbarData.rc.top, m_appbarData.rc.right, m_appbarData.rc.bottom, true);
}

void AppBar::registerClass()
{
  if(!S_ALREADY_REGISTERED) {
    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
    wc.lpszMenuName = NULL;
    wc.lpszClassName = S_CLASS_NAME;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    if(!RegisterClassEx(&wc)) {
      throw "Could not register class";
    }

    S_ALREADY_REGISTERED = true;
  }
}

void AppBar::updateList()
{
  auto newWindows = m_windows.windows();
  auto it = m_drawnWindows.begin();
  while (it != m_drawnWindows.end()) {
    auto newWindowIt = std::find_if(newWindows.begin(), newWindows.end(), [&it](const Window &newWindow) {
      return it->hwnd == newWindow.hwnd;
    });
    if (newWindowIt == newWindows.end()) {
      it = m_drawnWindows.erase(it);
    } else {
      *it++ = *newWindowIt;
      newWindows.erase(newWindowIt);
    }
  }

  std::copy(newWindows.begin(), newWindows.end(), std::back_inserter(m_drawnWindows));
}

void AppBar::drawWindowList()
{
  SIZE size;
  auto hdc = GetDC(m_hwnd);
  SelectObject(hdc, m_font);
  SelectObject(hdc, GetStockObject(DC_PEN));
  SetDCPenColor(hdc, RGB(0xff, 0xff, 0xff));


  if (GetTextExtentPoint32W(hdc, m_dots.c_str(), m_dots.size(), &size) == 0) {
    MessageBoxW(m_hwnd, L"Could not calculate text width", L"", MB_ICONERROR);
    return;
  }
  auto dotsWidth = size.cx;

  RECT rect;
  if (GetWindowRect(m_hwnd, &rect)) {
    updateList();

    auto width = (rect.right - rect.left) / m_drawnWindows.size();
    for (auto i = 0u; i < m_drawnWindows.size(); i++) {
      auto &window = m_drawnWindows[i];
      window.title = L"[ " + window.title + L" ]";
      if(m_selectMode && i < 9) {
        window.title = L"(" + std::to_wstring(i + 1) + L") " + window.title;
      }
      auto x = rect.left + width * i + (i ? 0 : 1);
      window.appBarRect.left = x;
      window.appBarRect.top = 1 + rect.top;
      window.appBarRect.right = rect.left + width * (i + 1);
      window.appBarRect.bottom = rect.bottom;

      int maxWidth;
      if (GetTextExtentExPoint(hdc, window.title.c_str(), window.title.size(), width - dotsWidth - 5, &maxWidth, NULL, &size)) {
        if (maxWidth != window.title.size()) {
          window.title = window.title.substr(0, maxWidth) + L" ]";
        }
      } else {
        MessageBox(m_hwnd, L"Could not call GetTextExtentExPoint", L"", MB_ICONERROR);
      }

    }

    POINT mousePos;
    GetCursorPos(&mousePos);
    drawRects(mousePos.x, mousePos.y);
  }
}

void AppBar::switchToIdx(int idx)
{
  if (idx < m_windows.windows().size()) {
    activateWindow(m_drawnWindows[idx]);
  }
}

HWND AppBar::hwnd() const
{
  return m_hwnd;
}

void AppBar::selectWindow()
{
  m_selectMode = true;
  drawWindowList();
  activateWindow(m_hwnd);
}

bool AppBar::S_ALREADY_REGISTERED = false;
const wchar_t* AppBar::S_CLASS_NAME = TEXT("TABBINGBAR");

