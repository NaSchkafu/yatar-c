#include <ShellHook.h>
#include <Windows.h>
#include <vector>


// Reference counter for DLL
static int refCount = 0;
HHOOK hotkeyHook;

struct HotkeyExt {
  HotkeyExt(Hotkey &&data)
    :modifierPressed(false), hotkeyData(data)
  {}
  bool modifierPressed;
  Hotkey hotkeyData;
};
static std::vector<HotkeyExt> registeredHotkeys;

API_EXPORT bool registerHotkey(Hotkey key)
{
  HotkeyExt ext(std::move(key));
  registeredHotkeys.push_back(ext);
  return true;
}

LRESULT CALLBACK GlobalHotkeyProc(int nCode, WPARAM wParam, LPARAM lParam)
{
  bool handled = false;
  if (nCode == HC_ACTION) {
    auto kbdInfo = (LPKBDLLHOOKSTRUCT)lParam;
    for (auto &hkey : registeredHotkeys) {
      if (wParam == WM_KEYDOWN) {
        hkey.modifierPressed = hkey.modifierPressed || hkey.hotkeyData.modifier == kbdInfo->vkCode;
        if (hkey.modifierPressed && hkey.hotkeyData.key == kbdInfo->vkCode) {
          hkey.hotkeyData.callback();
          handled = handled || hkey.hotkeyData.supressChain;
        }
      } else if (wParam == WM_KEYUP) {
        hkey.modifierPressed = hkey.modifierPressed && hkey.hotkeyData.modifier != kbdInfo->vkCode;
      }
    }
  }
  if (handled)
    return 1;
  return CallNextHookEx(NULL, nCode, wParam, lParam);
}

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD fdwReason, LPVOID )
{
  switch (fdwReason) {
  case DLL_PROCESS_ATTACH:
    refCount++;
    break;
  case DLL_PROCESS_DETACH:
    refCount--;
    break;
  }

  if (refCount == 1) {
    // Register hooks
    hotkeyHook = SetWindowsHookEx(WH_KEYBOARD_LL, GlobalHotkeyProc, NULL, 0);
    if (!hotkeyHook) {
      throw "Could not register global keyboard hook";
    }
  } else {
    UnhookWindowsHookEx(hotkeyHook);
    // Unregister hooks
  }

  return TRUE;
}
