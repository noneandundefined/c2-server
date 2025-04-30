#include "ICU_dos.h"
#include "../ICU_log.h"

#pragma comment(lib, "winhttp.lib")

DWORD WINAPI send_dos(LPVOID lpParam) {
    ThreadData *data = (ThreadData *)lpParam;

    URL_COMPONENTS urlComp;
    WCHAR host[256];
    WCHAR urlPath[1024];

    memset(&urlComp, 0, sizeof(urlComp));
    urlComp.dwStructSize = sizeof(urlComp);
    urlComp.lpszHostName = host;
    urlComp.dwHostNameLength = sizeof(host) / sizeof(WCHAR);
    urlComp.lpszUrlPath = urlPath;
    urlComp.dwUrlPathLength = sizeof(urlPath) / sizeof(WCHAR);

    if (!WinHttpCrackUrl(data->url, 0, 0, &urlComp)) {
        char thread_error[255];
        snprintf(thread_error, 255, "[Thread %lu] Failed to crack URL", GetCurrentThreadId());
        errors_log(thread_error);
        return 1;
    }

    for (int i = 0; i < data->request_per_thread; ++i) {
        HINTERNET hSession = WinHttpOpen(L"A WinHTTP Example/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
        if (!hSession) {
            char session_error[255];
            snprintf(session_error, 255, "[Thread %lu] WinHttpOpen failed", GetCurrentThreadId());
            errors_log(session_error);
            continue;
        }

        HINTERNET hConnect = WinHttpConnect(hSession, urlComp.lpszHostName,urlComp.nPort, 0);
        if (!hConnect) {
            char connect_error[255];
            snprintf(connect_error, 255, "[Thread %lu] WinHttpConnect failed", GetCurrentThreadId());
            errors_log(connect_error);
            continue;
        }

        HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", urlComp.lpszUrlPath, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, (urlComp.nScheme == INTERNET_SCHEME_HTTPS) ? WINHTTP_FLAG_SECURE : 0);
        if (!hRequest) {
            char request_error[255];
            snprintf(request_error, 255, "[Thread %lu] WinHttpOpenRequest failed", GetCurrentThreadId());
            errors_log(request_error);

            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hSession);
            continue;
        }

        LARGE_INTEGER freq, start, end;
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&start);

        BOOL bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
        if (bResults) {
            bResults = WinHttpReceiveResponse(hRequest, NULL);

            QueryPerformanceCounter(&end);
            double elapsedMs = ((double)(end.QuadPart - start.QuadPart) * 1000.0) / freq.QuadPart;

            DWORD statusCode = 0;
            DWORD statusCodeSize = sizeof(statusCode);

            if (WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX, &statusCode, &statusCodeSize, WINHTTP_NO_HEADER_INDEX)) {
                dos_log(data->url, statusCode, elapsedMs);
            } else {
                char request_errors[255];
                snprintf(request_errors, 255, "Failed sent request to %s", data->url);
                errors_log(request_errors);
            }
        } else {
            char results_error[255];
            snprintf(results_error, 255, "[Thread %lu] Request to %ls failed", GetCurrentThreadId(), data->url);
            errors_log(results_error);
        }

        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
    }

    return 0;
}

void start_dos(LPCWSTR *urls, int url_count, int threads_per_url, int requests_per_thread) {
    for (int i = 0; i < url_count; ++i) {
        HANDLE *threads = (HANDLE *)malloc(sizeof(HANDLE) * threads_per_url);
        ThreadData *threadData = (ThreadData *)malloc(sizeof(ThreadData) * threads_per_url);

        for (int j = 0; j < threads_per_url; ++j) {
            threadData[i].url = urls[i];
            threadData[i].request_per_thread = requests_per_thread;
            threads[i] = CreateThread(NULL, 0, send_dos, &threadData[i], 0, NULL);
        }

        WaitForMultipleObjects(threads_per_url, threads, TRUE, INFINITE);

        for (int j = 0; j < threads_per_url; ++j) {
            CloseHandle(threads[i]);
        }

        free(threads);
        free(threadData);
    }
}

// int wmain() {
//     LPCWSTR urls[] = {
//         L"https://example.com/",
//         L"https://httpbin.org/get"
//     };
//     int url_count = sizeof(urls) / sizeof(urls[0]);
//     int threads_per_url = 5;
//     int requests_per_thread = 10;

//     run(urls, url_count, threads_per_url, requests_per_thread);

//     return 0;
// }
