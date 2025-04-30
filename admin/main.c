#include <windows.h>
#include <stdio.h>

#include "core/admin.h"
#include "core/conn.h"
#include "stdlib/env.h"

int main() {
    printf("\033[31m");
    printf("  _____ _____ _    _ \n");
    printf(" |_   _/ ____| |  | |                TOOLS INSTALLER\n");
    printf("   | || |    | |  | |\n");
    printf("   | || |    | |  | |  Author: noneandundefined\n");
    printf("  _| || |____| |__| |  GitHub: github.com/noneandundefined/icu\n");
    printf(" |_____\\_____|\\____/ \n");
    printf("\033[0m");

    printf("\n\033[31m:: Disclaimer:\033[0m \033[33m[!] ICU is not responsible for DDoS attacks or unauthorized access. \nThe responsibility for such actions lies with the violators.\n\033[0m ");

    char* SERVER_ADDR = get_env_value("SERVER_ADDR");
    char* SERVER_PORT = get_env_value("SERVER_PORT");
    unsigned short UNSIGNED_SERVER_PORT = (unsigned short)atoi(SERVER_PORT);
    is_connection(SERVER_ADDR, UNSIGNED_SERVER_PORT);

    input();

    system("pause");
    return 0;
}
