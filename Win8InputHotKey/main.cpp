#define   _WIN32_WINNT   0x0500   //   for   KBDLLHOOKSTRUCT

#include <stdio.h>
#include <windows.h>
#include <tchar.h>

HHOOK hKeyboardHook;
BOOL bMy = FALSE;LRESULT CALLBACK MyKeyboardProc(int nCode,WPARAM wParam,LPARAM lParam)
{
    KBDLLHOOKSTRUCT *Key_Info = (KBDLLHOOKSTRUCT*)lParam;
    if (HC_ACTION == nCode)
    {
        if (WM_KEYDOWN == wParam || WM_SYSKEYDOWN)
        {
            if (Key_Info->vkCode == VK_SPACE && (GetKeyState(VK_LWIN) & 0x8000) && bMy == FALSE)
            {
                bMy = TRUE;
                keybd_event(VK_CONTROL, MapVirtualKey(VK_CONTROL,0), 0,0); 
                keybd_event(VK_SPACE,0, 0 ,0);

                keybd_event(VK_CONTROL, MapVirtualKey(VK_CONTROL,0),KEYEVENTF_KEYUP,0); 
                keybd_event(VK_SPACE,0, KEYEVENTF_KEYUP ,0);
                bMy = FALSE;
                return 0;
            }
            if (Key_Info->vkCode == VK_SPACE && (GetKeyState(VK_CONTROL) & 0x8000) && bMy == FALSE)
            {
                bMy = TRUE;
                keybd_event(VK_LWIN, MapVirtualKey(VK_LWIN,0), 0,0); 
                keybd_event(VK_SPACE,0, 0 ,0);

                keybd_event(VK_LWIN, MapVirtualKey(VK_LWIN,0),KEYEVENTF_KEYUP,0); 
                keybd_event(VK_SPACE,0, KEYEVENTF_KEYUP ,0);
                bMy = FALSE;
                return 0;
            }
        }
    }
    return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam); //回调
}

DWORD WINAPI MyMouseLogger(LPVOID lpParm)
{
    HINSTANCE hInstance = GetModuleHandle(NULL);

    // here I put WH_MOUSE instead of WH_MOUSE_LL
    hKeyboardHook = SetWindowsHookEx( WH_KEYBOARD_LL, MyKeyboardProc, hInstance, NULL );

    MSG message;
    while (GetMessage(&message,NULL,0,0)) {
        TranslateMessage( &message );
        DispatchMessage( &message );
    }

    UnhookWindowsHookEx(hKeyboardHook);
    return 0;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
    OSVERSIONINFO osvi;
    BOOL bIsWindowsXPorLater;

    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    GetVersionEx(&osvi);
    if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 2)
    {
        HANDLE hThread;
        DWORD dwThread;

        hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MyMouseLogger, NULL, NULL, &dwThread);
        if (hThread)
            return WaitForSingleObject(hThread,INFINITE);
    }
    else
    {
        MessageBox(NULL, L"当前系统不是Win8", L"无法正常工作", MB_OK);
    }
    return 0;
}
