#include <windows.h>
#include <stdio.h>

DWORD WINAPI BotThread(LPVOID lpParam) {
    MessageBoxA(NULL, "Привет из DLL в проводнике!", "Injected", MB_OK);
    while (1) {
        Sleep(1000); // Проверка на установку .exe и запуск его
    }

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, 0, BotThread, NULL, 0, NULL);
    }

    return TRUE;
}
