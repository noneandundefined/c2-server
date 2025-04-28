#include <windows.h>
#include <winhttp.h>
#include <stdio.h>

#ifndef ICU_DOS_H
#define ICU_DOS_H

typedef struct {
    LPCWSTR url;
    int request_per_thread;
} ThreadData;

DWORD WINAPI send_dos(LPVOID lpParam);

void start_dos(LPCWSTR *urls, int url_count, int threads_per_url, int requests_per_thread);

#endif
