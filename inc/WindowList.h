//
// Created by Fluxy on 18.06.2016.
//

#ifndef YATAR_C_WINDOWFINDER_H
#define YATAR_C_WINDOWFINDER_H

#include <string>
#include <vector>

#include <windows.h>

struct Window
{
    std::wstring title;
    HWND hwnd;

	RECT appBarRect;
};

class WindowList {
public:
    void update();
    const std::vector<Window>& windows() const;

private:
    bool isTabbarWindow(HWND hwnd);
    void insertWindow(HWND hwnd);

    static BOOL CALLBACK enumWindowsProc(HWND hwnd, LPARAM lParam);

private:
    std::vector<Window> m_windows;
};


#endif //YATAR_C_WINDOWFINDER_H
