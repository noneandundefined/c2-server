#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

#include "stdlib/ICU_env.h"

void remove_from_reestr() {
    HKEY hKey;

    char* n_reestr = get_env_value("WIN_REESTR_NAME");

    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &hKey) == ERROR_SUCCESS) {
        RegDeleteValueA(hKey, n_reestr);

        RegCloseKey(hKey);
    }
}

void delete_file(const char *file_path) {
    if (!DeleteFileA(file_path)) {
        printf("Error deleted files %s: %lu", file_path, GetLastError());
    }
}

void delete_files_app_data() {
    char is_user[MAX_PATH];
    DWORD result = GetEnvironmentVariableA("USERPROFILE", is_user, MAX_PATH);
    if (result == 0) {
        printf("Error get user: %lu", GetLastError());
        return;
    }

    char* n_script = get_env_value("SCRIPT_NAME");
    char* n_appdata = get_env_value("APP_DATA_FOLDER");

    char app_data_file[MAX_PATH];
    snprintf(app_data_file, MAX_PATH, "%s\\AppData\\Roaming\\%s\\%s.exe", is_user, n_appdata, n_script);
    delete_file(app_data_file);
}

void delete_file_from_program_files() {
    char* n_app = get_env_value("APP_NAME");
    char* n_script = get_env_value("SCRIPT_NAME");
    char* n_appdata = get_env_value("APP_DATA_FOLDER");

    char program_files_file[MAX_PATH];
    snprintf(program_files_file, MAX_PATH, "C:\\Program Files\\%s\\%s.exe", n_appdata, n_script);
    delete_file(program_files_file);

    char main_file[MAX_PATH];
    snprintf(main_file, MAX_PATH, "C:\\Program Files\\%s\\%s.exe", n_appdata, n_app);
    delete_file(main_file);
}

int main() {
    remove_from_reestr();

    delete_files_app_data();

    delete_file_from_program_files();

    system("pause");

    return 0;
}
