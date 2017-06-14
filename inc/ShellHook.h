#pragma once

#include <Windows.h>
#include <functional>

#ifdef DLL_BUILD
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


API_EXPORT bool registerHotkey(Hotkey key);