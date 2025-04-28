#include <shlobj.h>
#include <time.h>

#include "stdlib/ICU_env.h"
#include "ICU_log.h"

#pragma comment(lib, "shell32.lib")

void get_local_appdata_path(WCHAR* buffer, DWORD size) {
    PWSTR path = NULL;
    if (SUCCEEDED(SHGetKnownFolderPath(&FOLDERID_LocalAppData, 0, NULL, &path))) {
        wcsncpy(buffer, path, size);
        CoTaskMemFree(path);
    } else {
        wprintf(L"Failed to get Local AppData path\n");
    }
}

void create_directories(LPCWSTR path) {
    WCHAR tmp[MAX_PATH];
    wcscpy(tmp, path);

    WCHAR* p = tmp;
    if (tmp[1] == L':' && tmp[2] == L'\\') {
        p += 3;
    }

    for (; *p; p++) {
        if (*p == L'\\') {
            *p = 0;
            if (!CreateDirectoryW(tmp, NULL)) {
                DWORD err = GetLastError();
                if (err != ERROR_ALREADY_EXISTS) {
                    wprintf(L"Failed to create directory: %ls (error %lu)\n", tmp, err);
                    return;
                }
            }

            *p = L'\\';
        }
    }

    if (!CreateDirectoryW(tmp, NULL)) {
        DWORD err = GetLastError();
        if (err != ERROR_ALREADY_EXISTS) {
            wprintf(L"Failed to create directory: %ls (error %lu)\n", tmp, err);
            return;
        }
    }
}

void dos_log(LPCWSTR url, DWORD status_code, double response_time_ms) {
    WCHAR basePath[MAX_PATH];
    WCHAR appPath[MAX_PATH];
    WCHAR datePath[MAX_PATH];
    WCHAR finalPath[MAX_PATH];
    WCHAR dateFolder[64];

    SYSTEMTIME st;
    GetLocalTime(&st);

    get_local_appdata_path(basePath, MAX_PATH);

    char* APP_DATA_FOLDER = get_env_value("APP_DATA_FOLDER");

    swprintf(appPath, MAX_PATH, L"%ls\\%s", basePath, APP_DATA_FOLDER);
    create_directories(appPath);

    swprintf(dateFolder, 64, L"%02d.%02d.%04d", st.wDay, st.wMonth, st.wYear);

    swprintf(datePath, MAX_PATH, L"%ls\\%ls", appPath, dateFolder);
    create_directories(datePath);

    swprintf(finalPath, MAX_PATH, L"%ls\\DOS.log", datePath);

    HANDLE hFile = CreateFileW(
        finalPath,
        FILE_APPEND_DATA,
        FILE_SHARE_READ,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        wprintf(L"Failed to open log file (error %lu)\n", GetLastError());
        return;
    }

    char buffer[1024];
    int len = sprintf(buffer, "[%02d.%02d.%04d %02d:%02d:%02d] %ls %lu %.2fms\n",
                      st.wDay, st.wMonth, st.wYear,
                      st.wHour, st.wMinute, st.wSecond,
                      url, status_code, response_time_ms);

    DWORD written;
    WriteFile(hFile, buffer, len, &written, NULL);
    CloseHandle(hFile);
}


void errors_log(char* error) {
    WCHAR basePath[MAX_PATH];
    WCHAR appPath[MAX_PATH];
    WCHAR datePath[MAX_PATH];
    WCHAR finalPath[MAX_PATH];
    WCHAR dateFolder[64];

    SYSTEMTIME st;
    GetLocalTime(&st);

    get_local_appdata_path(basePath, MAX_PATH);

    char* APP_DATA_FOLDER = get_env_value("APP_DATA_FOLDER");

    swprintf(appPath, MAX_PATH, L"%ls\\%s", basePath, APP_DATA_FOLDER);
    create_directories(appPath);

    swprintf(dateFolder, 64, L"%02d.%02d.%04d", st.wDay, st.wMonth, st.wYear);

    swprintf(datePath, MAX_PATH, L"%ls\\%ls", appPath, dateFolder);
    create_directories(datePath);

    swprintf(finalPath, MAX_PATH, L"%ls\\ERRORS.log", datePath);

    HANDLE hFile = CreateFileW(
        finalPath,
        FILE_APPEND_DATA,
        FILE_SHARE_READ,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        return;
    }

    char buffer[1024];
    int len = sprintf(buffer, "[%02d.%02d.%04d %02d:%02d:%02d] ERROR %s\n",
                      st.wDay, st.wMonth, st.wYear,
                      st.wHour, st.wMinute, st.wSecond, error);

    DWORD written;
    WriteFile(hFile, buffer, len, &written, NULL);
    CloseHandle(hFile);
}

void info_log(char* info) {
    WCHAR basePath[MAX_PATH];
    WCHAR appPath[MAX_PATH];
    WCHAR datePath[MAX_PATH];
    WCHAR finalPath[MAX_PATH];
    WCHAR dateFolder[64];

    SYSTEMTIME st;
    GetLocalTime(&st);

    get_local_appdata_path(basePath, MAX_PATH);

    char* APP_DATA_FOLDER = get_env_value("APP_DATA_FOLDER");

    swprintf(appPath, MAX_PATH, L"%ls\\%s", basePath, APP_DATA_FOLDER);
    create_directories(appPath);

    swprintf(dateFolder, 64, L"%02d.%02d.%04d", st.wDay, st.wMonth, st.wYear);

    swprintf(datePath, MAX_PATH, L"%ls\\%ls", appPath, dateFolder);
    create_directories(datePath);

    swprintf(finalPath, MAX_PATH, L"%ls\\INFO.log", datePath);

    HANDLE hFile = CreateFileW(
        finalPath,
        FILE_APPEND_DATA,
        FILE_SHARE_READ,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        return;
    }

    char buffer[1024];
    int len = sprintf(buffer, "[%02d.%02d.%04d %02d:%02d:%02d] INFO %s\n",
                      st.wDay, st.wMonth, st.wYear,
                      st.wHour, st.wMinute, st.wSecond, info);

    DWORD written;
    WriteFile(hFile, buffer, len, &written, NULL);
    CloseHandle(hFile);
}
