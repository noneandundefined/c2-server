#include <iphlpapi.h>
#include <windows.h>
#include <winhttp.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <direct.h>

#include "c2-ipgeo.h"

#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "iphlpapi.lib")

#define MAX_LINE_LENGTH 1024
#define MAX_RESPONSE_LENGTH 4096

char* get_env_value(const char* key) {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));

    char* last_slash = strrchr(cwd, '\\');
    if (last_slash) *last_slash = '\0';

    char env_path[2048];
    snprintf(env_path, sizeof(env_path), "%s\\.env", cwd);

    FILE* file = fopen(env_path, "r");
    if (!file) {
        printf("Failed open .env file\n");
        return NULL;
    }

    char line[MAX_LINE_LENGTH];
    char* value = NULL;
    char* token;

    while(fgets(line, sizeof(line), file)) {
        token = strtok(line, "=");
        if (token && strcmp(token, key) == 0) {
            token = strtok(NULL, "\n");
            if (token) {
                value = strdup(token);
                break;
            }
        }
    }

    fclose(file);
    return value;
}

char* ipgeo() {
    char* api_key = get_env_value("API_IPGEO");
    if (!api_key) {
        return NULL;
    }

    WCHAR hostname[] = L"api.ipgeolocation.io";

    char path[256];
    snprintf(path, sizeof(path), "/ipgeo?apiKey=%s", api_key);

    WCHAR wide_path[256];
    MultiByteToWideChar(CP_UTF8, 0, path, -1, wide_path, 256);

    HINTERNET hSession = WinHttpOpen(L"IPGeo Client/1.0",
                                    WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                                    WINHTTP_NO_PROXY_NAME,
                                    WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession) {
        printf("Error initial WinHTTP\n");
        free(api_key);
        return NULL;
    }

    HINTERNET hConnect = WinHttpConnect(hSession, hostname, INTERNET_DEFAULT_HTTPS_PORT, 0);
    if (!hConnect) {
        printf("Failed open request\n");
        WinHttpCloseHandle(hSession);
        free(api_key);
        return NULL;
    }

    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", wide_path,
                                            NULL, WINHTTP_NO_REFERER,
                                            WINHTTP_DEFAULT_ACCEPT_TYPES,
                                            WINHTTP_FLAG_SECURE);

    if (!hRequest) {
        printf("Failed open request\n");
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        free(api_key);
        return NULL;
    }

    if (!WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                           WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
        printf("Failed send request\n");
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        free(api_key);
        return NULL;
    }

    if (!WinHttpReceiveResponse(hRequest, NULL)) {
        printf("Failed get request\n");
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        free(api_key);
        return NULL;
    }

    char* response = (char*)malloc(MAX_RESPONSE_LENGTH);
    if (!response) {
        printf("Memory allocation failed\n");
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        free(api_key);
        return NULL;
    }

    DWORD bytesRead = 0;
    DWORD totalBytesRead = 0;

    while (WinHttpReadData(hRequest, response + totalBytesRead, MAX_RESPONSE_LENGTH - totalBytesRead, &bytesRead) && bytesRead > 0) {
        totalBytesRead += bytesRead;
    }

    response[totalBytesRead] = '\0';
    printf("Answer API:\n%s\n", response);

    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    free(api_key);

    return response;
}

int get_mac_address(uint8_t mac_address[6]) {
    IP_ADAPTER_INFO adapterInfo[16];
    DWORD bufLen = sizeof(adapterInfo);
    DWORD status = GetAdaptersInfo(adapterInfo, &bufLen);

    if (status != ERROR_SUCCESS) {
        printf("GetAdaptersInfo failed with error: %lu\n", status);
        return -1;
    }

    PIP_ADAPTER_INFO adapter = adapterInfo;

    if (adapter->AddressLength != 6) {
        printf("Unexpected MAC address length: %d\n", adapter->AddressLength);
        return -1;
    }

    memcpy(mac_address, adapter->Address, 6);
    return 0;
}
