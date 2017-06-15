#pragma once

#include <Windows.h>
#include <functional>

#ifdef _WINDLL
#define API_EXPORT __declspec(dllexport)
#else
#define API_EXPORT __declspec(dllimport)
#endif

struct Hotkey {
  DWORD modifier;
  DWORD key;
  BOOL supressChain;

  std::function<void()> callback;
};

using WindowCallback = std::function<void(HWND, BOOL)>;

API_EXPORT void registerHotkey(Hotkey key);
API_EXPORT void registerWindowActivatedCallback(WindowCallback callback);