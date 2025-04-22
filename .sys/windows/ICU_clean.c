#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

void remove_from_reestr() {
    HKEY hKey;

    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &hKey) == ERROR_SUCCESS) {
        RegDeleteValueA(hKey, "windows-x86_x64"); // MyBot_x86_x64

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

    char app_data_file[MAX_PATH];
    snprintf(app_data_file, MAX_PATH, "%s\\AppData\\Roaming\\windows-x86_x64\\windows-x86_x64.exe", is_user);
    delete_file(app_data_file);
}

void delete_file_from_program_files() {
    const char *program_files_file = "C:\\Program Files\\windows-x86_x64\\windows-x86_x64.exe";
    deleteFile(program_files_file);

    const char *main_file = "C:\\Program Files\\windows-x86_x64\\icu-windows.exe";
    delete_file(main_file);
}

int main() {
    delete_files_app_data();

    delete_file_from_program_files();

    return 0;
}
