#include <windows.h>
#include <stdio.h>
#include <wchar.h>

#ifndef ICU_LOG_H
#define ICU_LOG_H

// void get_local_appdata_path(WCHAR* buffer, DWORD size);

// void create_directory(LPCWSTR path);

void dos_log(LPCWSTR url, DWORD status_code, double response_time_ms);

void errors_log(char* error);

void info_log(char* info);

#endif
