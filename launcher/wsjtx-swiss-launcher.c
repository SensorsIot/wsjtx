/*
 * WSJT-SWISS Launcher
 *
 * Small launcher that sets the DLL search path to lib\ subdirectory
 * and launches the real wsjtx.exe from there.
 *
 * Compile with: gcc -mwindows -o wsjtx-swiss.exe wsjtx-swiss-launcher.c
 */

#include <windows.h>
#include <stdlib.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {

    WCHAR exePath[MAX_PATH];
    WCHAR libPath[MAX_PATH];
    WCHAR wsjtxPath[MAX_PATH];

    // Get the directory where this launcher is located
    GetModuleFileNameW(NULL, exePath, MAX_PATH);

    // Remove the exe filename to get directory
    WCHAR *lastSlash = wcsrchr(exePath, L'\\');
    if (lastSlash) {
        *lastSlash = L'\0';
    }

    // Build path to lib subdirectory
    swprintf(libPath, MAX_PATH, L"%s\\lib", exePath);

    // Build path to wsjtx.exe in lib
    swprintf(wsjtxPath, MAX_PATH, L"%s\\lib\\wsjtx.exe", exePath);

    // Add lib directory to DLL search path
    SetDllDirectoryW(libPath);

    // Also set current directory to lib (some Qt plugins need this)
    SetCurrentDirectoryW(libPath);

    // Launch wsjtx.exe
    STARTUPINFOW si = {0};
    PROCESS_INFORMATION pi = {0};
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_SHOW;

    // Pass through any command line arguments
    WCHAR cmdLine[MAX_PATH * 2];
    if (lpCmdLine && lpCmdLine[0]) {
        swprintf(cmdLine, MAX_PATH * 2, L"\"%s\" %S", wsjtxPath, lpCmdLine);
    } else {
        swprintf(cmdLine, MAX_PATH * 2, L"\"%s\"", wsjtxPath);
    }

    if (!CreateProcessW(NULL, cmdLine, NULL, NULL, FALSE, 0, NULL, libPath, &si, &pi)) {
        MessageBoxW(NULL, L"Failed to start WSJT-SWISS.\n\nMake sure wsjtx.exe exists in the lib subfolder.",
                    L"WSJT-SWISS Launcher", MB_ICONERROR);
        return 1;
    }

    // Close handles - we don't wait for the process
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
